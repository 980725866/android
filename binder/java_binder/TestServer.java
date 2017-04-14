import android.os.ServiceManager;
import android.util.Slog;

public class TestServer
{
	private static final String TAG = "TestServer";

	public static void main(String args[])
	{
		Slog.i(TAG,"add hello server");
		ServiceManager.addService("hello", new HelloService());
		while (true)
        {
            try {
            	Thread.sleep(100);
          	} catch (Exception e){}
        }
	}

}











