package com.autonavi.smarteye.util.imgupload;

import android.os.Build;
import android.os.FileObserver;
import android.util.Log;

import net.lingala.zip4j.ZipFile;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.stream.Stream;


/**
 * 监听文件夹。
 * 如果文件夹中的文件数量在一定时间内不再变化，则进行上传操作。
 */
public class OriginalFolderMonitor {

    private final static String TAG = "OriginalFolderMonitor";
    // 被监听的父文件夹。original/20250101/ ；original是父文件夹
    private final String monitorFolderPath = "/storage/ext4_sdcard/Android/data/img/original";
    private final File parentFolder;
    // 存储每个子文件夹最后更新的时间戳
    private final Map<String, Long> folderLastUpdateMap = new ConcurrentHashMap<>();
    // 存储每个正在被监控子文件夹对应的 FileObserver 对象
    private final Map<String, FileObserver> observers = new HashMap<>();
    private final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();

    public OriginalFolderMonitor() {
        this.parentFolder = new File(monitorFolderPath);
        startMonitoring();
    }

    // 开启监控
    public void startMonitoring() {
        // 删除关机前最后一个可能未打包的文件夹。因为该文件夹可能带来打包出错
        deleteLargestNameFolder(monitorFolderPath);

        // 定时扫描不活跃的文件夹是否可以打包
        scheduler.scheduleWithFixedDelay(this::checkInactiveFolders, 5, 5, TimeUnit.SECONDS);

        // 扫描一次子文件夹，为每个子文件夹创建FileObserver来监听新增文件。确保 app 启动后就立刻进入监听状态
        scanAndAttachObservers();

        // 动态监测新增或删除的子文件夹，更新被监听子文件夹列表
        scheduler.scheduleWithFixedDelay(this::scanAndAttachObservers, 5, 5, TimeUnit.SECONDS);
    }

    // 停止监控
    public void stopMonitoring() {
        for (FileObserver observer : observers.values()) {
            observer.stopWatching();
        }
        observers.clear();
        scheduler.shutdown();
    }

    // 动态检测新/删文件夹，并监听
    private void scanAndAttachObservers() {
        File[] folders = parentFolder.listFiles(File::isDirectory);
        if (folders == null) return;

        Set<String> currentFolders = new HashSet<>();

        for (File folder : folders) {
            String path = folder.getAbsolutePath();
            currentFolders.add(path);

            if (!observers.containsKey(path)) {
                // 追踪子文件夹
                attachObserver(path);
            }
        }

        // 清理已删除的文件夹
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            observers.keySet().removeIf(path -> {
                if (!currentFolders.contains(path)) {
                    observers.get(path).stopWatching();
                    folderLastUpdateMap.remove(path);
                    observers.remove(path);
                    return true;
                }
                return false;
            });
        }
    }

    // 文件夹追踪
    private void attachObserver(String folderPath) {
        FileObserver observer = new FileObserver(folderPath, FileObserver.CREATE) {
            @Override
            public void onEvent(int event, String path) {
                if (event == FileObserver.CREATE && path != null) {
                    // 更新最后修改时间
                    folderLastUpdateMap.put(folderPath, System.currentTimeMillis());
                }
            }
        };
        observer.startWatching();
        observers.put(folderPath, observer);
        folderLastUpdateMap.put(folderPath, System.currentTimeMillis()); // 初始化
    }

    // 检查文件夹是否沉寂后触发上传
    private void checkInactiveFolders() {
        long now = System.currentTimeMillis();
        for (Map.Entry<String, Long> entry : folderLastUpdateMap.entrySet()) {
            String path = entry.getKey();
            long lastUpdate = entry.getValue();
            if (now - lastUpdate >= 2000) { // 2秒内无更新
                processFolder(path);
            }
        }
    }

    private void processFolder(String path) {
        boolean success = false;
        File folder = new File(path);
        File zipFile = null;
        try {
            zipFile = zipFolder(folder);
            Log.i(TAG, "ZIP打包完成 " + zipFile.getName());
            success = true;
        } catch (Exception e) {
            success = false;
            if (zipFile != null) {
                zipFile.delete();
            }
            Log.e(TAG, "ZIP打包失败 " + e);
        }
        if (success) {
            deleteRecursively(folder);
            Log.i(TAG, "原始文件夹删除完成 " + folder.getName());
            // 清理对应状态，避免下次继续处理同一文件夹
            folderLastUpdateMap.remove(path);
            FileObserver observer = observers.remove(path);
            if (observer != null) {
                observer.stopWatching();
            }
        }
    }

    /**
     * 使用 zip4j 将整个文件夹压缩成 ZIP 文件
     */
    private File zipFolder(File folder) throws Exception {
        String zipFilePath = folder.getParent() + File.separator + folder.getName() + ".zip";
        ZipFile zipFile = new ZipFile(zipFilePath);

        File[] files = folder.listFiles();
        if (files != null) {
            for (File file : files) {
                zipFile.addFile(file);
            }
        }
        return new File(zipFilePath);
    }

    /**
     * 递归删除文件或文件夹及其内容
     */
    private static void deleteRecursively(File fileOrDirectory) {
        if (fileOrDirectory.isDirectory()) {
            File[] children = fileOrDirectory.listFiles();
            if (children != null) {
                for (File child : children) {
                    deleteRecursively(child);
                }
            }
        }
        fileOrDirectory.delete();
    }

    /**
     * 删除名字最大的文件夹
     *
     * @param parentDirPath
     */
    private static void deleteLargestNameFolder(String parentDirPath) {
        Path parent = Paths.get(parentDirPath);

        // 1. 列出所有子目录并找出名字最大的
        try (Stream<Path> stream = Files.list(parent)) {
            Optional<Path> maxDir = stream
                    .filter(Files::isDirectory)
                    .max(Comparator.comparing(path -> path.getFileName().toString()));

            if (maxDir.isPresent()) {
                File file = maxDir.get().toFile();
                deleteRecursively(file);
            }

        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

}

