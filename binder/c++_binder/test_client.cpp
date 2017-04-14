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

int main(int argc __unused, char** argv)
{
	int ret  = -1;
	sp<ProcessState> proc(ProcessState::self());
	sp<IServiceManager> sm = defaultServiceManager();	
	sp<IBinder> binder = sm->getService(String16("hello"));
	if (binder == 0) {
		ALOGI("test_client getService failed");
		return -1;
	}
	sp<IHelloService> service =
	interface_cast<IHelloService>(binder);

	if(argc == 2) {
		service->sayhello();
		ALOGI("client call sayhello");
	}else if (argc == 3){
		int cnt = service->sayhello_to(argv[2]);
		ALOGI("client call sayhello_to, cnt = %d", cnt);
	}
	ALOGI("test_client: main");
    
    
}

