/*
**
** Copyright 2010-2011 The NITDroid Open Source Project
** Copyright 2009, The Android-x86 Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
** Author: Roslyakov Alexey <alexey.roslyakov@newsycat.com>
** Author: Niels Keeman <nielskeeman@gmail.com>
**
*/

#define LOG_TAG "CameraHardware"
#include <utils/Log.h>

#include "CameraHardware.h"
#include <fcntl.h>
#include <sys/mman.h>

#define VIDEO_DEVICE        "/dev/video0"
#define PREVIEW_WIDTH           800
#define PREVIEW_HEIGHT          480
#define PIC_WIDTH               2576
#define PIC_HEIGHT              1936
#define PIXEL_FORMAT        V4L2_PIX_FMT_YUYV

namespace android {

wp<CameraHardwareInterface> CameraHardware::singleton;

CameraHardware::CameraHardware()
                  : mParameters(),
                    mPreviewHeap(0),
                    mRawHeap(0),
                    mCurrentPreviewFrame(0),
                    nQueued(0),
                    nDequeued(0),
                    mNotifyCb(NULL),
                    mDataCb(NULL),
                    mTimestampFn(NULL),
                    mCallbackCookie(NULL)
{
    initDefaultParameters();

    mPreviewRunning = false;
    mExitAutoFocusThread = false;
    mExitPreviewThread = false;

    mPreviewThread = new PreviewThread(this);
    mAutoFocusThread = new AutoFocusThread(this);
}

void CameraHardware::initDefaultParameters()
{
    CameraParameters p;

    p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_SIZES, "800x480");
    p.setPreviewSize(PREVIEW_WIDTH, PREVIEW_HEIGHT);
    p.setPreviewFrameRate(10);
    p.setPreviewFormat(CameraParameters::PIXEL_FORMAT_YUV422I);

    p.set(CameraParameters::KEY_SUPPORTED_PICTURE_SIZES, "2576x1936");
    p.setPictureSize(PIC_WIDTH, PIC_HEIGHT);
    p.setPictureFormat("jpeg");

    if (setParameters(p) != NO_ERROR) {
        LOGE("Failed to set default parameters?!");
    }
}

CameraHardware::~CameraHardware()
{
    singleton.clear();
}

sp<IMemoryHeap> CameraHardware::getPreviewHeap() const
{
    return mPreviewHeap;
}

sp<IMemoryHeap> CameraHardware::getRawHeap() const
{
    return mRawHeap;
}

// ---------------------------------------------------------------------------

void CameraHardware::setCallbacks(notify_callback notify_cb,
                                  data_callback data_cb,
                                  data_callback_timestamp data_cb_timestamp,
                                  void *arg) {
    Mutex::Autolock lock(mLock);
    mNotifyCb = notify_cb;
    mDataCb = data_cb;
    mTimestampFn = data_cb_timestamp;
    mCallbackCookie = arg;
}

void CameraHardware::enableMsgType(int32_t msgType)
{
    Mutex::Autolock lock(mLock);
    mMsgEnabled |= msgType;
}

void CameraHardware::disableMsgType(int32_t msgType)
{
    Mutex::Autolock lock(mLock);
    mMsgEnabled &= ~msgType;
}

bool CameraHardware::msgTypeEnabled(int32_t msgType)
{
    Mutex::Autolock lock(mLock);
    return (mMsgEnabled & msgType);
}


//-------------------------------------------------------------

int CameraHardware::previewThreadWrapper()
{
    LOGI("%s: starting", __func__);
    while (1) {
        mPreviewLock.lock();
        while (!mPreviewRunning) {
            LOGI("%s: calling camera.stopPreview() and waiting", __func__);
            camera.StopStreaming();
            //camera.Close();
            /* signal that we're stopping */
            LOGD("%s: going to wait", __func__);
            mPreviewStoppedCondition.signal();
            mPreviewCondition.wait(mPreviewLock);
            LOGD("%s: return from wait", __func__);
        }
        mPreviewLock.unlock();

        if (mExitPreviewThread) {
            LOGI("%s: exiting", __func__);
            camera.StopStreaming();
            camera.Close();
            return 0;
        }
        previewThread();
    }
}

int CameraHardware::previewThread()
{
    if (mPreviewRunning) {
        static int skipFrame = 0;
        if (skipFrame-- > 0)
            return NO_ERROR;
        // Get preview frame
        //Mutex::Autolock lock(mLock);
        LOGD("entering camera.GrabPreviewFrame");
        camera.GrabPreviewFrame(mPreviewHeap->getBase());
        mPreviewLock.lock();
        if (mPreviewRunning && (mMsgEnabled & CAMERA_MSG_PREVIEW_FRAME)) {
            LOGD("mDataCb called");
            mDataCb(CAMERA_MSG_PREVIEW_FRAME, mPreviewBuffer, mCallbackCookie);
        }
        mPreviewLock.unlock();
    }

    return NO_ERROR;
}

status_t CameraHardware::startPreview()
{
#if 0
    if (mPreviewThread != 0) {
        //already running
        return INVALID_OPERATION;
    }
#endif

    LOGD("CameraHardware::startPreview");
    Mutex::Autolock lock(mLock);
    mPreviewLock.lock();
    if (mPreviewRunning) {
        // already running
        LOGE("%s : preview thread already running", __func__);
        mPreviewLock.unlock();
        return INVALID_OPERATION;
    }

    if (camera.Open(VIDEO_DEVICE) != 0) {
        LOGE("camera.Open failed: %s", strerror(errno));
        //return UNKNOWN_ERROR;
    }

    if ( camera.SetFormat(PREVIEW_WIDTH, PREVIEW_HEIGHT, PIXEL_FORMAT) != 0) {
        LOGE("camera.SetFormat failed: %s", strerror(errno));
        return UNKNOWN_ERROR;
    }

    int previewFrameSize = PREVIEW_WIDTH * PREVIEW_HEIGHT * 2;

    mPreviewHeap = new MemoryHeapBase(previewFrameSize);
    mPreviewBuffer = new MemoryBase(mPreviewHeap, 0, previewFrameSize);

    camera.Init();
    camera.StartStreaming();

    //previewStopped = false;
    mPreviewRunning = true;
    mPreviewCondition.signal();
    mPreviewLock.unlock();
    
    LOGD("Preview started");
    return NO_ERROR;
}

void CameraHardware::stopPreview()
{
    LOGD("%s :", __func__);

    /* request that the preview thread stop. */
    mPreviewLock.lock();
    LOGD("stopPreview: mPreviewLock.locked");
    if (mPreviewRunning) {
        mPreviewRunning = false;
        LOGD("stopPreview: going to signal");
        mPreviewCondition.signal();
        /* wait until preview thread is stopped */
        LOGD("stopPreview: going to wait");
        //mPreviewLock.unlock();
        mPreviewStoppedCondition.wait(mPreviewLock);
        LOGD("preview stopped");
    } else {
        LOGD("%s : preview not running, doing nothing", __func__);
    }
    mPreviewLock.unlock();
}

bool CameraHardware::previewEnabled()
{
    Mutex::Autolock lock(mPreviewLock);
    LOGV("%s : %d", __func__, mPreviewRunning);
    return mPreviewRunning;
}

status_t CameraHardware::startRecording()
{
    return UNKNOWN_ERROR;
}

void CameraHardware::stopRecording()
{
}

bool CameraHardware::recordingEnabled()
{
    return false;
}

void CameraHardware::releaseRecordingFrame(const sp<IMemory>& mem)
{
}

// ---------------------------------------------------------------------------

#if 0
int CameraHardware::beginAutoFocusThread(void *cookie)
{
    CameraHardware *c = (CameraHardware *)cookie;
    return c->autoFocusThread();
}
#endif

int CameraHardware::autoFocusThread()
{
    int count =0;
    int af_status =0 ;

    LOGV("%s : starting", __func__);

    /* block until we're told to start.  we don't want to use
     * a restartable thread and requestExitAndWait() in cancelAutoFocus()
     * because it would cause deadlock between our callbacks and the
     * caller of cancelAutoFocus() which both want to grab the same lock
     * in CameraServices layer.
     */
    mFocusLock.lock();
    /* check early exit request */
    if (mExitAutoFocusThread) {
        mFocusLock.unlock();
        LOGV("%s : exiting on request0", __func__);
        return NO_ERROR;
    }
    mFocusCondition.wait(mFocusLock);
    /* check early exit request */
    if (mExitAutoFocusThread) {
        mFocusLock.unlock();
        LOGV("%s : exiting on request1", __func__);
        return NO_ERROR;
    }
    mFocusLock.unlock();

#if 0
    LOGV("%s : calling setAutoFocus", __func__);
    if (mSecCamera->setAutofocus() < 0) {
        LOGE("ERR(%s):Fail on mSecCamera->setAutofocus()", __func__);
        return UNKNOWN_ERROR;
    }
#endif

    af_status = 0x01; //mSecCamera->getAutoFocusResult();

    if (af_status == 0x01) {
        LOGV("%s : AF Success!!", __func__);
        if (mMsgEnabled & CAMERA_MSG_FOCUS)
            mNotifyCb(CAMERA_MSG_FOCUS, true, 0, mCallbackCookie);
    } else if (af_status == 0x02) {
        LOGV("%s : AF Cancelled !!", __func__);
        if (mMsgEnabled & CAMERA_MSG_FOCUS) {
            /* CAMERA_MSG_FOCUS only takes a bool.  true for
             * finished and false for failure.  cancel is still
             * considered a true result.
             */
            mNotifyCb(CAMERA_MSG_FOCUS, true, 0, mCallbackCookie);
        }
    } else {
        LOGV("%s : AF Fail !!", __func__);
        LOGV("%s : mMsgEnabled = 0x%x", __func__, mMsgEnabled);
        if (mMsgEnabled & CAMERA_MSG_FOCUS)
            mNotifyCb(CAMERA_MSG_FOCUS, false, 0, mCallbackCookie);
    }

    LOGV("%s : exiting with no error", __func__);
    return NO_ERROR;
}

status_t CameraHardware::autoFocus()
{
    LOGV("CameraHardware::autoFocus");
    /* signal autoFocusThread to run once */
    mFocusCondition.signal();
    return NO_ERROR;
}

status_t CameraHardware::cancelAutoFocus()
{
    return NO_ERROR;
}

/*static*/ int CameraHardware::beginPictureThread(void *cookie)
{
    CameraHardware *c = (CameraHardware *)cookie;
    return c->pictureThread();
}

int CameraHardware::pictureThread()
{
    unsigned char *frame;
    int bufferSize;
    int w,h;
    int ret;
    struct v4l2_buffer buffer;
    struct v4l2_format format;
    struct v4l2_buffer cfilledbuffer;
    struct v4l2_requestbuffers creqbuf;
    struct v4l2_capability cap;

    LOGD("CameraHardware::pictureThread");

   if (mMsgEnabled & CAMERA_MSG_SHUTTER)
        mNotifyCb(CAMERA_MSG_SHUTTER, 0, 0, mCallbackCookie);

    mParameters.getPictureSize(&w, &h);
    LOGD("Picture Size: Width = %d \t Height = %d", w, h);

    int width, height;
    mParameters.getPictureSize(&width, &height);
    camera.Open(VIDEO_DEVICE);
    camera.SetFormat(PIC_WIDTH, PIC_HEIGHT, PIXEL_FORMAT);
    camera.Init();
    camera.StartStreaming();

    if (mMsgEnabled & CAMERA_MSG_COMPRESSED_IMAGE) {
        LOGD ("mJpegPictureCallback");
        sp<IMemory> shot;
        {
            //Mutex::Autolock lock(mPreviewLock);
            shot = camera.GrabJpegFrame();
        }
        mDataCb(CAMERA_MSG_COMPRESSED_IMAGE, shot,mCallbackCookie);
    }
    camera.StopStreaming();
    //camera.Uninit();
    //camera.Close();

    return NO_ERROR;
}

status_t CameraHardware::takePicture()
{
    LOGD("CameraHardware::takePicture");
    stopPreview();
    if (createThread(beginPictureThread, this) == false)
        return -1;

    return NO_ERROR;
}

status_t CameraHardware::cancelPicture()
{

    return NO_ERROR;
}

status_t CameraHardware::dump(int fd, const Vector<String16>& args) const
{
    return NO_ERROR;
}

status_t CameraHardware::setParameters(const CameraParameters& params)
{
    LOGD("setParameters, getPreviewFormat=%s, getPictureFormat=%s",
         params.getPreviewFormat(), params.getPictureFormat());
    Mutex::Autolock lock(mLock);

#if 0
    if (strcmp(params.getPreviewFormat(), "yuv422sp") != 0) {
        LOGE("Only yuv422sp preview is supported");
        return -1;
    }
#endif

    if (strcmp(params.getPictureFormat(), "jpeg") != 0) {
        LOGE("Only jpeg still pictures are supported");
        return -1;
    }

    int w, h;
    int framerate;

    params.getPreviewSize(&w, &h);
    framerate = params.getPreviewFrameRate();
    LOGD("PREVIEW SIZE: w=%d h=%d framerate=%d", w, h, framerate);

    params.getPictureSize(&w, &h);

    mParameters = params;

    // Set to fixed sizes
    mParameters.setPreviewSize(PREVIEW_WIDTH, PREVIEW_HEIGHT);

    return NO_ERROR;
}

status_t CameraHardware::sendCommand(int32_t command, int32_t arg1,
                                         int32_t arg2)
{
    return BAD_VALUE;
}

CameraParameters CameraHardware::getParameters() const
{
    CameraParameters params;

    {
    Mutex::Autolock lock(mLock);
        params = mParameters;
    }

    return params;
}

void CameraHardware::release()
{
    /* shut down any threads we have that might be running.  do it here
     * instead of the destructor.  we're guaranteed to be on another thread
     * than the ones below.  if we used the destructor, since the threads
     * have a reference to this object, we could wind up trying to wait
     * for ourself to exit, which is a deadlock.
     */
    if (mPreviewThread != NULL) {
        /* this thread is normally already in it's threadLoop but blocked
         * on the condition variable or running.  signal it so it wakes
         * up and can exit.
         */
        mPreviewThread->requestExit();
        mExitPreviewThread = true;
        mPreviewRunning = true; /* let it run so it can exit */
        mPreviewCondition.signal();
        mPreviewThread->requestExitAndWait();
        mPreviewThread.clear();
    }
    if (mAutoFocusThread != NULL) {
        /* this thread is normally already in it's threadLoop but blocked
         * on the condition variable.  signal it so it wakes up and can exit.
         */
        mFocusLock.lock();
        mAutoFocusThread->requestExit();
        mExitAutoFocusThread = true;
        mFocusCondition.signal();
        mFocusLock.unlock();
        mAutoFocusThread->requestExitAndWait();
        mAutoFocusThread.clear();
    }
    close(camera_device);
}

sp<CameraHardwareInterface> CameraHardware::createInstance()
{
    if (singleton != 0) {
        sp<CameraHardwareInterface> hardware = singleton.promote();
        if (hardware != 0) {
            return hardware;
        }
    }
    sp<CameraHardwareInterface> hardware(new CameraHardware());
    singleton = hardware;
    return hardware;
}

static CameraInfo sCameraInfo[] = {
    {
        CAMERA_FACING_BACK,
        0,  /* orientation */
    }
};

extern "C" int HAL_getNumberOfCameras()
{
    return sizeof(sCameraInfo) / sizeof(sCameraInfo[0]);
}

extern "C" void HAL_getCameraInfo(int cameraId, struct CameraInfo* cameraInfo)
{
    memcpy(cameraInfo, &sCameraInfo[cameraId], sizeof(CameraInfo));
}

extern "C" sp<CameraHardwareInterface> HAL_openCameraHardware(int cameraId)
{
    return CameraHardware::createInstance();
}

}; // namespace android
