#define LOG_TAG "LedServiceHal"

#include <hardware/led_hal.h>
#include <hardware/hardware.h>

#include <cutils/log.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

static int fd = -1;
static int led_open(struct led_device_t* device)
{
	fd = open("/dev/leds",O_RDWR);
	if(fd < 0){
		ALOGE("hal open error fd =%d",fd);
		return -1;
	}
	ALOGI("hal led_open yes fd = %d\n",fd);
	return 0;
}

static void led_close(struct led_device_t* device __unused)
{
	if(fd >=0)
		close(fd);
}

static int led_ctrl(struct led_device_t* device __unused,int cmd,int args)
{
	ALOGI("hal led_ctrl cmd = %d,args = %d\n",cmd,args);
	ioctl(fd,cmd,args);
    return 0;
}

static int led_device_open(const hw_module_t* module, const char* id __unused,
                      hw_device_t** device __unused) {
    
	struct led_device_t *dev = malloc(sizeof(struct led_device_t));
	if (!dev) {
        ALOGE("Can not allocate memory for the led device");
        return -ENOMEM;
    }
    memset(dev, 0, sizeof(*dev));
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*)module;
    dev->ledOpen = led_open;
    dev->ledCtrl = led_ctrl;
	dev->ledClose = led_close;
    *device = (struct hw_device_t *) dev;

    return 0;
}

static struct hw_module_methods_t led_module_methods = {
    .open = led_device_open,
};

struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .id = LED_HARDWARE_MODULE_ID,
    .name = "Default led HAL",
    .author = "The Android Open Source Project",
    .methods = &led_module_methods,
};
