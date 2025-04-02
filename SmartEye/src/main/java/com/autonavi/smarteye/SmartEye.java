package com.autonavi.smarteye;

import com.autonavi.smarteye.service.DataLoader;

public class SmartEye {

    public static native void start();

    /**
     * C++ 层调用 Java
     */
    public static String cxxCallJava(String actionType, String json) {
        String response = null;
        switch (actionType) {
            case "GetEth0Address": response = NetUtils.getEth0Address("usb0");break;
            default: {}
        }
        if (response == null) {
            response = "";
        }
        return response;
    }


}
