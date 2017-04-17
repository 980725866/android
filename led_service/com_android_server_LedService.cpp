
#define LOG_TAG "LedService"

#include "jni.h"
#include "JNIHelp.h"
#include <utils/misc.h>
#include <utils/Log.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

namespace android
{
int fd = -1;

static int ledOpen(JNIEnv *env, jobject clazz)
{
	fd = open("/dev/leds",O_RDWR);
    if(fd < 0){
		ALOGI("ledOpen error\n");
	}
	ALOGI("ledOpen yes fd = %d\n",fd);
	return fd;
}

static int ledCtrl(JNIEnv *env, jobject clazz, jint cmd,jint args)
{
    ALOGI("ledCtrl cmd = %d,args = %d\n",cmd,args);
    ioctl(fd,cmd,args);
	return 0;
}

static void ledClose(JNIEnv *env, jobject clazz)
{
    ALOGI("ledClose\n");
    close(fd);
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
