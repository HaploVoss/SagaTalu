#pragma once
#include <WebServer.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <SDCardManager.h>
#include <SdFat.h>

namespace sumi {

class WifiTransfer {
 public:
  bool begin(const char* ssid, const char* password);
  void stop();
  void handleClient();

  bool isRunning() const { return running_; }
  bool isApMode() const { return apMode_; }
  String ipAddress() const { return ip_; }

 private:
  WebServer* server_ = nullptr;
  bool running_ = false;
  bool apMode_ = false;
  String ip_;

  void setupRoutes();
  void handleRoot();
  void handleUpload();
  void handleFileList();
  void handleDelete();
  void handleMkdir();
  void handleDownload();
  void handleExportBg();
};

}  // namespace sumi