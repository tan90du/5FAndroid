package com.autonavi.smarteye;


import static com.autonavi.smarteye.service.http.HttpClient.fetchDataAsync;

import android.app.Application;
import android.util.Log;

import com.autonavi.smarteye.config.Config;
import com.autonavi.smarteye.service.http.HttpCallback;

public class App extends Application {


    @Override
    public void onCreate() {
        super.onCreate();

        Config.init(this);

        System.loadLibrary("smarteye");
        SmartEye.start();

        new Thread(new Runnable() {
            @Override
            public void run() {

                // 异步请求示例 POST请求示例
                fetchDataAsync("http://113.44.214.28:28080/admin-api/pcmanager/arithmetic/location/reporting",
                        "POST", "{\"mac\": \"123\", \"location\": \"113.4214,32.142\"}", new HttpCallback() {
                            @Override
                            public void onSuccess(String response) {
                                // 处理逻辑
                                Log.i("Response", response);
                            }

                            @Override
                            public void onFailure(Exception e) {
                                Log.i("Request failed: ", e.getMessage());
                            }
                        });
            }
        }).start();
    }
}
