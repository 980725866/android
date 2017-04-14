logcat HelloService:* GoodbyeService:* TestService:* *:S &
mount -t nfs -o nolock 192.168.1.100:/work /mnt

参考文件:
frameworks\av\include\media\IMediaPlayerService.h     (IMediaPlayerService,BnMediaPlayerService)
frameworks\av\media\libmedia\IMediaPlayerService.cpp                      (BpMediaPlayerService)
frameworks\av\media\libmediaplayerservice\MediaPlayerService.h
frameworks\av\media\libmediaplayerservice\MediaPlayerService.cpp
frameworks\av\media\mediaserver\Main_mediaserver.cpp   (server, addService)

参考frameworks\av\media\mediaserver\Android.mk

server注册服务时, 对每个服务都提供不同的ptr/cookie,
在驱动程序里对每个服务都构造一个binder_node, 它也含有ptr/cookie

client使用服务前要先getService：会在驱动程序里对该服务构造一个binder_ref, 
binder_ref含有desc, node成员, desc是整数, node指向对应服务的binder_node

使用服务时, client构造数据，调用ioctl：数据里含有handle

驱动程序根据handle找到binder_ref(desc==handle), 找到binder_node, 再找到server,
从binder_node取出ptr/cookie连同那些数据发给server

server根据ptr/cookie知道要调用哪一个服务，．．．．

最核心函数: ioctl
client的最核心数据是：handle
server的最核心数据是：ptr/cookie


4. 内部机制_代理类分析: BpServiceManager和BpHelloService
4.1 获得BpServiceManager对象的过程:
    defaultServiceManager构造了一个BpServiceManager对象，
    其中它的mRemote = new BpBinder(0); // mRemote->mHandle=0

defaultServiceManager // IServiceManager.cpp
                           // 把BpBinder(mHandle=0)对象转换为IServiceManager接口(BpServiceManager)                           
	gDefaultServiceManager = interface_cast<IServiceManager>(
                ProcessState::self()->getContextObject(NULL));
分析:
ProcessState::self()->getContextObject(NULL)
	getStrongProxyForHandle(0); 
		b = new BpBinder(handle);   // mHandle=handle=0


interface_cast<IServiceManager>(new BpBinder(0))  // IInterface.h
	IServiceManager::asInterface(obj);
			return new BpServiceManager(obj); // mRemote=obj=new BpBinder(0);


4.2 获得BpHelloService对象的过程:
    调用BpServiceManager的getService函数获得一个flat_binder_object，
    从中取出handle, 创建一个BpBinder(handle),
    然后使用interface_cast使用这个BpBinder创建一个BpHelloService对象

// binder是BpBinder对象, 里面含有HelloService的handle
sp<IBinder> binder =
        sm->getService(String16("hello")); // IServiceManager.cpp
        			// 构造数据: 数据中肯定含有"hello"
        			// 发送数据: 给handle 0, 即 service_manager进程
        			// 从收到的回复中取出HelloService的handle
        					 return reply.readStrongBinder();
        					 						unflatten_binder(ProcessState::self(), *this, &val);
        					 								*out = proc->getStrongProxyForHandle(flat->handle);
        					 													new BpBinder(handle); 
        
                            // 把binder转换为IHelloService接口(BpHelloService对象)
                            // binder是BpBinder对象, 里面含有HelloService的handle
sp<IHelloService> service = interface_cast<IHelloService>(binder);


4.3 代理类如何发送数据: ioctl, 数据里含有handle, 含有其他构造的参数
        构造好数据之后，调用：
        remote()->transact(...)
             IPCThreadState::self()->transact(mHandle, code, data, reply, flags);

5. 内部机制_数据传输: ProcessState和IPCThreadState

5.1 addService
    对于不同服务，构造flat_binder_object结构体，里面的.binder/.cookie对于不同的服务它的值不一样
														
sm->addService(String16("hello"), new BnHelloService());
			data.writeStrongBinder(service);  // service = new BnHelloService();
							flatten_binder(ProcessState::self(), val, this); // val = service = new BnHelloService();
									flat_binder_object obj;  // 参数 binder = val = service = new BnHelloService();
									IBinder *local = binder->localBinder(); // =this = new BnHelloService();
			            obj.type = BINDER_TYPE_BINDER;
			            obj.binder = reinterpret_cast<uintptr_t>(local->getWeakRefs());
			            obj.cookie = reinterpret_cast<uintptr_t>(local);  // new BnHelloService();
									
									

5.2 server如何分辨client想使用哪一个服务
    server收到数据里含有flat_binder_object结构体,
    它可以根据.binder/.cookie分析client想使用哪一个服务
    
    把.cookie转换为BnXXXX对象，然后调用它的函数:
    		
    		// 根据cookie构造了一个BBinder指针, 实际上是指向某个BnXXX对象
    		sp<BBinder> b((BBinder*)tr.cookie);
    		
    		// 然后调用它的transact函数
    		error = b->transact(tr.code, buffer, &reply, tr.flags);
    									err = onTransact(code, data, reply, flags);  // 就会调用到BnXXX里实现的onTransact
                            // 它就会根据code值来调用不同的函数  