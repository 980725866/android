

IPC : Inter-Process Communication, 进程间通信
RPC : Remote Procedure Call, 远程过程调用

frameworks\native\cmds\servicemanager
service_manager.c :
a. binder_open
b. binder_become_context_manager
c. binder_loop(bs, svcmgr_handler);
   c.1 res = ioctl(bs->fd, BINDER_WRITE_READ, &bwr);
   c.2 binder_parse
          // 解析
          // 处理  : svcmgr_handler
                       SVC_MGR_GET_SERVICE/SVC_MGR_CHECK_SERVICE : 获取服务
                       SVC_MGR_ADD_SERVICE : 注册服务          
          // 回复          

bctest.c
注册服务的过程:
a. binder_open
b. binder_call(bs, &msg, &reply, 0, SVC_MGR_ADD_SERVICE)
                   // 含有服务的名字
                         // 它会含有servicemanager回复的数据 
                                 // 0表示servicemanager
                                    // code: 表示要调用servicemanager中的"addservice函数"


获取服务的过程:
a. binder_open
b. binder_call(bs, &msg, &reply, target, SVC_MGR_CHECK_SERVICE)
                   // 含有服务的名字
                         // 它会含有servicemanager回复的数据, 表示提供服务的进程 
                                 // 0表示servicemanager
                                    // code: 表示要调用servicemanager中的"getservice函数"
                                    

binder.c (封装好的C函数)

