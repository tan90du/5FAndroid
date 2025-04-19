package com.autonavi.smarteye;

import android.util.Log;

import com.autonavi.smarteye.config.Config;
import com.autonavi.smarteye.service.DataLoader;
import com.autonavi.smarteye.util.imgupload.MarkedFolderMonitor;
import com.autonavi.smarteye.util.imgupload.OriginalFolderMonitor;
import com.autonavi.smarteye.util.imgupload.OriginalUploadManager;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.concurrent.*;

public class ActiveTasks {

    private static final String TAG = "ActiveTasks";

    // 1. 普通异步任务池（固定大小，或用 cachedPool）
    private static final ExecutorService executor = Executors.newFixedThreadPool(5);

    // 2. 定时/周期性任务池
    private static final ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(2);

    public static void startAllTasks() {
        // 发送上线通知并监听任务状态（一次性启动 + 内部循环）
        executor.submit(ActiveTasks::onlineAndFenceMonitor);

        // 原始文件夹监控，打包ZIP压缩包
//        executor.submit(OriginalFolderMonitor::new);

        // 上传ZIP压缩包
//        executor.submit(OriginalUploadManager::new);

        // 标记图像文件夹监控
//        executor.submit(MarkedFolderMonitor::new);

        // （如果还需要定时/延迟执行的任务，比如定期上传、定期同步等，可用 scheduler）
        // 例如：每隔 5 分钟重新发送一次坡度计算
//        scheduler.scheduleAtFixedRate(() -> {
//            String jsonStr = "{...}";
//            DataLoader.calculateAndSendSlope(jsonStr);
//        }, 0, 5, TimeUnit.MINUTES);

        // JVM 关闭时优雅停池
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            shutdownAndAwaitTermination(executor);
            shutdownAndAwaitTermination(scheduler);
        }));
    }

    /**
     * 上线通知和电子围栏检测
     */
    private static void onlineAndFenceMonitor() {
        // 发送上线通知，直到成功
        String response;
        JSONObject json;
        do {
            response = DataLoader.sendOnlineMessage();
            try {
                json = new JSONObject(response);
            } catch (JSONException e) {
                Log.e(TAG, "上线通知 JSON 解析错误", e);
                continue;
            }
            if ("4001001".equals(json.optString("code"))) {
                Log.i(TAG, "设备上线成功");
                break;
            }
            Log.i(TAG, "设备上线失败，重试中...");
        } while (true);

        // 监听电子围栏进入/离开
//        while (true) {
//            if (DataLoader.isAccessFence()) {
        Config.taskStatus = "open";
        DataLoader.taskStart();
        Log.i(TAG, "任务开始");
//            } else {
//                Config.taskStatus = "pause";
//                DataLoader.taskPause();
//                Log.i(TAG, "任务暂停");
//            }
//            try {
//                TimeUnit.SECONDS.sleep(2);
//            } catch (InterruptedException e) {
//                throw new RuntimeException(e);
//            }
//        }
    }

    private static void shutdownAndAwaitTermination(ExecutorService pool) {
        pool.shutdown(); // 停止接受新任务
        try {
            // 等待现有任务结束
            if (!pool.awaitTermination(30, TimeUnit.SECONDS)) {
                pool.shutdownNow(); // 强制取消
                if (!pool.awaitTermination(30, TimeUnit.SECONDS))
                    Log.e(TAG, "线程池未能终止");
            }
        } catch (InterruptedException ie) {
            pool.shutdownNow();
            Thread.currentThread().interrupt();
        }
    }
}
