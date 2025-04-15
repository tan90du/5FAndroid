package com.autonavi.smarteye.util.imgupload;

import static com.autonavi.smarteye.util.imgupload.MarkedUploadManager.uploadFolderDirectly;

import android.os.Build;

import com.autonavi.smarteye.config.Config;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class MarkedFolderMonitor {

    // 被监听的父文件夹。marked/xxx/ ；marked是父文件夹
    private static final String monitorFolderPath = "/storage/ext4_sdcard/Android/data/img/marked";
    private final File parentFolder;
    private final String commonUploadUrl = Config.serverIpAddress + "/upload";
    private final String recognitionUploadUrl = Config.serverIpAddress + "/recognition/callback";
    private final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();

    public MarkedFolderMonitor() {
        this.parentFolder = new File(monitorFolderPath);
        startMonitoring();
    }

    // 启动定时任务。定时扫描一次父文件夹
    public void startMonitoring() {
        scheduler.scheduleWithFixedDelay(() -> {
            List<File> remainingFolders = getSortedSubfoldersExcludingMax();
            for (File folder : remainingFolders) {
                // 将子文件夹上传
                uploadFolderDirectly(folder, commonUploadUrl, recognitionUploadUrl);
            }
        }, 2, 2, TimeUnit.SECONDS);
    }

    // 关闭定时任务
    public void stopMonitoring() {
        scheduler.shutdown();
    }

    // 获取父文件夹中所有子文件夹，排序后去掉名字最大的文件夹
    private List<File> getSortedSubfoldersExcludingMax() {
        File[] folders = parentFolder.listFiles(File::isDirectory);
        if (folders == null || folders.length == 0) {
            return Collections.emptyList();
        }
        List<File> folderList = new ArrayList<>(Arrays.asList(folders));
        // 按文件夹名字排序，假设名字是可以按字典顺序排序的（如 20250401）
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            folderList.sort(Comparator.comparing(File::getName));
        }
        // 去掉名字最大的文件夹（最后一个）。最新生成的文件夹可能文件写入未完成，禁止上传。
        folderList.remove(folderList.size() - 1);
        return folderList;
    }
}
