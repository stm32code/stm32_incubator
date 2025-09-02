package com.example.BluetoothSmartBox.Utils;


import com.example.BluetoothSmartBox.Bluetooth.BlueTooth;

// 常量类
public class Common {
    public static String MY_UUID = "00001101-0000-1000-8000-00805F9B34FB";  //要连接通信，此id必须和对方设备保持一致
    public static final int MSG_GET_DATA = 101; //获取到数据
    public static final int MSG_ERROR = 55;   // 错误
    public static final int MSG_CONNET = 200; // 连接到服务器
    public static final int SEND_OK = 201; //发送成功
    public static final int SEND_ERROR = 202;//发送失败
    public static final int SCAN_START = 52; //扫描蓝牙
    public static String CONNET_NAME = ""; //连接的蓝牙名称
    public static boolean ISBTCONNECT = false;  //蓝牙是否连接
    public static BlueTooth blueTooth;
}
