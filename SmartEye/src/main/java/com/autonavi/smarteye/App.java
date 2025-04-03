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
import com.autonavi.smarteye.util.SDCardTool;

import java.lang.reflect.Method;
import java.net.NetworkInterface;
import java.util.Collections;
import java.util.List;

public class App extends Application {


    @Override
    public void onCreate() {
        super.onCreate();

        Config.init(this);
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
    }
}
