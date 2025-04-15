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

public class App extends Application {

    private static final String TAG = "Application";

    @Override
    public void onCreate() {
        super.onCreate();

        Config.init(this);

        /**
         * 1. 发送上线通知
         * 2. 监听任务状态。进入电子围栏区域 - 开启任务；离开电子围栏区域 - 暂停任务
         */
        new Thread(() -> {
            // 发送上线通知
            String response = DataLoader.sendOnlineMessage();
            JSONObject jsonObject = null;
            try {
                jsonObject = new JSONObject(response);
                while (Integer.parseInt(jsonObject.getString("code")) != 4001001) {
                    Log.i(TAG, "设备上线失败");
                    response = DataLoader.sendOnlineMessage();
                    jsonObject = new JSONObject(response);
                }
                Log.i(TAG, "设备上线成功");
            } catch (JSONException e) {
                Log.e(TAG, "上线通知 Json解析错误 " + e);
            }

            // 监听任务开启状态
            while (true) {
                // 判断是否进入电子围栏
//                while (true) {
//                    if (DataLoader.isAccessFence()) {
                        //启动任务
                        Config.taskStatus = "open";
                        DataLoader.taskStart();
                        Log.i(TAG,"任务开始");
                        break;
//                    }
//                    try {
//                        TimeUnit.SECONDS.sleep(2);
//                    } catch (InterruptedException e) {
//                        throw new RuntimeException(e);
//                    }
//                }
                /*while (true) {
                    if (!DataLoader.isAccessFence()) {
                        //暂停任务
                        Config.taskStatus = "pause";
                        DataLoader.taskPause();
                        Log.i(TAG,"任务暂停");
                        break;
                    }
                    try {
                        TimeUnit.SECONDS.sleep(2);
                    } catch (InterruptedException e) {
                        throw new RuntimeException(e);
                    }
                }*/
            }
        }).start();

        // 开启原始文件夹监控，打 ZIP 压缩包
        new Thread(OriginalFolderMonitor::new).start();
        // 上传压缩包
        new Thread(OriginalUploadManager::new).start();
        // 监测标记图像的文件夹，并上传
        new Thread(MarkedFolderMonitor::new).start();

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

//        new Thread(()->{
//            DataLoader.getDiseaseLibrary();
//        }).start();

//        Log.i(TAG, "休眠3分钟，等待算力终端加载模型");
//        try {
//            TimeUnit.SECONDS.sleep(2);
//        } catch (InterruptedException e) {
//            throw new RuntimeException(e);
//        }
//        Log.i(TAG, "等待算力终端加载模型完毕");
        System.loadLibrary("smarteye");
        SmartEye.start();
    }
}
