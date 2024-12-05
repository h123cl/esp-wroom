#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

// WiFi 连接信息
const char* ssid = "hcl1";
const char* password = "12345678";

// 创建Web服务器
WebServer server(80);

// 上传页面HTML
const char uploadPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>ESP32文件上传</title>
  </head>
  <body>
    <h1>选择.nc文件并上传</h1>
    <form method="POST" action="/upload" enctype="multipart/form-data">
      <input type="file" name="file">
      <input type="submit" value="Upload">
    </form>
  </body>
</html>
)rawliteral";

// 处理上传页面
void handleRoot() {
  server.send(200, "text/html", uploadPage);
}

// 处理文件上传
void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    // 打开文件
    String filename = "/" + upload.filename;
    if (!SPIFFS.begin(true)) {
      server.send(500, "text/plain", "文件系统初始化失败,请联系作者");
      return;
    }
    File file = SPIFFS.open(filename, FILE_WRITE);
    if (!file) {
      server.send(500, "text/plain", "文件打开失败");
      return;
    }
    file.close();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    // 写入文件
    File file = SPIFFS.open("/" + upload.filename, FILE_APPEND);
    if (file) {
      file.write(upload.buf, upload.currentSize);
      file.close();
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    // 上传完成
    server.send(200, "text/plain", "上传完成");
  }
}

void setup() {
  Serial.begin(115200);

  // 连接WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println(WiFi.localIP());

  // 配置SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIffs ERROR");
    return;
  }

  // 配置Web服务器
  server.on("/", handleRoot);
  server.on("/upload", HTTP_POST, []() {
    server.send(200);
  }, handleFileUpload);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
