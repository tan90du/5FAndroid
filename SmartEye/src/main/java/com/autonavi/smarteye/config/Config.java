package com.autonavi.smarteye.config;

import android.content.Context;
import android.provider.Settings;
import android.util.Log;

import com.autonavi.smarteye.util.sdcard.SDCardTool;

import java.io.File;
import java.lang.reflect.Method;


public class Config {
    private static final String TAG = "Config";
    // 设备物理ID。这里使用SN编号作为物理ID
    public static String deviceId = getSystemPropertiesByKey("ro.serialno");
    // 服务器IP地址
    public static String ip = "http://36.137.19.162:48080";
    public static String serverIpAddress = ip + "/admin-api/pcmanager/arithmetic";
    // 云服务器地址
    public static String cloudServerIpAddress = "http://36.134.115.63:9001/api";
    // 任务是否开启 flag
    public static volatile String taskStatus = "pause"; // open, pause
    // 全局Context
    private static Context sContext;

    public static void init(Context context) {
        // 初始化sContext
        if (context == null) {
            Log.e(TAG, "context 不能为 null");
        }
        sContext = context.getApplicationContext();

        // 初始化SDCard读写权限
        if (SDCardTool.checkPermissions()) {
        } else SDCardTool.requestPermissions();

        String originalPath = "/storage/ext4_sdcard/Android/data/img/original";
        String markedPath = "/storage/ext4_sdcard/Android/data/img/marked";
        File originalDir = new File(originalPath);
        File markedDir = new File(markedPath);
        if (!originalDir.exists() || !originalDir.isDirectory()) {
            if (originalDir.mkdirs()) {
                Log.i(TAG, "创建文件夹 " + originalDir);
            }
        }
        if (!markedDir.exists() || !markedDir.isDirectory()) {
            if (markedDir.mkdirs()) {
                Log.i(TAG, "创建文件夹 " + markedDir);
            }
        }
    }

    public static Context getContext() {
        if (sContext == null) {
            Log.e(TAG, "Config 尚未初始化，请在 Application.onCreate() 中调用 Config.init(context)");
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
            Log.e(TAG, "" + e);
        }
        return deviceType;
    }

}

