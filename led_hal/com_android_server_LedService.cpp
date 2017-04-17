
#define LOG_TAG "LedServiceHal"

#include "jni.h"
#include "JNIHelp.h"
#include <utils/misc.h>
#include <utils/Log.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <hardware/led_hal.h>

namespace android
{
struct led_device_t* led_dev;

static int ledOpen(JNIEnv *env, jobject clazz)
{
	int err;
    struct hw_module_t* module;
	struct hw_device_t* device;
    err = hw_get_module(LED_HARDWARE_MODULE_ID, (struct hw_module_t const**)&module);
    if(err == 0){
		err = module->methods->open(module, NULL, &device);
		if (err == 0) {
			led_dev = (struct led_device_t*)device;
			return led_dev->ledOpen(led_dev);
		}
		return -1;
	}
	ALOGI("native ledOpen error");
	return -1;
}

static int ledCtrl(JNIEnv *env, jobject clazz, jint cmd,jint args)
{
    ALOGI("native ledCtrl cmd = %d,args = %d",cmd,args);
    led_dev->ledCtrl(led_dev,cmd,args);
	return 0;
}

static void ledClose(JNIEnv *env, jobject clazz)
{
	led_dev->ledClose(led_dev);
    ALOGI("native ledClose\n");
}

static JNINativeMethod method_table[] = {
    { "native_ledOpen", "()I", (void*)ledOpen },
    { "native_ledCtrl", "(II)I", (void*)ledCtrl },
    { "native_ledClose", "()V", (void*)ledClose }
};

int register_android_server_LedService(JNIEnv *env)
{
    return jniRegisterNativeMethods(env, "com/android/server/LedService",
            method_table, NELEM(method_table));
}

};
