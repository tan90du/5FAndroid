package com.autonavi.smarteye;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class BootReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        if (Intent.ACTION_BOOT_COMPLETED.equals(intent.getAction())) {

            Intent service = new Intent(context, MainActivity.class);
            service.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            context.startActivity(service);
        }
    }
}
