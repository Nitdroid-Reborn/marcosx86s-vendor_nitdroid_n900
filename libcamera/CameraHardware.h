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

#ifndef ANDROID_HARDWARE_CAMERA_HARDWARE_H
#define ANDROID_HARDWARE_CAMERA_HARDWARE_H

#include <utils/threads.h>
#include <camera/CameraHardwareInterface.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <utils/threads.h>

#include <sys/ioctl.h>
#include "V4L2Camera.h"

namespace android {

class CameraHardware : public CameraHardwareInterface {
public:
    virtual sp<IMemoryHeap> getPreviewHeap() const;
    virtual sp<IMemoryHeap> getRawHeap() const;

    virtual status_t    startPreview();
    virtual void        setCallbacks(notify_callback notify_cb,
                                     data_callback data_cb,
                                     data_callback_timestamp data_cb_timestamp,
                                     void* arg);
    /**
     * Enable a message, or set of messages.
     */
    virtual void        enableMsgType(int32_t msgType);

    /**
     * Disable a message, or a set of messages.
     */
    virtual void        disableMsgType(int32_t msgType);

    /**
     * Query whether a message, or a set of messages, is enabled.
     * Note that this is operates as an AND, if any of the messages
     * queried are off, this will return false.
     */
    virtual bool        msgTypeEnabled(int32_t msgType);

    virtual void        stopPreview();
    virtual bool        previewEnabled();

    virtual status_t    startRecording();
    virtual void        stopRecording();
    virtual bool        recordingEnabled();
    virtual void        releaseRecordingFrame(const sp<IMemory>& mem);

    virtual status_t    autoFocus();
    virtual status_t    cancelAutoFocus();
    virtual status_t    takePicture();
    virtual status_t    cancelPicture();
    virtual status_t    dump(int fd, const Vector<String16>& args) const;
    virtual status_t    setParameters(const CameraParameters& params);
    virtual CameraParameters  getParameters() const;
    virtual void release();
    virtual status_t sendCommand(int32_t cmd, int32_t arg1, int32_t arg2);

    static sp<CameraHardwareInterface> createInstance();

private:
                        CameraHardware();
    virtual             ~CameraHardware();

    static wp<CameraHardwareInterface> singleton;

    static const int kBufferCount = 4;

    class PreviewThread : public Thread {
        CameraHardware* mHardware;
    public:
        PreviewThread(CameraHardware* hw)
            : Thread(false), mHardware(hw) { }
        virtual void onFirstRef() {
            run("CameraPreviewThread", PRIORITY_URGENT_DISPLAY);
        }
        virtual bool threadLoop() {
            mHardware->previewThreadWrapper();
            return false;
        }
    };

    class AutoFocusThread : public Thread {
        CameraHardware *mHardware;
    public:
        AutoFocusThread(CameraHardware *hw): Thread(false), mHardware(hw) { }
        virtual void onFirstRef() {
            run("CameraAutoFocusThread", PRIORITY_DEFAULT);
        }
        virtual bool threadLoop() {
            mHardware->autoFocusThread();
            return true;
        }
    };

    void initDefaultParameters();
    bool initHeapLocked();

    int previewThread();
    int previewThreadWrapper();

   /* used by auto focus thread to block until it's told to run */
    mutable Mutex       mFocusLock;
    mutable Condition   mFocusCondition;
    bool        mExitAutoFocusThread;
    sp<AutoFocusThread> mAutoFocusThread;
    //static int beginAutoFocusThread(void *cookie);
    int autoFocusThread();

    static int beginPictureThread(void *cookie);
    int pictureThread();

    mutable Mutex           mLock;

    CameraParameters        mParameters;

    sp<MemoryHeapBase>      mPreviewHeap;
    sp<MemoryBase>          mPreviewBuffer;

    sp<MemoryHeapBase>      mRawHeap;

    /* used by preview thread to block until it's told to run */
    mutable Mutex           mPreviewLock;
    mutable Condition       mPreviewCondition;
    mutable Condition       mPreviewStoppedCondition;
    bool                    mPreviewRunning;
    bool                    mExitPreviewThread;

    // protected by mLock
    sp<PreviewThread>       mPreviewThread;

    // only used from PreviewThread
    int                     mCurrentPreviewFrame;

    void *                  framebuffer;
    //bool                    previewStopped;
    int                     camera_device;
    void*                   mem[4];
    int                     nQueued;
    int                     nDequeued;
    V4L2Camera              camera;
    notify_callback         mNotifyCb;
    data_callback           mDataCb;
    data_callback_timestamp mTimestampFn;
    void*                   mCallbackCookie;
    int32_t                 mMsgEnabled;

};

}; // namespace android

#endif
