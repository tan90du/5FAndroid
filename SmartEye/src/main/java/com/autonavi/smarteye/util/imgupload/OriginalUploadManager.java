package com.autonavi.smarteye.util.imgupload;

import android.os.Build;
import android.util.Log;

import com.autonavi.smarteye.config.Config;
import com.autonavi.smarteye.service.http.HttpClient;
import com.squareup.okhttp.MediaType;
import com.squareup.okhttp.MultipartBuilder;
import com.squareup.okhttp.OkHttpClient;
import com.squareup.okhttp.Request;
import com.squareup.okhttp.RequestBody;
import com.squareup.okhttp.Response;

import org.apache.commons.codec.digest.DigestUtils;
import org.apache.commons.io.FilenameUtils;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;


/**
 * 上传原始图片到Saas平台
 */
public class OriginalUploadManager {

    private final static String TAG = "OriginalImageUpload";
    // 被监控文件夹
    private static final String folderPath = "/storage/ext4_sdcard/Android/data/img/original";
    private static String originalImageUploadUrl = Config.ip + "/admin-api/pcmanager/disease-pending/uploadDiseasePending";
//    private static String originalImageUploadUrl = "http://192.168.2.16:48080/admin-api/pcmanager/disease-pending/uploadDiseasePending";
    private final File monitorFolder;
    // 记录文件的上次状态：包含文件长度、最后修改时间和记录该状态的时间戳
    private final Map<String, FileState> fileStateMap = new ConcurrentHashMap<>();
    // 定时任务调度器
    private final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();
    // 变化检测阀值（毫秒）
    private static final long INACTIVITY_THRESHOLD_MS = 2000; // 2秒

    public OriginalUploadManager() {
        this.monitorFolder = new File(folderPath);
        if (!monitorFolder.exists() || !monitorFolder.isDirectory()) {
            throw new IllegalArgumentException("指定路径不存在或不是文件夹: " + folderPath);
        }
        startMonitoring();
    }

    // 启动监控任务：每隔一定时间扫描文件夹内 zip 文件
    public void startMonitoring() {
        // 初始延迟2秒，每隔2秒执行一次扫描
        scheduler.scheduleWithFixedDelay(this::scanAndProcessFiles, 5, 5, TimeUnit.SECONDS);
    }

    // 关闭监控任务
    public void stopMonitoring() {
        scheduler.shutdown();
    }

    // 扫描文件夹，对所有 zip 文件进行检测并处理
    private void scanAndProcessFiles() {
        // 获取所有zip文件
        File[] files = monitorFolder.listFiles((dir, name) -> name.toLowerCase().endsWith(".zip"));
        if (files == null) {
            return;
        }
        // 遍历当前文件夹所有zip文件
        for (File file : files) {
            String path = file.getAbsolutePath();
            long currentLength = file.length();
            long currentModified = file.lastModified();
            long now = System.currentTimeMillis();

            // 检查状态记录
            FileState prevState = fileStateMap.get(path);
            if (prevState == null) {
                // 如果是新文件，记录当前状态
                fileStateMap.put(path, new FileState(currentLength, currentModified, now));
            } else {
                // 如果文件发生过变化，则更新状态中的文件信息和时间戳
                if (currentLength != prevState.length || currentModified != prevState.lastModified) {
                    fileStateMap.put(path, new FileState(currentLength, currentModified, now));
                } else {
                    // 文件未发生变化，检查是否超过设定不活跃阀值
                    if (now - prevState.checkTime >= INACTIVITY_THRESHOLD_MS) {
                        // 上传并删除文件
                        uploadAndDeleteFile(file);
                        // 处理完成后，将状态移除
                        fileStateMap.remove(path);
                    }
                }
            }
        }

        // 清理已经被删除的文件，从状态中移除
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            fileStateMap.keySet().removeIf(path -> {
                File f = new File(path);
                return !f.exists();
            });
        }
    }

    // 上传完成后删除文件（具体的上传逻辑留空）
    private void uploadAndDeleteFile(File file) {
        Log.i(TAG, "开始上传文件: " + file.getAbsolutePath());
        boolean success = uploadZip(file, originalImageUploadUrl);
        if (success) {
            Log.i(TAG, "上传成功，删除文件: " + file.getAbsolutePath());
            while (!file.delete()) {
                Log.i(TAG, "删除文件失败, 尝试重删 " + file.getAbsolutePath());
            }
        }
    }

    // 内部类，用于记录每个zip文件的状态信息
    private static class FileState {
        final long length;
        final long lastModified;
        final long checkTime; // 记录上一次检测时的时间

        public FileState(long length, long lastModified, long checkTime) {
            this.length = length;
            this.lastModified = lastModified;
            this.checkTime = checkTime;
        }
    }


    /**
     * 上传 ZIP 文件，支持断点续传
     */
    public static boolean uploadZip(File zipFile, String uploadUrl) {
        String fileMd5 = calcFileMd5(zipFile);
        String fileName = zipFile.getName();

        long fileSize = zipFile.length();
        int totalChunks = (int) Math.ceil((double) fileSize / (2 * 1024 * 1024));

        Log.i(TAG, "开始上传文件，文件名：" + fileName + "，MD5：" + fileMd5 + "，共分片：" + totalChunks);

        // 定义每个分片最多的重试次数
        int maxRetries = 5;

        try (BufferedInputStream bis = new BufferedInputStream(new FileInputStream(zipFile))) {
            byte[] buffer = new byte[2 * 1024 * 1024];

            for (int chunkIndex = 0; chunkIndex < totalChunks; chunkIndex++) {
                int bytesRead = bis.read(buffer);
                if (bytesRead <= 0) break;

                // 将该分片保存为临时文件
                File tempChunk = File.createTempFile("chunk_" + chunkIndex + "_", "." + FilenameUtils.getExtension(fileName), Config.getContext().getCacheDir());
                try (FileOutputStream fos = new FileOutputStream(tempChunk)) {
                    fos.write(buffer, 0, bytesRead);
                }

                // 使用重试逻辑上传该分片
                boolean success = false;
                int attempt = 0;
                while (attempt < maxRetries && !success) {
                    attempt++;
                    Log.i(TAG, "上传分片 " + chunkIndex + "/" + totalChunks + " 尝试第 " + attempt + " 次");
                    success = uploadChunk(tempChunk, fileName, fileMd5, chunkIndex, totalChunks, uploadUrl);
                    if (!success) {
                        Log.i(TAG, "分片 "  + chunkIndex + "/" + totalChunks + " 上传失败，重试中...");
                    }
                }

                if (!success) {
                    // 存在某个分片多次重复上传失败,终止当前ZIP上传
                    Log.i(TAG, "分片 " + chunkIndex + "/" + totalChunks + " 重试 " + maxRetries + " 次后仍然上传失败，终止上传。");
                    return false;
                } else {
                    Log.i(TAG, "分片 " + chunkIndex + "/" + totalChunks + " 上传成功");
                }

                // 删除临时分片
                tempChunk.delete();
            }
            return true;
        } catch (Exception e) {
            Log.e(TAG, "zip上传出错 " + e);
        }
        return false;
    }


    /**
     * 上传分片
     *
     * @param chunkFile
     * @param fileName
     * @param fileMd5
     * @param chunkIndex
     * @param totalChunks
     * @param uploadUrl
     * @return
     */
    private static boolean uploadChunk(File chunkFile, String fileName, String fileMd5,
                                       int chunkIndex, int totalChunks, String uploadUrl) {
        MediaType mediaType = MediaType.parse("application/octet-stream");
        RequestBody fileBody = RequestBody.create(mediaType, chunkFile);

        // 使用 MultipartBuilder 构建 multipart/form-data 请求体
        RequestBody requestBody = new MultipartBuilder()
                .type(MultipartBuilder.FORM)
                .addFormDataPart("direction", "暂未上传该参数")
                .addFormDataPart("equipmentMac", Config.deviceId)
                .addFormDataPart("file", chunkFile.getName(), fileBody)
                .addFormDataPart("fileMd5", fileMd5)
                .addFormDataPart("chunkIndex", String.valueOf(chunkIndex))
                .addFormDataPart("totalChunks", String.valueOf(totalChunks))
                .addFormDataPart("fileName", fileName)
                .build();

        Request request = new Request.Builder()
                .url(uploadUrl)
                .post(requestBody)
                .build();

        try {
            Response response = HttpClient.getClient().newCall(request).execute();
            return response.isSuccessful();
        } catch (Exception e) {
            Log.e(TAG, "上传分片出错 " + e);
            return false;
        }
    }

    /**
     * 计算文件 MD5
     *
     * @param file
     * @return
     */
    private static String calcFileMd5(File file) {
        try (InputStream is = new FileInputStream(file)) {
            return DigestUtils.md5Hex(is);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
