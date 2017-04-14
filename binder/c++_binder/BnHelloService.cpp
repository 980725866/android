#define LOG_TAG "HelloService"

#include "IHelloService.h"
#include <utils/Log.h>
#include <utils/String8.h>
#include <utils/String16.h>

namespace android {

status_t BnHelloService::onTransact( uint32_t code,
								   const Parcel& data,
								   Parcel* reply,
								   uint32_t flags)
{
	int ret;
	data.readInt32();
	switch(code)
	{
		case SAY_HELLO:{
			sayhello();
			return NO_ERROR;
			}
			break;
		case SAY_HELLO_TO: {
			String16 name16 = data.readString16();
			String8 name8(name16);
			ret = sayhello_to(name8.string());
			
			reply->writeInt32(ret);
			return NO_ERROR;
			}
			break;
		default:
			return BBinder::onTransact(code, data, reply, flags);
	}
}

void BnHelloService::sayhello()
{
	static int ret = 0;
	ALOGI("cnt = %d",++ret);
}

int BnHelloService::sayhello_to(const char *name)
{
	static int status = 0;
	ALOGI("name = %s",name);
	status++;
	return status;
}


}

