参考 vibrator震动器

ILedService.aidl  						接口文件 frameworks/base/core/java/android/os/
ILedService.java  						由ILedService.aidl生成  out/target/common/obj/JAVA_LIBRARIES/framework_intermediates/src/core/java/android/os/

SystemServer.java 						frameworks/base/services/java/com/android/server
LedService.java 						frameworks/base/services/core/java/com/android/server
com_android_server_LedService.cpp		frameworks/base/services/core/jni
onload.cpp 								frameworks/base/services/core/jni

(1) AIDL
1. 把 ILedService.aidl 放入 frameworks/base/core/java/android/os
2. 修改 frameworks/base/Android.mk  添加一行
         core/java/android/os/IVibratorService.aidl \
+        core/java/android/os/ILedService.aidl \

3. mmm frameworks/base

4. 它会生成: ./out/target/common/obj/JAVA_LIBRARIES/framework_intermediates/src/core/java/android/os/ILedService.java

(2) Server : LedService.java 
             SystemServer.java

把新文件上传到服务器, 所在目录:
frameworks/base/services/java/com/android/server/SystemServer.java
frameworks/base/services/core/java/com/android/server/LedService.java

不需要修改 frameworks/base/services/core/Android.mk
它的内容里已经把该目录下所有JAVA文件自动包含进去了：
LOCAL_SRC_FILES += \
    $(call all-java-files-under,java)



(3) JNI : com_android_server_LedService.cpp
          onload.cpp

把新文件上传到服务器, 所在目录:
frameworks/base/services/core/jni/onload.cpp
frameworks/base/services/core/jni/com_android_server_LedService.cpp

修改 frameworks/base/services/core/jni/Android.mk :
  $(LOCAL_REL_DIR)/com_android_server_VibratorService.cpp \
+ $(LOCAL_REL_DIR)/com_android_server_LedService.cpp \


编译：
$ mmm frameworks/base/services
$ make snod  //重新打包镜像文件
$ ./gen-img.sh //生成system.img脚本



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

linux kernel command 设置
console=ttySAC0,115200n8 androidboot.console=ttySAC0 ctp=2 skipcali=y vmalloc=384m ethmac=1C:6F:65:34:51:7E androidboot.selinux=permissive

Androidstudio 的设置
1. import jar
2. 包含out/target/common/obj/JAVA_LIBRARIES/framework_intermediates/classes.jar
3. 修改build.grade(app)
defaultConfig {
        applicationId "example.com.led2"
        minSdkVersion 21
        targetSdkVersion 22
        versionCode 1
        versionName "1.0"
        // enabling multidex support
        multiDexEnabled true
}
dexOptions {
	javaMaxHeapSize "4g"
}
dependencies {
    compile fileTree(include: ['*.jar'], dir: 'libs')
    compile project(':classes')

    compile 'com.android.support:multidex:1.0.0'
}
4. 修改androidXml文件
<application
	android:name="android.support.multidex.MultiDexApplication"//添加


[    5.011335] init: cannot find '/system/bin/setup_fs', disabling 'setup_fs'
[    5.031199] init: property 'ro.serialno' doesn't exist while expanding '${ro.serialno}'
[    5.031300] init: cannot expand '${ro.serialno}' while writing to '/sys/class/android_usb/android0/iSerial'
[    5.039208] init: cannot find '/system/bin/install-recovery.sh', disabling 'flash_recovery'


