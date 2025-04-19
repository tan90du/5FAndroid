本模块是对jpeg的封装，支持libjpeg-turbo和jpeg硬编

yuv -> jpg :

```
void doJpegTest() {

    JpgUtil *jpgUtil = new JpgUtil(JpgCodecType::CodecTypeSoft);

    BYTE *jpgBuffer = NULL;
    int jpgSize = 0;

    int yuv_size = 1920 * 1080 * 3 / 2;
    byte *yuv_buffer = new byte[yuv_size];
    FILE *fp_in = fopen("/storage/emulated/0/Android/data/com.autonavi.plugin/cache/jpeg_turbo_test.yuv", "rb+");
    if (fp_in != NULL) {
        fread(yuv_buffer, yuv_size, 1024, fp_in);
    } else {
        S_LOGI(">>>找不到yuv文件！");
        return;
    }
    fclose(fp_in);

    jpgUtil->doEncodeJpeg(yuv_buffer, yuv_size, 0, 1920, 1080, 4, 80, &jpgBuffer, jpgSize);
    
    FILE *fp_out = NULL;
    if ((fp_out = fopen("/storage/emulated/0/Android/data/com.autonavi.plugin/cache/111_11.jpg", "wb+")) == NULL) {
        S_LOGI("文件创建失败！");
    }else{
         S_LOGI("%s line=%d",__FILE__,__LINE__);
        fwrite(jpgBuffer, 1, jpgSize, fp_out);
        fclose(fp_out);

        S_LOGI( "文件创建成功！");
    }
} 

```

软编 jpeg -> yuv 

```
void doJpegToYuvTest() {

    JpgUtil *jpgUtil = new JpgUtil(JpgCodecType::CodecTypeSoft);

    int yuvSize;
    std::string yuvType;
    uint8_t *yuvBuffer;
    int ret = jpgUtil->doDecodeJpegToYuv("/storage/emulated/0/Android/data/com.autonavi.plugin/cache/111_11.jpg", &yuvBuffer, &yuvSize, &yuvType);

    if (ret > -1) {
        char yuvFileName[300];
        sprintf(yuvFileName, "/storage/emulated/0/Android/data/com.autonavi.plugin/cache/8888%s",
                yuvType.c_str());
        FILE *yuvFile = fopen(yuvFileName, "wb");
        fwrite(yuvBuffer, yuvSize, 1, yuvFile);

        fflush(yuvFile);
        fclose(yuvFile);
    }

    free(yuvBuffer);
}

```


硬编 jpeg -> yuv

```
void AppTest::doJpegHardEncodeTest() {

    JpgUtil *jpgUtil = new JpgUtil(JpgCodecType::CodecTypeHard);

    uint8_t *jpgBuffer = new uint8_t[1024 * 1024];
    int jpgSize = 1024 * 1024;

    int yuvSize = 1920 * 1080 * 3 / 2;
    unsigned char *yuvBuffer = new unsigned char[yuvSize];
    FILE *fp_in = fopen(
            "/storage/emulated/0/Android/data/com.autonavi.plugin/cache/jpeg_turbo_test.yuv",
            "rb+");
    if (fp_in != NULL) {
        fread(yuvBuffer, yuvSize, 1, fp_in);
    } else {
        Log::info("doJpegSoftEncodeTest", "找不到yuv文件");
        return;
    }
    fclose(fp_in);

    jpgUtil->doHardEncodeYuvToJpeg(yuvBuffer, 1920, 1080, 80, jpgBuffer, &jpgSize);

    FILE *fp_out = NULL;
    if ((fp_out = fopen(
            "/storage/emulated/0/Android/data/com.autonavi.plugin/cache/111_hard.jpg",
            "wb+")) == NULL) {
        Log::info("doJpegSoftEncodeTest", "文件创建失败");
    } else {
        fwrite(jpgBuffer, jpgSize, 1, fp_out);
        fclose(fp_out);

        Log::info("doJpegSoftEncodeTest", "文件创建成功");
    }

    delete[] jpgBuffer;
    delete[] yuvBuffer;

}
```