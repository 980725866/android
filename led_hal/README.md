V2:
(3) JNI: 重新上传
frameworks/base/services/core/jni/com_android_server_LedService.cpp

(4) HAL: led_hal.h
         led_hal.c 
把新文件上传到服务器, 所在目录:
hardware/libhardware/include/hardware/led_hal.h
hardware/libhardware/modules/led/led_hal.c
hardware/libhardware/modules/led/Android.mk

Android.mk内容如下:
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := led.default
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_C_INCLUDES := hardware/libhardware
LOCAL_SRC_FILES := led_hal.c
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := eng

include $(BUILD_SHARED_LIBRARY)


编译：
$ mmm frameworks/base/services
$ mmm hardware/libhardware/modules/led
$ make snod
$ ./gen-img.sh



