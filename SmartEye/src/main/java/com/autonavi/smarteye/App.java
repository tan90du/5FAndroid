package com.autonavi.smarteye;


import static com.autonavi.smarteye.config.Config.deviceId;
import static com.autonavi.smarteye.service.DataLoader.getOnlineMessage;
import static com.autonavi.smarteye.service.DataLoader.getStartTask;
import static com.autonavi.smarteye.service.http.HttpClient.fetchDataAsync;

import android.app.Application;
import android.util.Log;

import com.autonavi.smarteye.config.Config;
import com.autonavi.smarteye.service.http.HttpCallback;
import com.autonavi.smarteye.service.http.HttpClient;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;

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
//
//                // 异步请求示例 POST请求示例
////                fetchDataAsync("http://113.44.214.28:28080/admin-api/pcmanager/arithmetic/location/reporting",
//                fetchDataAsync("http://192.168.2.16:48080/admin-api/pcmanager/arithmetic/location/reporting",
////                        "POST", "{\"mac\": \"123\", \"location\": \"113.4214,32.142\"}", new HttpCallback() {
//                        "POST", "{\"mac\": \"123\", \"location\": \"113.4214,32.142\"}", new HttpCallback() {
//                            @Override
//                            public void onSuccess(String response) {
//                                // 处理逻辑
//                                Log.i("Response", response);
//                            }
//
//                            @Override
//                            public void onFailure(Exception e) {
//                                Log.i("Request failed: ", e.getMessage());
//                            }
//                        });
//            }
//        }).start();

//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//////                 设备上线
////                String response = getOnlineMessage();
////                JSONObject jsonObject = null;
////                try {
////                    jsonObject = new JSONObject(response);
////                    while (Integer.parseInt(jsonObject.getString("code"))!=200) {
////                        Log.i("DataLoader", "设备上线失败: ");
////                        response = getOnlineMessage();
////                        jsonObject = new JSONObject(response);
////                    }
////                    Log.i("DataLoader", "设备上线成功: ");
////                } catch (JSONException e) {
////                    throw new RuntimeException(e);
////                }
//
//                while (true){
//                    // 判断是否进入电子围栏
//                    while (true){
//                        if(getStartTask()){
//                            //启动任务
//                            break;
//                        }
//                    }
//                    while (true){
//                        if(!getStartTask()){
//                            //暂停任务
//                            break;
//                        }
//                    }
//                }
//            }
//        }).start();

    }
}
