package com.autonavi.smarteye.service.http;

import com.squareup.okhttp.Callback;
import com.squareup.okhttp.ConnectionPool;
import com.squareup.okhttp.MediaType;
import com.squareup.okhttp.OkHttpClient;
import com.squareup.okhttp.Request;
import com.squareup.okhttp.RequestBody;
import com.squareup.okhttp.Response;

import org.json.JSONException;

import java.io.IOException;
import java.util.concurrent.TimeUnit;

public class HttpClient {
    // 创建一个全局的 OkHttpClient 实例
    private static final OkHttpClient client;

    static {
        // 配置连接池
        ConnectionPool connectionPool = new ConnectionPool(10, 5 * 60 * 1000); // 10 个连接，5 分钟
        client = new OkHttpClient();
        client.setConnectionPool(connectionPool);
        client.setConnectTimeout(10, TimeUnit.SECONDS); // 连接超时
        client.setReadTimeout(10, TimeUnit.SECONDS);    // 读取超时
        client.setWriteTimeout(10, TimeUnit.SECONDS);   // 写入超时
    }

    // 同步请求 Synchronization
    public static String fetchDataSync(String url, String requestType, String param) throws IOException {
        Request request = null;

        if ("GET" == requestType) { // 传递参数示例：String param = "?param1=1&param2=str";
            String requestUrl = url + param;
            request = new Request.Builder()
                    .url(requestUrl)
                    .build();
        } else if ("POST" == requestType) { // 传递参数示例： String param = "{\"mac\": \"123\", \"location\": \"113.4214,32.142\"}";
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            RequestBody body = RequestBody.create(JSON, param);

            request = new Request.Builder()
                    .url(url)
                    .post(body)
                    .build();
        } else {
            throw new RuntimeException("暂未添加 GET 和 POST 以外的请求类型");
        }

        Response response = client.newCall(request).execute(); // 连接池复用
        if (!response.isSuccessful()) {
            throw new IOException("Unexpected response " + response);
        }

        return response.toString();
    }

    // 异步请求 Asynchronization
    public static void fetchDataAsync(String url, String requestType, String param, final HttpCallback callback) {
        Request request = null;

        if ("GET" == requestType) { // 传递参数示例：String param = "?param1=1&param2=str";
            String requestUrl = url + param;
            request = new Request.Builder()
                    .url(requestUrl)
                    .build();
        } else if ("POST" == requestType) { // 传递参数示例： String param = "{\"mac\": \"123\", \"location\": \"113.4214,32.142\"}";
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            RequestBody body = RequestBody.create(JSON, param);

            request = new Request.Builder()
                    .url(url)
                    .post(body)
                    .build();
        } else {
            throw new RuntimeException("暂未添加 GET 和 POST 以外的请求类型");
        }

        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Request request, IOException e) {
                // 失败回调
                callback.onFailure(e);
            }

            @Override
            public void onResponse(Response response) throws IOException {
                if (!response.isSuccessful()) {
                    callback.onFailure(new IOException("Unexpected code " + response));
                    return;
                }
                // 成功回调
                try {
                    callback.onSuccess(response.body().string());
                } catch (JSONException e) {
                    throw new RuntimeException(e);
                }
            }
        });
    }
}
