
#ifndef _HARDWARE_LED_H
#define _HARDWARE_LED_H

#include <hardware/hardware.h>

__BEGIN_DECLS

#define LED_HARDWARE_MODULE_ID 	"led"

struct led_device_t {
    
    struct hw_device_t common;

    
    int (*ledCtrl)(struct led_device_t* device,int cmd,int args);
    void (*ledClose)(struct led_device_t* device);
	int (*ledOpen)(struct led_device_t* device);
	
};

static inline int device_open(const struct hw_module_t* module,struct led_device_t** device)
{
    return module->methods->open(module, NULL, (struct hw_device_t**)device);
}

__END_DECLS

#endif  // _HARDWARE_LED_H
