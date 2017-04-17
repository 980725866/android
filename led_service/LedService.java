package com.android.server;
import android.os.ILedService;

public class LedService extends ILedService.Stub {
	private static final String TAG = "LedService";
	
	public static native int native_ledOpen();
	public static native int native_ledCtrl(int cmd, int args);
	public static native void native_ledClose();

	public LedService()
	{
		native_ledOpen();
	}
	
	public int ledCtrl(int cmd, int args) throws android.os.RemoteException
	{
		return native_ledCtrl(cmd,args);
	}

}