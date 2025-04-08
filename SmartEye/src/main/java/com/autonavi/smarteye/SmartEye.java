package com.autonavi.smarteye;

import com.autonavi.smarteye.service.DataLoader;
import com.autonavi.smarteye.service.DataSender;
import com.autonavi.smarteye.util.SDCardTool;

import java.io.IOException;

public class SmartEye {

    public static native void start();

    /**
     * C++ 层调用 Java
     */
    public static String cxxCallJava(String actionType, String json) throws IOException {
        String response = null;
        switch (actionType) {
            case "getAppSDCardPath":
                response = SDCardTool.getAppOuterDirectory();
                break;
            case "GetEth0Address":
                response = NetUtils.getEth0Address("usb0");
                break;
            case "getDeviceLocation":
                response = DataLoader.getDeviceLocation(json);
                break;
            case "getDiseaseLibrary":
                response = DataLoader.getDiseaseLibrary();
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
