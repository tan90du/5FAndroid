package com.autonavi.smarteye.service;

import android.util.Log;

import com.autonavi.smarteye.config.Config;
import com.autonavi.smarteye.service.http.HttpClient;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * 数据加载类。从SaaS平台获取数据
 */
public class DataLoader {
    private static final String TAG = "DataLoader";
    private static final String ipAddress = Config.serverIpAddress;
    private static final String deviceId = Config.deviceId;

    /**
     * 获取设备关联的病害母库清单
     *
     * @return 病害母库清单
     */
    public static String getDiseaseLibrary() throws IOException {
        String requestUrl = ipAddress + "/get/library/list";
        String response = HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        Log.i("DataLoader", "getDiseaseLibrary 响应内容: " + response);
        return response;
    }


}
