package com.autonavi.smarteye.config;

import android.content.Context;
import android.provider.Settings;
import android.util.Log;

import com.autonavi.smarteye.util.SDCardTool;

import java.lang.reflect.Method;


public class Config {
    // 设备物理ID
//    public static String deviceId = "124412";
    public static String deviceId = getSystemPropertiesByKey("ro.serialno");
    // 服务器IP地址
    public static String serverIpAddress = "http://192.168.2.16:48080/admin-api/pcmanager/arithmetic";
    public static Integer TaskStatus = 0;//0 设备下线 1 设备上线 2 任务开始 3 任务暂停

//    public static String serverIpAddress = "http://113.44.214.28:28080/admin-api/pcmanager/arithmetic";
    // 设备IMEI
//    private static String IMEI = getSystemPropertiesByKey("persist.telephony.imei1");
    // 设备SN
//    private static String SN = getSystemPropertiesByKey("ro.serialno");
    // 全局Context
    private static Context sContext;

    public static void init(Context context) {
        TaskStatus = 1;
        // 初始化sContext
        if (context == null) {
            throw new IllegalArgumentException("context 不能为 null");
        }
        sContext = context.getApplicationContext();

        // 初始化SDCard读写权限
        if (SDCardTool.checkPermissions()) {
        } else SDCardTool.requestPermissions();
    }

    public static Context getContext() {
        if (sContext == null) {
            throw new IllegalStateException("Config 尚未初始化，请在 Application.onCreate() 中调用 Config.init(context)");
        }
        return sContext;
    }

    private static String getSystemPropertiesByKey(String key) {
        Class<?> clazz = null;
        String deviceType = null;
        try {
            clazz = Class.forName("android.os.SystemProperties");
            Method method = clazz.getMethod("get", String.class);
            deviceType = (String) method.invoke(null, key);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return deviceType;
    }

}

