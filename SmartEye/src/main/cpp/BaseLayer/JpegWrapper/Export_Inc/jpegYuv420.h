#pragma once
int JpegHwEngine_yuv420_compress(int width, int height, uint8_t *yuv,
                                 uint8_t jpegQual, uint8_t *jpeg_addr, int *jpeg_size);