package com.autonavi.smarteye;

import com.autonavi.smarteye.config.Config;
import com.autonavi.smarteye.service.DataLoader;
import com.autonavi.smarteye.util.filename.TimeUpdater;

import java.io.IOException;

public class SmartEye {

    public static native void start();

    /**
     * C++ 层调用 Java
     */
    public static String cxxCallJava(String actionType, String json) {
        String response = null;
        switch (actionType) {
            case "getCurrentFormattedTime":
                response = TimeUpdater.getInstance().getCurrentFormattedTime();
                break;
            case "getAppSDCardPath":
                response = "/storage/ext4_sdcard/Android/data/"; // SDCardTool.getAppOuterDirectory();
                break;
            case "GetEth0Address":
                response = NetUtils.getEth0Address("usb0");
                break;
            case "sendDeviceLocation":
                response = DataLoader.sendDeviceLocation(json);
                break;
            case "getDiseaseLibrary":
                response = DataLoader.getDiseaseLibrary();
                break;
            case "getTaskStatus":
                response = Config.taskStatus;
                break;
            default: {
            }
        }
        if (response == null) {
            response = "";
        }
        return response;
    }


}
