package com.autonavi.smarteye;

import com.autonavi.smarteye.service.DataLoader;
import com.autonavi.smarteye.service.DataSender;
import com.autonavi.smarteye.util.SDCardTool;

public class SmartEye {

    public static native void start();

    /**
     * C++ 层调用 Java
     */
    public static String cxxCallJava(String actionType, String json) {
        String response = null;
        switch (actionType) {
            case "getAppSDCardPath":
                response = "/storage/ext4_sdcard/Android/data/";//SDCardTool.getAppOuterDirectory();
                break;
            case "GetEth0Address":
                response = NetUtils.getEth0Address("usb0");
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
