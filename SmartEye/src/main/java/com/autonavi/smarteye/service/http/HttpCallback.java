package com.autonavi.smarteye.service.http;

import org.json.JSONException;

public interface HttpCallback {
    void onSuccess(String result) throws JSONException;

    void onFailure(Exception e);
}
