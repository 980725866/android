import android.util.Slog;

public class HelloService extends IHelloService.Stub
{
	private static final String TAG = "HelloService";
	private static int cnt1 = 0;
	private static int cnt2 = 0;
	public void sayhello() throws android.os.RemoteException{
		cnt1++;
		Slog.i(TAG, "server sayhello : cnt1 = "+cnt1);
		//System.out.println("server sayhello" + cnt1);
	}

	public int sayhello_to(String name) throws android.os.RemoteException{
		cnt2++;
		Slog.i(TAG, "server sayhello_to name = " + name+" cnt2 = "+cnt2);
		//System.out.println("server sayhello_to name = " + name+" cnt2 = "+cnt2);
		return cnt2;
    }

}
































