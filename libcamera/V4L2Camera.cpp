/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      Author: Roslyakov Alexey <alexey.roslyakov@newsycat.com>
 *      Author: Niels Keeman <nielskeeman@gmail.com>
 *
 */

#define LOG_TAG "V4L2Camera"
#include <utils/Log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>

#include <linux/videodev.h>

#include "converter.h"
#include "V4L2Camera.h"

extern "C" { /* Android jpeglib.h missed extern "C" */
#include <jpeglib.h>
}

namespace android {

V4L2Camera::V4L2Camera ()
    : nQueued(0), nDequeued(0)
{
    LOGD("%s", __FUNCTION__);
    videoIn = (struct vdIn *) calloc (1, sizeof (struct vdIn));
}

V4L2Camera::~V4L2Camera()
{
    LOGD("%s", __FUNCTION__);
    free(videoIn);
}

int V4L2Camera::Open(const char *device)
{
    int ret;
    LOGD("V4L2Camera::Open(%s)", device);

    if ((fd = open(device, O_RDWR)) == -1) {
        LOGE("ERROR opening V4L interface: %s", strerror(errno));
        return -1;
    }

    ret = ioctl (fd, VIDIOC_QUERYCAP, &videoIn->cap);
    if (ret < 0) {
        LOGE("Error opening device: unable to query device: %s", strerror(errno));
        return -1;
    }

    if ((videoIn->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
        LOGE("Error opening device: video capture not supported.");
        return -1;
    }

    if (!(videoIn->cap.capabilities & V4L2_CAP_STREAMING)) {
        LOGE("Capture device does not support streaming i/o");
        return -1;
    }
    return 0;
}

int V4L2Camera::SetFormat(int width, int height, int pixelformat)
{
    LOGD("V4L2Camera::SetFormat(%d, %d, %d)", width, height, pixelformat);

    videoIn->width = width;
    videoIn->height = height;
    videoIn->framesizeIn = (width * height << 1);
    videoIn->formatIn = pixelformat;

    videoIn->format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    videoIn->format.fmt.pix.width = width;
    videoIn->format.fmt.pix.height = height;
    videoIn->format.fmt.pix.pixelformat = pixelformat;

    int ret = ioctl(fd, VIDIOC_S_FMT, &videoIn->format);
    if (ret < 0) {
        LOGE("SetFormat: VIDIOC_S_FMT Failed: %s", strerror(errno));
        return ret;
    }

    return 0;
}

void V4L2Camera::Close ()
{
    close(fd);
    LOGD("%s", "V4L2Camera::Close");
}

int V4L2Camera::Init()
{
    LOGD("%s", __FUNCTION__);
    int ret;

    /* Check if camera can handle NB_BUFFER buffers */
    videoIn->rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    videoIn->rb.memory = V4L2_MEMORY_MMAP;
    videoIn->rb.count = NB_BUFFER;

    ret = ioctl(fd, VIDIOC_REQBUFS, &videoIn->rb);
    if (ret < 0) {
        LOGE("Init: VIDIOC_REQBUFS failed: %s", strerror(errno));
        return ret;
    }

    for (int i = 0; i < NB_BUFFER; i++) {

        memset (&videoIn->buf, 0, sizeof (struct v4l2_buffer));

        videoIn->buf.index = i;
        videoIn->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        videoIn->buf.memory = V4L2_MEMORY_MMAP;

        ret = ioctl (fd, VIDIOC_QUERYBUF, &videoIn->buf);
        if (ret < 0) {
            LOGE("Init: Unable to query buffer (%s)", strerror(errno));
            return ret;
        }

        videoIn->mem[i] = mmap (0,
               videoIn->buf.length,
               PROT_READ | PROT_WRITE,
               MAP_SHARED,
               fd,
               videoIn->buf.m.offset);

        if (videoIn->mem[i] == MAP_FAILED) {
            LOGE("Init: Unable to map buffer (%s)", strerror(errno));
            return -1;
        }

        ret = ioctl(fd, VIDIOC_QBUF, &videoIn->buf);
        if (ret < 0) {
            LOGE("Init: VIDIOC_QBUF Failed");
            return -1;
        }

        nQueued++;
    }

    return 0;
}

int V4L2Camera::setAutofocus(void)
{
    LOGV("%s :", __func__);

    if (fd <= 0) {
        LOGE("ERR(%s):Camera was closed\n", __func__);
        return -1;
    }

#if 0
    if (fimc_v4l2_s_ctrl(fd, V4L2_CID_CAMERA_SET_AUTO_FOCUS, AUTO_FOCUS_ON) < 0) {
            LOGE("ERR(%s):Fail on V4L2_CID_CAMERA_SET_AUTO_FOCUS", __func__);
        return -1;
    }
#endif

    return 0;
}

int V4L2Camera::getAutoFocusResult(void)
{
    int af_result;

    //af_result = fimc_v4l2_g_ctrl(fd, V4L2_CID_CAMERA_AUTO_FOCUS_RESULT);

    LOGV("%s : returning %d", __func__, af_result);

    return af_result;
}

int V4L2Camera::cancelAutofocus(void)
{
    LOGV("%s :", __func__);

    if (fd <= 0) {
        LOGE("ERR(%s):Camera was closed\n", __func__);
        return -1;
    }

//    if (fimc_v4l2_s_ctrl(fd, V4L2_CID_CAMERA_SET_AUTO_FOCUS, AUTO_FOCUS_OFF) < 0) {
//        LOGE("ERR(%s):Fail on V4L2_CID_CAMERA_SET_AUTO_FOCUS", __func__);
//        return -1;
//    }

    return 0;
}

void V4L2Camera::Uninit ()
{
    LOGD("%s", __FUNCTION__);
    int ret;

    videoIn->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    videoIn->buf.memory = V4L2_MEMORY_MMAP;

    /* Dequeue everything */
    int DQcount = nQueued - nDequeued;

    for (int i = 0; i < DQcount-1; i++) {
        ret = ioctl(fd, VIDIOC_DQBUF, &videoIn->buf);
        if (ret < 0)
            LOGE("Uninit: VIDIOC_DQBUF Failed");
    }
    nQueued = 0;
    nDequeued = 0;

    /* Unmap buffers */
    for (int i = 0; i < NB_BUFFER; i++)
        if (munmap(videoIn->mem[i], videoIn->buf.length) < 0)
            LOGE("Uninit: Unmap failed");
}

int V4L2Camera::StartStreaming ()
{
    LOGD("V4L2Camera::StartStreaming");
    enum v4l2_buf_type type;
    int ret;

    if (!videoIn->isStreaming) {
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        ret = ioctl (fd, VIDIOC_STREAMON, &type);
        if (ret < 0) {
            LOGE("StartStreaming: Unable to start capture: %s", strerror(errno));
            return ret;
        }

        videoIn->isStreaming = true;
    }

    return 0;
}

int V4L2Camera::StopStreaming ()
{
    LOGD("%s", __FUNCTION__);
    enum v4l2_buf_type type;
    int ret;

    if (videoIn->isStreaming) {
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        ret = ioctl (fd, VIDIOC_STREAMOFF, &type);
        if (ret < 0) {
            LOGE("StopStreaming: Unable to stop capture: %s", strerror(errno));
            return ret;
        }

        videoIn->isStreaming = false;
    }

    return 0;
}

void V4L2Camera::GrabPreviewFrame (void *previewBuffer)
{
    LOGD("%s %p", __FUNCTION__, previewBuffer);
    unsigned char *tmpBuffer;
    int ret;

    videoIn->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    videoIn->buf.memory = V4L2_MEMORY_MMAP;

    /* DQ */
    ret = ioctl(fd, VIDIOC_DQBUF, &videoIn->buf);
    if (ret < 0) {
        static int reported = 0;
        if (!reported) {
            LOGE("GrabPreviewFrame: VIDIOC_DQBUF Failed: %s", strerror(errno));
            reported = 1;
        }
        return;
    }
    nDequeued++;

    LOGD("GrabPreviewFrame: nDequeued: %d, idx=%d, bytesused=%d", nDequeued,
         (int)videoIn->mem[videoIn->buf.index], (int) videoIn->buf.bytesused);
    
    //memcpy (previewBuffer, videoIn->mem[videoIn->buf.index], (size_t) videoIn->buf.bytesused);
    convert((uint8_t *)videoIn->mem[videoIn->buf.index], (uint8_t *)previewBuffer, videoIn->width, videoIn->height);
    //yuyv422_to_yuv420sp((unsigned char *)previewBuffer, (unsigned char*)videoIn->mem[videoIn->buf.index], videoIn->width, videoIn->height);

    ret = ioctl(fd, VIDIOC_QBUF, &videoIn->buf);
    if (ret < 0) {
        LOGE("GrabPreviewFrame: VIDIOC_QBUF Failed: %s", strerror(errno));
        return;
    }
    nQueued++;
}

sp<IMemory> V4L2Camera::GrabRawFrame ()
{
    LOGD("%s", __FUNCTION__);
    sp<MemoryHeapBase> memHeap = new MemoryHeapBase(videoIn->width * videoIn->height * 2);
    sp<MemoryBase> memBase = new MemoryBase(memHeap, 0, videoIn->width * videoIn->height * 2);

    // Not yet implemented, do I have to?

    return memBase;
}

sp<IMemory> V4L2Camera::GrabJpegFrame ()
{
    LOGD("%s", __FUNCTION__);
    FILE *output;
    FILE *input;
    size_t fileSize;
    int ret;

    videoIn->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    videoIn->buf.memory = V4L2_MEMORY_MMAP;

    /* Dequeue buffer */
    ret = ioctl(fd, VIDIOC_DQBUF, &videoIn->buf);
    if (ret < 0) {
        LOGE("GrabJpegFrame: VIDIOC_DQBUF Failed");
        return NULL;
    }
    nDequeued++;

    LOGI("GrabJpegFrame: Generated a frame from capture device");

    output = fopen("/data/data/tmp.jpg", "wb");

    if (output == NULL) {
        LOGE("GrabJpegFrame: Ouput file == NULL");
        return NULL;
    }

    fileSize = saveYUYVtoJPEG((unsigned char *)videoIn->mem[videoIn->buf.index], videoIn->width, videoIn->height, output, 85);

    fclose(output);

    input = fopen("/data/data/tmp.jpg", "rb");

    if (input == NULL)
        LOGE("GrabJpegFrame: Input file == NULL");
    else {
        sp<MemoryHeapBase> mjpegPictureHeap = new MemoryHeapBase(fileSize);
        sp<MemoryBase> jpegmemBase = new MemoryBase(mjpegPictureHeap, 0, fileSize);

        fread((uint8_t *)mjpegPictureHeap->base(), 1, fileSize, input);
        fclose(input);

        return jpegmemBase;
    }

    return NULL;
}

int V4L2Camera::saveYUYVtoJPEG (unsigned char *inputBuffer, int width, int height, FILE *file, int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];
    unsigned char *line_buffer, *yuyv;
    int z;
    int fileSize;

    line_buffer = (unsigned char *) calloc (width * 3, 1);
    yuyv = inputBuffer;

    cinfo.err = jpeg_std_error (&jerr);
    jpeg_create_compress (&cinfo);
    jpeg_stdio_dest (&cinfo, file);

    LOGI("JPEG PICTURE WIDTH AND HEIGHT: %dx%d", width, height);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults (&cinfo);
    jpeg_set_quality (&cinfo, quality, TRUE);

    jpeg_start_compress (&cinfo, TRUE);

    z = 0;
    while (cinfo.next_scanline < cinfo.image_height) {
        int x;
        unsigned char *ptr = line_buffer;

        for (x = 0; x < width; x++) {
            int r, g, b;
            int y, u, v;

            if (!z)
                y = yuyv[0] << 8;
            else
                y = yuyv[2] << 8;

            u = yuyv[1] - 128;
            v = yuyv[3] - 128;

            r = (y + (359 * v)) >> 8;
            g = (y - (88 * u) - (183 * v)) >> 8;
            b = (y + (454 * u)) >> 8;

            *(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
            *(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
            *(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

            if (z++) {
                z = 0;
                yuyv += 4;
            }
        }

        row_pointer[0] = line_buffer;
        jpeg_write_scanlines (&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress (&cinfo);
    fileSize = ftell(file);
    jpeg_destroy_compress (&cinfo);

    free (line_buffer);

    return fileSize;
}

#if 1
void V4L2Camera::convert(unsigned char *buf, unsigned char *rgb, int width, int height)
{
    int x,y,z=0;
    int blocks;

    blocks = (width * height) * 2;

    for (y = 0; y < blocks; y+=4) {
        unsigned char Y1, Y2, U, V;

        Y1 = buf[y + 0];
        U = buf[y + 1];
        Y2 = buf[y + 2];
        V = buf[y + 3];

        yuv_to_rgb16(Y1, U, V, &rgb[y]);
        yuv_to_rgb16(Y2, U, V, &rgb[y + 2]);
    }

}

void V4L2Camera::yuv_to_rgb16(unsigned char y, unsigned char u, unsigned char v, unsigned char *rgb)
{
    int r,g,b;
    int z;
    int rgb16;

    z = 0;

    r = 1.164 * (y - 16) + 1.596 * (v - 128);
    g = 1.164 * (y - 16) - 0.813 * (v - 128) - 0.391 * (u -128);
    b = 1.164 * (y - 16) + 2.018 * (u - 128);

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    rgb16 = (int)(((r >> 3)<<11) | ((g >> 2) << 5)| ((b >> 3) << 0));

    *rgb = (unsigned char)(rgb16 & 0xFF);
    rgb++;
    *rgb = (unsigned char)((rgb16 & 0xFF00) >> 8);

}
#endif

}; // namespace android
