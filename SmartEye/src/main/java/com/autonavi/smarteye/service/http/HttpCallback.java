package com.autonavi.smarteye.service.http;

public interface HttpCallback {
    void onSuccess(String result);

    void onFailure(Exception e);
}
