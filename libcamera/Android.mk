LOCAL_PATH:= $(call my-dir)

#ifeq ($strip(BOARD_USES_N900_CAMERA_HAL),true)
include $(CLEAR_VARS)

LOCAL_MODULE:= libcamera
LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES:=               \
    CameraHardware.cpp          \
    V4L2Camera.cpp              \
	converter.cpp

LOCAL_CFLAGS += -Iexternal/jpeg

LOCAL_SHARED_LIBRARIES:= \
    libui \
    libjpeg \
    libutils \
    libbinder \
    libcutils \
    libcamera_client

include $(BUILD_SHARED_LIBRARY)
#endif