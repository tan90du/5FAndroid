package com.autonavi.smarteye;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.LinkAddress;
import android.net.LinkProperties;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.util.Log;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;

/**
 * Created by jinghao on 15/11/17.
 */
public class NetUtils {

    public static String getEth0Address(String interfaceName) {
        try {
            Enumeration<NetworkInterface> networkInterfaces = NetworkInterface.getNetworkInterfaces();
            if(networkInterfaces == null) {
                return "";
            }
            List<NetworkInterface> interfaces = Collections.list(
                    networkInterfaces);


            for (NetworkInterface intf : interfaces) {
//                Log.i("interfaceName", "" + intf.getName());
                if (interfaceName != null) {
                    if (!intf.getName().equalsIgnoreCase(interfaceName)) continue;
                }
//                byte[] mac = intf.getHardwareAddress();
//                if (mac==null) return "";
                Enumeration<InetAddress> inetAddr = intf.getInetAddresses();
                if(inetAddr != null) {
                    while (inetAddr.hasMoreElements()) {
                        StringBuilder buf = new StringBuilder();
                        InetAddress element = inetAddr.nextElement();

                        byte[] ip = element.getAddress();
                        for (int idx = 0; idx < ip.length; idx++) {
                            buf.append(String.format("%s:", ip[idx] & 0xFF));
                        }
                        if (buf.length() > 0) {
                            buf.deleteCharAt(buf.length() - 1);
                        }
                        Log.i("getEth0Address", "" + buf.toString());
                        if (buf.toString().equals("192:168:65:55")) {
                            return buf.toString();
                        }
                    }
                }


                return "";
            }
        } catch (Exception ex) {
            Log.i("getEth0Address", "" + ex.getLocalizedMessage());

        } // for now eat exceptions
        return "";
        /*try {
            // this is so Linux hack
            return loadFileAsString("/sys/class/net/" +interfaceName + "/address").toUpperCase().trim();
        } catch (IOException ex) {
            return null;
        }*/
    }
}
