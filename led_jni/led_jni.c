#include <jni.h>  /* /usr/lib/jvm/java-1.7.0-openjdk-amd64/include/ */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <android/log.h>
#include <unistd.h>

static jint fd = -1;

JNIEXPORT jint JNICALL ledOpen(JNIEnv *env, jobject cls)
{
	__android_log_print(ANDROID_LOG_DEBUG, "Hardware", "open");
	fd  = open("/dev/leds",O_RDONLY);
	if(fd < 0){
		__android_log_print(ANDROID_LOG_DEBUG, "Hardware", "open error fd = %d",fd);
		return -1;
	}
	return fd;
}

JNIEXPORT jint JNICALL ledClose(JNIEnv *env, jobject cls)
{
	__android_log_print(ANDROID_LOG_DEBUG, "Hardware", "ledClose");
	close(fd);
}

JNIEXPORT jint JNICALL ledCrtl(JNIEnv *env, jobject cls, jint cmd, jint args)
{
	__android_log_print(ANDROID_LOG_DEBUG, "Hardware", "ledCrtl cmd = %d,args = %d",cmd,args);//打印android log信息 
	ioctl(fd,cmd,args);
}

static const JNINativeMethod methods[] = {
	{"ledOpen"    , "()I"  , (void *)ledOpen    },
	{"ledClose" , "()I"  , (void *)ledClose },
	{"ledCrtl"   , "(II)I", (void *)ledCrtl   },
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;
	jclass cls;
    if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_4)) {
		return JNI_ERR; /* JNI version not supported */
	}
	cls = (*env)->FindClass(env, "com/led_jni/com/hardware/MyHardware");
	if (cls == NULL) {
		return JNI_ERR;
	}
	/* 2. map java hello <-->c c_hello */
	if ((*env)->RegisterNatives(env, cls, methods, sizeof(methods)/sizeof(methods[0])) < 0)
		return JNI_ERR;

    return JNI_VERSION_1_4;
}

/*
arm-linux-gcc -fPIC -shared led_jni.c -o libledjni.so -I /usr/lib/jvm/java-1.7.0-openjdk-amd64/include/  -nostdlib /work/android-5.0.2/prebuilts/ndk/9/platforms/android-19/arch-arm/usr/lib/libc.so -I /work/android-5.0.2/prebuilts/ndk/9/platforms/android-19/arch-arm/usr/include /work/android-5.0.2/prebuilts/ndk/9/platforms/android-19/arch-arm/usr/lib/liblog.so

*/
























