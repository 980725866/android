#define LOG_TAG "HelloService"

//#define LOG_NDEBUG 0

#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <cutils/properties.h>
#include <utils/Log.h>
#include "IHelloService.h"


using namespace android;
int main()
{
   int ret  = -1;
    sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();

	ret = sm->addService(String16("hello"),new BnHelloService());

	if(ret != 0) {
		ALOGI("test_server add failed");
		return -1;
	}
	ALOGI("test_server  loop: %p", sm.get());
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();

	
    return 0;
}


