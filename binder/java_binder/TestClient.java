
import android.util.Slog;
import android.os.IBinder;
import android.os.ServiceManager;


public class TestClient
{
	private static final String TAG = "TestClient";

	public static void main(String[] args)
	{
		System.out.println("Usage: need parameter: <hello|goodbye> [name]");
		Slog.i(TAG,"get hello server");
		IBinder binder = ServiceManager.getService("hello");
		if (binder == null)
        {
            System.out.println("can not get hello service");
            Slog.i(TAG, "can not get hello service");
            return;
        }
		IHelloService service = IHelloService.Stub.asInterface(binder);

		if(args.length == 1){
			Slog.i(TAG,"client sayhello");
			try{
				service.sayhello();
			}catch(Exception e){}
		}
		else if(args.length == 2){
			Slog.i(TAG,"client sayhello_to");
			try{
				service.sayhello_to(args[1]);
			}catch(Exception e){}
		}
		
	}

}









