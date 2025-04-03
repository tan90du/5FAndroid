package com.autonavi.smarteye.service;

import android.util.Log;

import com.autonavi.smarteye.config.Config;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class DataSender {

    private static final String ipAddress = Config.serverIpAddress;
    private static final String deviceId = Config.deviceId;

    public static String uploadImg(String json) {
        Log.i("拿到数据了", json);
        try {
            // 指定目标 URL
            String ipAddress = Config.serverIpAddress;
            URL url = new URL(ipAddress + "/location/reporting");
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            Log.i("DataSender", conn.toString());

            // 设置请求方法为 POST
            conn.setRequestMethod("POST");
            // 允许向服务器发送请求体
            conn.setDoOutput(true);
            // 设置请求头，指定发送的数据类型为 JSON
            conn.setRequestProperty("Content-Type", "application/json; utf-8");
            // 设置接受返回数据的类型
            conn.setRequestProperty("Accept", "application/json");

            // 构造要发送的 JSON 数据
            String jsonInputString = "{\"mac\": \"A1:B2:C3:D4\", \"location\": \"113.4214,32.142\"}";

            // 写入数据到请求体
            try (OutputStream os = conn.getOutputStream()) {
                byte[] input = jsonInputString.getBytes("utf-8");
                os.write(input, 0, input.length);
            }

            // 获取响应状态码
            int responseCode = conn.getResponseCode();
            Log.d("DataSender", "POST 响应码: " + responseCode);

            // 根据响应状态读取响应数据
            try (BufferedReader br = new BufferedReader(
                    new InputStreamReader(conn.getInputStream(), "utf-8"))) {
                StringBuilder response = new StringBuilder();
                String responseLine;
                while ((responseLine = br.readLine()) != null) {
                    response.append(responseLine.trim());
                }
                Log.d("DataSender", "响应内容: " + response.toString());
            }

        } catch (Exception e) {
            Log.e("DataSender", "上传异常 " + e);
        }
        return "成功了";
    }


    /**
     * 上报设备的实时地理位置
     * @return 实时地理位置
     */
    public static String LocationReport() {
        try {
            URL url = new URL(ipAddress + "/location/reporting");
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();

            conn.setRequestMethod("POST");
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

