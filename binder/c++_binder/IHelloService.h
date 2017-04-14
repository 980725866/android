
#ifndef H_IHELLOSERVICE
#define H_IHELLOSERVICE
#include <utils/Errors.h>  // for status_t
#include <utils/KeyedVector.h>
#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <system/audio.h>
#include <utils/Log.h>
#include <utils/String8.h>
#include <utils/String16.h>


#define SAY_HELLO 		0
#define SAY_HELLO_TO	1

namespace android {

class IHelloService: public IInterface
{
public:
	DECLARE_META_INTERFACE(HelloService);
	virtual void sayhello() = 0;
	virtual int sayhello_to(const char *name) = 0;
	
};

class BnHelloService: public BnInterface<IHelloService>
{
public:
      virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags);
     virtual void sayhello();
	 virtual int sayhello_to(const char *name);

};

}

#endif	//H_IHELLOSERVICE











