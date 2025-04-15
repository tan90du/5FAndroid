package com.autonavi.smarteye.util.imgupload;

import android.os.Build;
import android.util.Log;

import com.autonavi.smarteye.config.Config;
import com.autonavi.smarteye.service.http.HttpClient;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.squareup.okhttp.MediaType;
import com.squareup.okhttp.MultipartBuilder;
import com.squareup.okhttp.Request;
import com.squareup.okhttp.RequestBody;
import com.squareup.okhttp.Response;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * 上传原始，标注图片及标注信息到Saas平台
 */
public class MarkedUploadManager {

    private final static String TAG = "MarkedImageUpload";

    /**
     * 批量上传整个文件夹
     *
     * @param folder
     * @param commonUploadUrl
     * @param recognitionUploadUrl
     */
    public static void uploadFolderDirectly(File folder, String commonUploadUrl, String recognitionUploadUrl) {
        try {
            List<File> files = getAllFiles(folder);
            JsonObject markedInfo = null;
            File originalImage = null;
            List<File> markedImages = new ArrayList<>();

            // 遍历文件夹中的所有文件，根据规则区分原始图和标记图
            for (File file : files) {
                String[] str = file.getName().split("\\.");
                String postfix = str[str.length - 1]; // 后缀
                if ("json".equals(postfix)) { // marked_info.json 配置文件
                    FileReader reader = new FileReader(file.getAbsolutePath());
                    markedInfo = JsonParser.parseReader(reader).getAsJsonObject();
                } else { // 图片文件
                    if (isOriginalImage(file)) {
                        originalImage = file;
                    } else {
                        markedImages.add(file);
                    }
                }
            }

            if (originalImage == null) {
                Log.e(TAG, "未找到原始图片");
            }
            if (markedImages.isEmpty()) {
                Log.e(TAG, "未找到标记图片");
            }

            // 先上传原始图片和第一张标记图片（非定妆照），获取服务器返回的id
            File firstMarkedImage = null;
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                firstMarkedImage = markedImages.stream()
                        .filter(f -> !f.getName().contains("finalPhoto"))
                        .findFirst()
                        .orElse(null);
            }
            if (firstMarkedImage != null) {
                markedImages.remove(firstMarkedImage);
            }
            String[] firstMarkedImageName = firstMarkedImage.getName().split(".jpg");
            String reqVO = buildFirstUploadReqVO(firstMarkedImageName[0], markedInfo);
            String response = uploadFirstImage(commonUploadUrl, originalImage, firstMarkedImage, reqVO);
            // 从response中解析出id
            JSONObject jsonObj = new JSONObject(response);
            int code = jsonObj.getInt("code");
            if (code != 0) {
                Log.e(TAG, "请求失败: " + jsonObj.getString("message"));
            } else {
                Log.i(TAG, "获取id-成功上传图片-原图 " + originalImage.getAbsolutePath());
                Log.i(TAG, "获取id-成功上传图片-标记图 " + firstMarkedImage.getAbsolutePath());
            }
            int id = jsonObj.getInt("data");

            // 根据返回的 id，继续上传剩余的标记图片
            for (File markedImage : markedImages) {
                String[] remainMarkedImageName = markedImage.getName().split(".jpg");
                reqVO = buildRemainUploadReqVO(remainMarkedImageName[0], id, markedInfo);
                uploadRemainImage(recognitionUploadUrl, markedImage, reqVO);
                Log.i(TAG, "图片上传成功 " + markedImage.getAbsolutePath());
            }

            // 上传完成后清理本地文件
            deleteFolderRecursively(folder);

        } catch (Exception e) {
            Log.e(TAG, "标记图片文件夹上传失败：" + e);
        }
    }

    // 图片上传
    public static String uploadFirstImage(String url, File image, File highlightImage, String reqVO) throws IOException {
        // 定义媒体类型，根据图片实际格式调整
        MediaType MEDIA_TYPE_IMAGE = MediaType.parse("image/jpeg");

        // 使用 MultipartBuilder 构建 multipart/form-data 请求体
        MultipartBuilder builder = new MultipartBuilder()
                .type(MultipartBuilder.FORM)
                .addFormDataPart("image", image.getName(),
                        RequestBody.create(MEDIA_TYPE_IMAGE, image))
                .addFormDataPart("highlightImage", highlightImage.getName(),
                        RequestBody.create(MEDIA_TYPE_IMAGE, highlightImage));

        // 解析 reqVO JSON 字符串，并将其中每个字段添加到表单参数中
        try {
            JSONObject jsonObject = new JSONObject(reqVO);
            // 迭代所有 JSON 键值
            Iterator<String> keys = jsonObject.keys();
            while (keys.hasNext()) {
                String key = keys.next();
                Object value = jsonObject.get(key);
                // 如果字段为数组或对象类型，转换为字符串形式上传
                String valueStr = value.toString();
                // 添加到请求参数中
                builder.addFormDataPart(key, valueStr);
            }
        } catch (JSONException e) {
            throw new IOException("解析上传参数 JSON 失败: " + e);
        }


        RequestBody requestBody = builder.build();

        Request request = new Request.Builder()
                .url(url)
                .post(requestBody)
                .build();

        Response response = HttpClient.getClient().newCall(request).execute();
        if (!response.isSuccessful()) {
            Log.e(TAG, "图片上传未接收到服务器返回的Id " + response);
        }
        return response.body().string();
    }

    public static void uploadRemainImage(String url, File highlightImage, String reqVO) throws IOException {
        // 定义媒体类型，根据图片实际格式调整
        MediaType MEDIA_TYPE_IMAGE = MediaType.parse("image/jpeg");

        // 使用 MultipartBuilder 构建 multipart/form-data 请求体
        MultipartBuilder builder = new MultipartBuilder()
                .type(MultipartBuilder.FORM)
                .addFormDataPart("highlightImage", highlightImage.getName(),
                        RequestBody.create(MEDIA_TYPE_IMAGE, highlightImage));

        // 解析 reqVO JSON 字符串，并将其中每个字段添加到表单参数中
        try {
            JSONObject jsonObject = new JSONObject(reqVO);
            // 迭代所有 JSON 键值
            Iterator<String> keys = jsonObject.keys();
            while (keys.hasNext()) {
                String key = keys.next();
                Object value = jsonObject.get(key);
                // 如果字段为数组或对象类型，转换为字符串形式上传
                String valueStr = value.toString();
                // 添加到请求参数中
                builder.addFormDataPart(key, valueStr);
            }
        } catch (JSONException e) {
            throw new IOException("解析上传参数 JSON 失败: " + e);
        }

        RequestBody requestBody = builder.build();

        Request request = new Request.Builder()
                .url(url)
                .post(requestBody)
                .build();

        HttpClient.getClient().newCall(request).execute();
    }

    private static boolean isOriginalImage(File file) {
        String[] str1 = file.getName().split("_");
        String[] str2 = str1[1].split("\\.");
        return "original".equals(str2[0]);
    }

    private static String buildFirstUploadReqVO(String filename, JsonObject markedInfo) {
        JsonObject info = markedInfo.getAsJsonObject(filename);
        String diseaseType = info.get("diseaseType").getAsString();
        String location = info.get("location").getAsString();
        String direction = info.get("direction").getAsString();
        String uniqueId = info.get("uniqueId").getAsString();
        String equipmentMac = Config.deviceId;
        String position = info.getAsJsonArray("position").toString();

        if(info.has("laneNumber")) {
            Log.i(TAG, "车道道路线 " + info.get("laneNumber").getAsString());
        }

        String reqVO = "{"
                + "\"diseaseType\": \"" + diseaseType + "\","
                + "\"location\": \"" + location + "\","
                + "\"direction\": \"" + direction + "\","
                + "\"uniqueId\": \"" + uniqueId + "\","
                + "\"equipmentMac\": \"" + equipmentMac + "\","
                + "\"positionList\": " + position
                + "}";

        return reqVO;
    }

    private static String buildRemainUploadReqVO(String filename, int id, JsonObject markedInfo) {
        JsonObject info = markedInfo.getAsJsonObject(filename);
        String uniqueId = info.get("uniqueId").getAsString();
        String diseaseType = info.get("diseaseType").getAsString();
        String location = info.get("location").getAsString();
        String quantity = info.get("quantity").getAsString();
        boolean isFinalPhoto = false;
        String position = info.getAsJsonArray("position").toString();

        if(info.has("laneNumber")) {
            Log.i(TAG, "车道道路线 " + info.get("laneNumber").getAsString());
        }

        // filename: timestamp-longitude-latitude-altitude-speed-finalPhoto_marked
        String[] str1 = filename.split("_");
        String[] str2 = str1[0].split("-");
        if ("finalPhoto".equals(str2[str2.length - 1])) { // 定妆照
            isFinalPhoto = true;
        }

        String reqVO = "{"
                + "\"id\": \"" + id + "\","
                + "\"uniqueId\": \"" + uniqueId + "\","
                + "\"diseaseType\": \"" + diseaseType + "\","
                + "\"location\": \"" + location + "\","
                + "\"quantity\": \"" + quantity + "\","
                + "\"isFinalPhoto\": \"" + isFinalPhoto + "\","
                + "\"position\": " + position
                + "}";

        return reqVO;
    }

    /**
     * 递归获取文件夹中所有文件
     *
     * @param folder
     * @return
     */
    private static List<File> getAllFiles(File folder) {
        List<File> files = new ArrayList<>();
        File[] listFiles = folder.listFiles();
        if (listFiles != null) {
            for (File file : listFiles) {
                if (file.isDirectory()) {
                    files.addAll(getAllFiles(file));
                } else {
                    files.add(file);
                }
            }
        }
        return files;
    }

    /**
     * 删除文件夹
     *
     * @param fileOrDir
     */
    private static void deleteFolderRecursively(File fileOrDir) {
        if (fileOrDir.isDirectory()) {
            for (File child : fileOrDir.listFiles()) {
                deleteFolderRecursively(child);
            }
        }
        fileOrDir.delete();
    }

}
