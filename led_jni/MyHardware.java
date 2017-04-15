
public class MyHardware {

    public static native int ledOpen();

    public static native int ledClose();

    public static native int ledCrtl(int cmd,int args);

    static {
        try {
            System.loadLibrary("ledjni");
        }catch (Exception e){
            System.out.println(e);
        }
    }
}
