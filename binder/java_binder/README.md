3. 用java实现hello服务_测试

logcat TestServer:* TestClient:* HelloService:* *:S &
CLASSPATH=/mnt/android_fs/TestServer.jar app_process / TestServer &
CLASSPATH=/mnt/android_fs/TestClient.jar app_process / TestClient hello
CLASSPATH=/mnt/android_fs/TestClient.jar app_process / TestClient hello world

启动方式的差别:
dalvikvm
app_process ：会创建2个binder线程: Binder_1, Binder_2

2. 用java实现hello服务_编程
怎么做?
2.1 定义接口:
    写IHelloService.aidl文件, 上传, 编译, 得到IHelloService.java
    里面有Stub  : onTransact, 它会分辨收到数据然后调用sayhello, sayhello_to
          Proxy : 提供有sayhello, sayhello_to两个函数, 它们会构造数据然后发送给server

2.2 实现服务类: HelloService.java
    在里面定义sayhello, sayhello_to

2.3 实现APP: TestServer, TestClient    
    TestServer : addService, 循环
    TestCliet  : getService, 调用sayhello,sayhello_to(来自Proxy)

