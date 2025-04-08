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
import java.util.ArrayList;
import java.util.List;

/**
 * 数据加载类。从SaaS平台获取数据
 */
public class DataLoader {
    private static final String TAG = "DataLoader";
    public static final String ipAddress = Config.serverIpAddress;
    private static final String deviceId = Config.deviceId;
    public static Double pointLat=0.0;
    public static Double pointLng=0.0;
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

    /**
     * 获取电子围栏列表
     *
     * @return 设备电子围栏坐标数据（多边形坐标点集合）
     */
    public static String getCoordinatePosition() throws IOException {
        String requestUrl = ipAddress + "/get/fence/list";
        String response = HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        Log.i("DataLoader", "getCoordinatePosition 响应内容: " + response);
        return response;
    }

    /**192.168.2.16:48080
     * 设备位置上报
     *
     * @return 上报设备的实时地理位置
     */
    public static String getDeviceLocation(String location){
        String requestUrl = ipAddress + "/location/reporting";
        String[] sp = location.split(",");
        pointLat = Double.parseDouble(sp[0]);
        pointLng = Double.parseDouble(sp[1]);
        String response = null;
        try {
            String json = "{\"mac\":\""+deviceId+"\",\"location\":"+location+"\"}";
            response = HttpClient.fetchDataSync(requestUrl, "POST",json);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return response;
    }

    /**
     * 设备上线通知
     *
     * @return 设备开机标记设备为在线状态
     */
    public static String getOnlineMessage(){
        String requestUrl = ipAddress + "/online";
        String response = null;
        try {
            response = HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        Log.i("DataLoader", "getOnlineMessage 响应内容: " + response);
        return response;
    }

    /**
     * 设备下线通知
     *
     * @return 设备关机标记设备为离线状态
     */
    public static String getOfflineMessage() throws IOException {
        String requestUrl = ipAddress + "/offline";
        String response = HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        Log.i("DataLoader", "getOfflineMessage 响应内容: " + response);
        return response;
    }

    /**
     * 获取坡度配置列表
     *
     * @return  获取道路坡度检测配置参数
     */
    public static String getSlopeList() throws IOException {
        String requestUrl = ipAddress + "/get/slope/list";
        String response = HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        Log.i("DataLoader", "getSlopeList 响应内容: " + response);
        return response;
    }

    /**
     * 设备进入电子围栏自动开始任务
     *
     * @return 是否进入电子围栏
     */
    public static boolean getStartTask(){
//        String requestUrl = ipAddress + "/task/start";
//        String response = HttpClient.fetchDataSync(requestUrl, "GET", "?mac=" + deviceId);
        String response = "";
//        double pointLat = Double.parseDouble(sp[0]);
//        double pointLng = Double.parseDouble(sp[1]);

        double pointLat = 2.0;
        double pointLng = 3.0;
        // 按照封闭图形的顺序给坐标点
        Point p1 = new Point(1,1);
        Point p2 = new Point(1,5);
        Point p3 = new Point(5,5);
        Point p4 = new Point(5,1);

        List<Point> fences = new ArrayList<>();
        fences.add(p1);
        fences.add(p2);
        fences.add(p3);
        fences.add(p4);

        //判断是否进入电子围栏
        if(isPointInFence(pointLat,pointLng,fences)){
            // 启动任务
            Log.i("DataLoader", "当前位置在电子围栏中，启动任务: " + String.valueOf(pointLat)+","+String.valueOf(pointLng));
            return true;
        }else {
            Log.i("DataLoader", "当前位置不在电子围栏中，不启动任务: " + String.valueOf(pointLat)+","+String.valueOf(pointLng));
            return false;
        }
    }
    public static boolean isPointInFence(double pointLat, double pointLng, List<Point> fences) {
        // 坐标转换（解决纬度在前经度在后的常见问题）
        double px = pointLng; // X轴对应经度
        double py = pointLat; // Y轴对应纬度

        // 验证多边形有效性
        if (fences.size() < 3) return false;

        // 边界点检查（包含在边上的情况）
        for (int j = 0; j < fences.size(); j++) {
            int next = (j + 1) % fences.size();
            if (isPointOnSegment(new Point(px, py), fences.get(j), fences.get(next))) {
                return true;
            }
        }

        // 射线法核心算法
        boolean inside = false;
        for (int j = 0; j < fences.size(); j++) {
            int next = (j + 1) % fences.size();
            Point a = fences.get(j);
            Point b = fences.get(next);

            // 坐标转换
            double ax = a.getLng(), ay = a.getLat();
            double bx = b.getLng(), by = b.getLat();

            // 排除水平边
            if (Math.abs(ay - by) < 1e-10) continue;

            // 确保射线只与向上或向下的边相交
            if (((ay > py) != (by > py)) &&
                    (px < (bx - ax) * (py - ay) / (by - ay) + ax)) {
                inside = !inside;
            }
        }
        return inside;
    }

    // 增强版的点在线段判断
    private static boolean isPointOnSegment(Point p, Point a, Point b) {
        // 坐标转换
        double px = p.getLng(), py = p.getLat();
        double ax = a.getLng(), ay = a.getLat();
        double bx = b.getLng(), by = b.getLat();

        // 快速排除
        if (px < Math.min(ax, bx) - 1e-10 || px > Math.max(ax, bx) + 1e-10) return false;
        if (py < Math.min(ay, by) - 1e-10 || py > Math.max(ay, by) + 1e-10) return false;

        // 方向向量验证
        double cross = (px - ax) * (by - ay) - (py - ay) * (bx - ax);
        return Math.abs(cross) < 1e-10;
    }

    // 更健壮的Point类
    public static class Point {
        private final double lat;  // 纬度(Y)
        private final double lng;  // 经度(X)

        public Point(double lng, double lat) {
            this.lat = lat;
            this.lng = lng;
        }

        public double getLat() { return lat; }
        public double getLng() { return lng; }
    }



}
