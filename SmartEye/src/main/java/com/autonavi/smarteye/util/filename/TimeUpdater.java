package com.autonavi.smarteye.util.filename;

import android.util.Log;

import com.autonavi.smarteye.config.Config;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * 定时更新 currentFormattedTime,用于定时创建文件夹的命名
 */
public class TimeUpdater {

    private static final String TAG = "TimeUpdater";
    private static final SimpleDateFormat FORMATTER = new SimpleDateFormat("yyyyMMdd-HHmm", Locale.getDefault());

    private volatile String currentFormattedTime;
    private final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();

    // 私有构造方法，确保外部不能创建实例
    private TimeUpdater() {
        updateTime(); // 初始化时立即更新
        startScheduledTask(); // 启动定时任务
    }

    // 静态内部类方式实现线程安全单例
    private static class Holder {
        private static final TimeUpdater INSTANCE = new TimeUpdater();
    }

    public static TimeUpdater getInstance() {
        return Holder.INSTANCE;
    }

    private void updateTime() {
        currentFormattedTime = FORMATTER.format(new Date());
        Log.d(TAG, "更新时间：" + currentFormattedTime);
    }

    private void startScheduledTask() {
        // 间隔指定时间创建文件夹
        scheduler.scheduleAtFixedRate(this::updateTime, 0, 1, TimeUnit.MINUTES);
    }

    public String getCurrentFormattedTime() {
        return currentFormattedTime;
    }

    public void stop() {
        scheduler.shutdown();
    }
}
