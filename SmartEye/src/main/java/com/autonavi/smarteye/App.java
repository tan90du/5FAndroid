package com.autonavi.smarteye;


import android.app.Application;
import android.util.Log;

import com.autonavi.smarteye.config.Config;
import com.autonavi.smarteye.service.DataLoader;
import com.autonavi.smarteye.util.imgupload.MarkedFolderMonitor;
import com.autonavi.smarteye.util.imgupload.OriginalFolderMonitor;
import com.autonavi.smarteye.util.imgupload.OriginalUploadManager;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.concurrent.TimeUnit;

public class App extends Application {

    private static final String TAG = "Application";

    @Override
    public void onCreate() {
        super.onCreate();

        Config.init(this);

        /*new Thread(()->{
            String jsonStr = "{\n" +
                    "    \"point1\": {\n" +
                    "        \"latitude\": 40.7128,\n" +
                    "        \"longitude\": -74.006,\n" +
                    "        \"elevation\": 10\n" +
                    "    },\n" +
                    "    \"point2\": {\n" +
                    "        \"latitude\": 34.0522,\n" +
                    "        \"longitude\": -118.2437,\n" +
                    "        \"elevation\": 880\n" +
                    "    }\n" +
                    "}";
            DataLoader.calculateAndSendSlope(jsonStr);
        }).start();*/

//        Log.i(TAG, "休眠2分钟，等待算力终端加载模型");
//        try {
//            TimeUnit.MINUTES.sleep(2);
//        } catch (InterruptedException e) {
//            throw new RuntimeException(e);
//        }
//        Log.i(TAG, "等待算力终端加载模型完毕");
//
//        // 开启所有任务
        ActiveTasks.startAllTasks();

        System.loadLibrary("smarteye");
        SmartEye.start();
    }
}
