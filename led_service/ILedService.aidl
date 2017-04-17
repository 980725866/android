package android.os;

/** {@hide} */
interface ILedService
{
    //void ledOpen();
    int ledCtrl(int cmd,int args);
}