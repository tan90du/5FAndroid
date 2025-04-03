package com.autonavi.smarteye.util;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.text.TextUtils;
import android.util.Log;

import com.autonavi.smarteye.config.Config;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class SDCardTool {
    private static Context context = Config.getContext();
    private static final int REQUEST_CODE_PERMISSIONS = 101;
    private static final String TAG = "SDCardWriter";

    public static boolean checkPermissions() {
        return ContextCompat.checkSelfPermission(context, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED &&
                ContextCompat.checkSelfPermission(context, Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED;
    }

    public static void requestPermissions() {
        ActivityCompat.requestPermissions((Activity) context, new String[]{
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.READ_EXTERNAL_STORAGE
        }, REQUEST_CODE_PERMISSIONS);
    }


    public static String getAppOuterDirectory() {
        String filesPath = null;
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())
                || !Environment.isExternalStorageRemovable()) {

            File[] externalFileDirs = context.getExternalFilesDirs("");

            boolean isOuterFilePathExist = false;
            // 判断是否存在外置Sd存储卡
            try {
                for (File externalFile : externalFileDirs) {
                    if (externalFile != null && externalFile.exists()) {
                        String path = externalFile.getPath();
                        if (!TextUtils.isEmpty(path) && path.contains("ext4_sdcard")) {
                            filesPath = path;
                            isOuterFilePathExist = true;
                            break;
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
                filesPath = "";
            }

            // 如果外置Sd卡异常，返回的存储路径为""
            if (!isOuterFilePathExist) {
                filesPath = "";
            }

            Log.d(TAG, "机身外存储卡路径：" + filesPath);
        } else {
            filesPath = context.getFilesDir().getPath();
            Log.d(TAG, "机身外存储卡路径：" + filesPath);
        }
        return filesPath + "/"; // /storage/ext4_sdcard/Android/data/your.package.name/files/
    }

    public static void writeToSDCard(String filePath, String fileName) {
        // Get the directory for the app's private external storage
//        File dir = context.getExternalFilesDir(null); // This will give you a directory like /storage/emulated/0/Android/data/your.package.name/files/
//        File dir = new File(getAppOuterDirectory()); // This will give you a directory like /storage/ext4_sdcard/Android/data/your.package.name/files
        File dir = new File(filePath);
        if (dir != null && !dir.exists()) {
            dir.mkdirs(); // Create the directory if it doesn't exist
        }

        File file = new File(dir, fileName);
        Log.d(TAG, "File path: " + file.getAbsolutePath());
        try (FileOutputStream fos = new FileOutputStream(file)) {
            String data = "Hello, SD Card!";
            fos.write(data.getBytes());
            Log.i(TAG, "File written: " + file.getAbsolutePath());
        } catch (IOException e) {
            Log.i(TAG, "Error writing to SD card " + file.getAbsolutePath() + e);
        }
    }

}
