#define LOG_TAG "HelloService"

#include <stdint.h>
#include <sys/types.h>

#include <binder/Parcel.h>
#include <binder/IMemory.h>
#include <utils/Errors.h>  // for status_t
#include <utils/String8.h>

#include "IHelloService.h"
//#include <utils/RefBase.h>
//#include <binder/IInterface.h>

using namespace android ;

class BpHelloService: public BpInterface <IHelloService>
{
public:
    BpHelloService(const sp<IBinder>& impl)
        : BpInterface<IHelloService>(impl)
    {
    
    }
	void sayhello()
	{
		Parcel data,reply;
		data.writeInt32(0);
	    remote()->transact(SAY_HELLO, data, &reply);
		ALOGI("client sayhello\n");
	}

	int sayhello_to(const char *name)
	{
		Parcel data,reply;
		data.writeInt32(0);
		data.writeString16(String16(name));
	    remote()->transact(SAY_HELLO_TO, data, &reply);
		ALOGI("client sayheoo_to \n");
		return reply.readInt32();
	}
	
};




IMPLEMENT_META_INTERFACE(HelloService, "android.media.IHelloService");

