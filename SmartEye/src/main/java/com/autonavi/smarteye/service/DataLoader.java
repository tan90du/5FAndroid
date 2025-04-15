package com.autonavi.smarteye.service;

import android.util.Log;

import com.autonavi.smarteye.config.Config;
import com.autonavi.smarteye.service.http.HttpCallback;
import com.autonavi.smarteye.service.http.HttpClient;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

/**
 * 数据加载类。从SaaS平台获取数据
 */
public class DataLoader {
    private static final String TAG = "DataLoader";
    private static final String ipAddress = Config.serverIpAddress;
    private static final String deviceId = Config.deviceId;
    private static List<List<String>> fenceList = null;
    private static volatile double pointLng = 0.0; // 经度
    private static volatile double pointLat = 0.0; // 纬度
    private static long lastSendLocationTime;

    static {
        Calendar calendar = Calendar.getInstance();
        long timestamp = calendar.getTimeInMillis();
        lastSendLocationTime = timestamp;
    }


    /**
     * 获取设备关联的病害母库清单
     *
     * @return 病害母库清单
     */
    public static String getDiseaseLibrary() {
        String requestUrl = ipAddress + "/get/library/list";
        String response = null;
        int successCode = -1;
        while (successCode != 0) {
            try {
                response = HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
                JsonObject jsonObj = JsonParser.parseString(response).getAsJsonObject();
                successCode = jsonObj.get("code").getAsInt();
            } catch (Exception e) {
                Log.e(TAG, "获取病毒母库列表失败 " + e);
            }
        }
        Log.i(TAG, "病毒母库列表 " + response);
        return response;
    }

    /**
     * 获取电子围栏列表
     *
     * @return 设备电子围栏坐标数据（多边形坐标点集合）
     */
    public static String getFenceList() {
        String requestUrl = ipAddress + "/get/fence/list";
        String response = null;
        try {
            response = HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        } catch (Exception e) {
            Log.e(TAG, "电子围栏列表出错 " + e);
        }
        Log.i(TAG, "电子围栏列表 " + response);
        return response;
    }

    /**
     * 设备位置上报
     *
     * @return 上报设备的实时地理位置
     */
    public static String sendDeviceLocation(String location) {
        String requestUrl = ipAddress + "/location/reporting";
        String[] sp = location.split(",");
        pointLng = Double.parseDouble(sp[0]);
        pointLat = Double.parseDouble(sp[1]);

        // 每 5 秒上传一次
        Calendar calendar = Calendar.getInstance();
        long timestamp = calendar.getTimeInMillis();
        if (pointLng <= 180 && pointLat <= 90 && timestamp - lastSendLocationTime >= 5000) {
            try {
                String json = "{\"mac\":\"" + deviceId + "\",\"location\":\"" + location + "\"}";
                HttpClient.fetchDataAsync(requestUrl, "POST", json, new HttpCallback() {
                    @Override
                    public void onSuccess(String result) {
                        Log.i(TAG, "设备位置上报成功");
                    }

                    @Override
                    public void onFailure(Exception e) {
                        Log.e(TAG, "设备位置上报出错 " + e);
                    }
                });
            } catch (Exception e) {
                Log.e(TAG, "设备位置上报出错 " + e);
            }
            lastSendLocationTime = timestamp;
        }
        return "";
    }

    /**
     * 设备上线通知
     *
     * @return 设备开机标记设备为在线状态
     */
    public static String sendOnlineMessage() {
        String requestUrl = ipAddress + "/online";
        String response = null;
        try {
            response = HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        } catch (Exception e) {
            Log.e(TAG, "设备位置上报出错 " + e);
        }
//        System.out.println("位置" + response);
        return response;
    }

    /**
     * 设备下线通知
     *
     * @return 设备关机标记设备为离线状态
     */
    public static void sendOfflineMessage() {
        String requestUrl = ipAddress + "/offline";
        try {
            HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        } catch (Exception e) {
        }
    }

    /**
     * 任务开始
     */
    public static void taskStart() {
        String requestUrl = ipAddress + "/task/start";
        try {
            HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        } catch (Exception e) {
        }
    }

    /**
     * 任务暂停
     */
    public static void taskPause() {
        String requestUrl = ipAddress + "/task/pause";
        try {
            HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        } catch (Exception e) {
        }
    }

    /**
     * 计算坡度信息并上传
     *
     * @param req
     * @return
     */
    public static void calculateAndSendSlope(String req) {
        String requestUrl = Config.cloudServerIpAddress + "/calculate_slope";
        HttpClient.fetchDataAsync(requestUrl, "POST", req, new HttpCallback() {
            @Override
            public void onSuccess(String result) {
                JsonObject jsonObj = JsonParser.parseString(result).getAsJsonObject();
                String resultCode = jsonObj.get("resultCode").getAsString();
                if (!"0".equals(resultCode)) {
                    Log.e(TAG, "坡度计算失败");
                } else {
                    // 上传坡度信息
                }
            }

            @Override
            public void onFailure(Exception e) {
                Log.e(TAG, "坡度计算失败");
            }
        });
    }

    /**
     * 获取坡度配置列表
     *
     * @return 获取道路坡度检测配置参数
     */
    public static String getSlopeList() {
        String requestUrl = ipAddress + "/get/slope/list";
        String response = null;
        try {
            response = HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        } catch (Exception e) {
            Log.e(TAG, "获取坡度配置列表出错 " + e);
        }
        Log.i(TAG, "Method… " + response);
        return response;
    }

    /**
     * 判断是否进入电子围栏区域
     *
     * @return 是否进入电子围栏
     */
    public static boolean isAccessFence() {
        if (null == fenceList) {
            String response = getFenceList();
            JsonObject jsonObj = JsonParser.parseString(response).getAsJsonObject();
            JsonArray dataArray = jsonObj.getAsJsonArray("data");

            fenceList = new ArrayList<>();
            for (JsonElement element : dataArray) {
                JsonArray subArray = element.getAsJsonArray();
                List<String> subList = new ArrayList<>();
                for (JsonElement strElem : subArray) {
                    subList.add(strElem.getAsString());
                }
                fenceList.add(subList);
            }
        }

        //判断是否进入电子围栏
        if (isPointInFence(pointLng, pointLat, fenceList)) {
            // 启动任务
            Log.i(TAG, "当前位置 经度 " + pointLng + " 纬度 " + pointLat + " 在电子围栏中，任务运行");
            return true;
        } else {
            Log.i(TAG, "当前位置 经度 " + pointLng + " 纬度 " + pointLat + " 不在电子围栏中，任务暂停");
            return false;
        }
    }

    /**
     * 判断给定点 (pointLng, pointLat) 是否在任意一个电子围栏区域内
     *
     * @param pointLng  点的经度
     * @param pointLat  点的纬度
     * @param fenceList 电子围栏列表，每个子 List 表示一个区域，区域中的每个元素为 "经度,纬度" 字符串
     * @return 如果点在任何一个区域内，返回 true；否则返回 false
     */
    private static boolean isPointInFence(double pointLng, double pointLat, List<List<String>> fenceList) {
        if (pointLng <= 180.0 && pointLat <= 90.0) {
            for (List<String> fence : fenceList) {
                if (isPointInPolygon(pointLng, pointLat, fence)) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * 利用射线法判断点是否在多边形内部，并补充判断点是否在多边形边上
     *
     * @param pointLng 点的经度
     * @param pointLat 点的纬度
     * @param fence    多边形顶点坐标列表，格式为 "经度,纬度"
     * @return 如果点在多边形内部或恰好在多边形边上，返回 true，否则返回 false
     */
    private static boolean isPointInPolygon(double pointLng, double pointLat, List<String> fence) {
        if (fence.isEmpty()) {
            return false;
        }
        List<String> polygonPoints = new ArrayList<>(fence);
        // 保证多边形闭合：若首尾不一致，则加入首点
        if (!polygonPoints.get(0).equals(polygonPoints.get(polygonPoints.size() - 1))) {
            polygonPoints.add(polygonPoints.get(0));
        }

        boolean inside = false;
        int n = polygonPoints.size();
        for (int i = 0; i < n - 1; i++) {
            String[] coord1 = polygonPoints.get(i).split(",");
            double lng1 = Double.parseDouble(coord1[0]);
            double lat1 = Double.parseDouble(coord1[1]);

            String[] coord2 = polygonPoints.get(i + 1).split(",");
            double lng2 = Double.parseDouble(coord2[0]);
            double lat2 = Double.parseDouble(coord2[1]);

            // 先判断点是否在当前线段上
            if (isPointOnLineSegment(pointLng, pointLat, lng1, lat1, lng2, lat2)) {
                return true;
            }

            // 射线法判定：判断点与边的相交情况
            if (((lat1 > pointLat) != (lat2 > pointLat)) &&
                    (pointLng < (lng2 - lng1) * (pointLat - lat1) / (lat2 - lat1) + lng1)) {
                inside = !inside;
            }
        }
        return inside;
    }

    /**
     * 判断点 (px, py) 是否在线段 (ax, ay) 到 (bx, by) 上。
     * 注意：这里采用一定的容差值 tolerance，避免浮点计算误差。
     *
     * @param px 点的经度
     * @param py 点的纬度
     * @param ax 线段起点的经度
     * @param ay 线段起点的纬度
     * @param bx 线段终点的经度
     * @param by 线段终点的纬度
     * @return 如果点在线段上则返回 true，否则返回 false
     */
    private static boolean isPointOnLineSegment(double px, double py,
                                                double ax, double ay,
                                                double bx, double by) {
        // 计算向量叉乘是否接近0（在直线上），容差值可根据实际需求调整
        double tolerance = 1e-9;
        double cross = (px - ax) * (by - ay) - (py - ay) * (bx - ax);
        if (Math.abs(cross) > tolerance) {
            return false;
        }
        // 判断 px,py 是否在线段 [ax, bx] 与 [ay, by] 的范围内
        double dot = (px - ax) * (bx - ax) + (py - ay) * (by - ay);
        if (dot < 0) {
            return false;
        }
        double squaredLen = (bx - ax) * (bx - ax) + (by - ay) * (by - ay);
        if (dot > squaredLen) {
            return false;
        }
        return true;
    }
}
