file:
	led_driver.c
	led_jni.c
	MyHardware.java

注意事项
android_studio
1 在libs下添加armeabi目录
2 在APP build.grade下添加
sourceSets{
	main{
		jniLibs.srcDirs = ['libs']
	}
}

编译 libledjin.so
arm-linux-gcc -fPIC -shared led_jni.c -o libledjni.so -I /usr/lib/jvm/java-1.7.0-openjdk-amd64/include/  -nostdlib /work/android-5.0.2/prebuilts/ndk/9/platforms/android-19/arch-arm/usr/lib/libc.so -I /work/android-5.0.2/prebuilts/ndk/9/platforms/android-19/arch-arm/usr/include /work/android-5.0.2/prebuilts/ndk/9/platforms/android-19/arch-arm/usr/lib/liblog.so



