package com.autonavi.smarteye.config;

import android.content.Context;
import android.provider.Settings;

import java.lang.reflect.Method;


public class Config {
    // 设备物理ID
    public static final String deviceId = "SNafjljflajffa";
    // 服务器IP地址
    public static final String serverIpAddress = "http://113.44.214.28:28080/admin-api/pcmanager/arithmetic";
    // 设备IMEI
    private static final String IMEI = getSystemPropertiesByKey("persist.telephony.imei1");
    // 设备SN
    private static final String SN = getSystemPropertiesByKey("ro.serialno");

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

