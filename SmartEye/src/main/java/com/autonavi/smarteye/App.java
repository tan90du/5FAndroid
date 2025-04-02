package com.autonavi.smarteye;

import android.Manifest;
import android.app.Application;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.support.v4.app.ActivityCompat;
import android.telephony.TelephonyManager;
import android.util.Log;

import android.provider.Settings;

import com.autonavi.smarteye.config.Config;
import com.autonavi.smarteye.service.DataSender;

import java.lang.reflect.Method;
import java.net.NetworkInterface;
import java.util.Collections;
import java.util.List;

//public static String getAndroidId(Context context) {
//    return Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
//}

public class App extends Application {


    @Override
    public void onCreate() {
        super.onCreate();
        System.loadLibrary("smarteye");

        SmartEye.start();

//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//                Log.d("DataSender", "子线程调用 uploadImg()");
//                DataSender ds = new DataSender();
//                ds.uploadImg();
//                Log.d("DataSender", "uploadImg() 执行完成");
//            }
//        }).start();

//        // 检查权限
//        Context context = getApplicationContext();
//        if (ActivityCompat.checkSelfPermission(context, Manifest.permission.READ_PHONE_STATE) != PackageManager.PERMISSION_GRANTED) {
//            Log.i("Config", "Permission not granted");
//        }
//
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
//            Log.i("Config",Build.getSerial());  // 从 Android 8.0 开始使用 getSerial() 获取设备序列号
//        } else {
//            Log.i("Config",Build.SERIAL);
//        }


    }
}
