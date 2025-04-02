package com.autonavi.smarteye.service;

import android.util.Log;

import com.autonavi.smarteye.config.Config;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * 数据加载类。从SaaS平台获取数据
 */
public class DataLoader {

    private static final String ipAddress = Config.serverIpAddress;
    private static final String deviceId = Config.deviceId;

    /**
     * 获取设备关联的病害母库清单
     *
     * @return 病害母库清单
     */
    public static String getDiseaseLibrary() {
        try {
            URL url = new URL(ipAddress + "/location/reporting");
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();

            conn.setRequestMethod("GET");
            conn.setDoOutput(true);
            conn.setRequestProperty("Content-Type", "application/json; utf-8");
            conn.setRequestProperty("Accept", "application/json");

            String jsonInputString = deviceId; // 传递设备Id

            try (OutputStream os = conn.getOutputStream()) {
                byte[] input = jsonInputString.getBytes("utf-8");
                os.write(input, 0, input.length);
            }

            int responseCode = conn.getResponseCode();
            Log.i("DataLoader", "POST 响应码: " + responseCode);

            try (BufferedReader br = new BufferedReader(
                    new InputStreamReader(conn.getInputStream(), "utf-8"))) {
                StringBuilder response = new StringBuilder();
                String responseLine;
                while ((responseLine = br.readLine()) != null) {
                    response.append(responseLine.trim());
                }
                Log.i("DataLoader", "响应内容: " + response.toString());
                return response.toString();
            }

        } catch (Exception e) {
            throw new RuntimeException("DataLoader 设备关联的病害母库清单请求异常 deviceId " + deviceId + e);
        }
    }

    /**
     * 获取设备电子围栏坐标数据（多边形坐标点集合）
     *
     * @return
     */
    public static String getFence() {
        try {
            URL url = new URL(ipAddress + "/get/fence/list");
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();

            conn.setRequestMethod("GET");
            conn.setDoOutput(true);
            conn.setRequestProperty("Content-Type", "application/json; utf-8");
            conn.setRequestProperty("Accept", "application/json");

            String jsonInputString = Config.deviceId; // 传递设备Id

            try (OutputStream os = conn.getOutputStream()) {
                byte[] input = jsonInputString.getBytes("utf-8");
                os.write(input, 0, input.length);
            }

            int responseCode = conn.getResponseCode();
            Log.i("DataLoader", "POST 响应码: " + responseCode);

            try (BufferedReader br = new BufferedReader(
                    new InputStreamReader(conn.getInputStream(), "utf-8"))) {
                StringBuilder response = new StringBuilder();
                String responseLine;
                while ((responseLine = br.readLine()) != null) {
                    response.append(responseLine.trim());
                }
                Log.i("DataLoader", "响应内容: " + response.toString());
                return response.toString();
            }

        } catch (Exception e) {
            throw new RuntimeException("DataLoader 设备关联的病害母库清单请求异常 deviceId " + deviceId + e);
        }
    }

}
