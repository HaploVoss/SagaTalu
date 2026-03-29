#include "WifiTransfer.h"
#include "../assets/sumi_home_bg.h"

static const char UPLOAD_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Saga Talu File Transfer</title>
<style>
  body{font-family:sans-serif;max-width:700px;margin:40px auto;padding:0 20px;background:#f5f5f5}
  h1{color:#333;font-size:1.4em;margin-bottom:4px}
  .path{color:#666;font-size:0.9em;margin-bottom:16px;word-break:break-all}
  .path a{color:#007bff;text-decoration:none}
  .path a:hover{text-decoration:underline}
  .drop{border:3px dashed #aaa;border-radius:12px;padding:30px;text-align:center;background:#fff;margin:16px 0;cursor:pointer}
  .drop.over{border-color:#007bff;background:#e8f0ff}
  input[type=file]{display:none}
  button{background:#007bff;color:#fff;border:none;padding:8px 20px;border-radius:6px;cursor:pointer;font-size:0.95em}
  button:hover{background:#0056b3}
  .btn-sm{padding:4px 10px;font-size:0.85em}
  .btn-del{background:#dc3545}.btn-del:hover{background:#a71d2a}
  .btn-dl{background:#28a745;color:#fff;text-decoration:none;padding:4px 10px;border-radius:6px;font-size:0.85em;display:inline-block}
  .btn-dl:hover{background:#1e7e34}
  #status{margin:12px 0;padding:10px;border-radius:6px;display:none}
  .ok{background:#d4edda;color:#155724}
  .err{background:#f8d7da;color:#721c24}
  .prog{background:#d1ecf1;color:#0c5460}
  table{width:100%;border-collapse:collapse;background:#fff;border-radius:8px;overflow:hidden}
  th{background:#e9ecef;padding:10px 12px;text-align:left;font-size:0.9em}
  td{padding:9px 12px;border-top:1px solid #eee;font-size:0.9em}
  .dir{font-weight:bold}
  .dir a{color:#333;text-decoration:none}
  .dir a:hover{color:#007bff}
  .actions{display:flex;gap:6px;justify-content:flex-end}
  .new-folder{margin:8px 0;display:flex;gap:8px}
  .new-folder input{flex:1;padding:6px 10px;border:1px solid #ccc;border-radius:6px;font-size:0.9em}
</style>
</head>
<body>
<h1>Saga Talu File Transfer</h1>
<div class="path" id="pathbar"></div>

<div class="drop" id="drop" onclick="document.getElementById('fileinput').click()">
  <p>Drop files here or click to browse<br><small>Uploads go to current folder</small></p>
  <input type="file" id="fileinput" multiple>
</div>
<div id="status"></div>

<div class="new-folder">
  <input type="text" id="foldername" placeholder="New folder name">
  <button onclick="createFolder()">Create Folder</button>
</div>

<table>
  <thead><tr><th>Name</th><th>Size</th><th></th></tr></thead>
  <tbody id="filelist"></tbody>
</table>

<script>
let currentPath = '/';

function showStatus(msg, cls) {
  const s = document.getElementById('status');
  s.textContent = msg;
  s.className = cls;
  s.style.display = 'block';
}

function formatSize(bytes) {
  if (bytes === 0) return '-';
  if (bytes < 1024) return bytes + ' B';
  if (bytes < 1048576) return Math.round(bytes/1024) + ' KB';
  return (bytes/1048576).toFixed(1) + ' MB';
}

function renderPath(path) {
  const bar = document.getElementById('pathbar');
  const parts = path.split('/').filter(Boolean);
  let html = '<a href="#" onclick="navigate(\'/\')">root</a>';
  let built = '';
  parts.forEach(p => {
    built += '/' + p;
    const b = built;
    html += ' / <a href="#" onclick="navigate(\'' + b + '\')">' + p + '</a>';
  });
  bar.innerHTML = html;
}

async function navigate(path) {
  currentPath = path;
  renderPath(path);
  const r = await fetch('/list?path=' + encodeURIComponent(path));
  const items = await r.json();
  const tbody = document.getElementById('filelist');
  tbody.innerHTML = '';

  // Parent directory link
  if (path !== '/') {
    const parent = path.substring(0, path.lastIndexOf('/')) || '/';
    const tr = document.createElement('tr');
    tr.innerHTML = '<td class="dir" colspan="2"><a href="#" onclick="navigate(\'' + parent + '\')">.. (up)</a></td><td></td>';
    tbody.appendChild(tr);
  }

  items.forEach(item => {
    const tr = document.createElement('tr');
    if (item.isDir) {
      const childPath = (currentPath === '/' ? '' : currentPath) + '/' + item.name;
      tr.innerHTML = '<td class="dir" colspan="2"><a href="#" onclick="navigate(\'' + childPath + '\')">[+] ' + item.name + '</a></td><td></td>';
    } else {
      tr.innerHTML =
        '<td>' + item.name + '</td>' +
        '<td>' + formatSize(item.size) + '</td>' +
        '<td class="actions"><a class="btn-sm btn-dl" href="/download?path='+encodeURIComponent((currentPath==='/'?'':currentPath)+'/'+item.name)+'" download>Download</a> <button class="btn-sm btn-del" onclick="delFile(\''+item.name+'\')">Delete</button></td>';
    }
    tbody.appendChild(tr);
  });
}

async function uploadFiles(files) {
  for (const f of files) {
    showStatus('Uploading ' + f.name + '...', 'prog');
    const fd = new FormData();
    fd.append('file', f, f.name);
    fd.append('path', currentPath);
    try {
      const r = await fetch('/upload?path=' + encodeURIComponent(currentPath), {method:'POST', body:fd});
      const t = await r.text();
      if (r.ok) showStatus('Done: ' + f.name, 'ok');
      else showStatus('Error: ' + t, 'err');
    } catch(e) {
      showStatus('Upload failed: ' + e, 'err');
    }
  }
  navigate(currentPath);
}

async function delFile(name) {
  const path = (currentPath === '/' ? '' : currentPath) + '/' + name;
  if (!confirm('Delete ' + name + '?')) return;
  await fetch('/delete?path=' + encodeURIComponent(path), {method:'POST'});
  navigate(currentPath);
}

async function createFolder() {
  const name = document.getElementById('foldername').value.trim();
  if (!name) return;
  await fetch('/mkdir?path=' + encodeURIComponent(currentPath) + '&name=' + encodeURIComponent(name), {method:'POST'});
  document.getElementById('foldername').value = '';
  navigate(currentPath);
}

const drop = document.getElementById('drop');
drop.addEventListener('dragover', e => { e.preventDefault(); drop.classList.add('over'); });
drop.addEventListener('dragleave', () => drop.classList.remove('over'));
drop.addEventListener('drop', e => { e.preventDefault(); drop.classList.remove('over'); uploadFiles(e.dataTransfer.files); });
document.getElementById('fileinput').addEventListener('change', e => uploadFiles(e.target.files));

navigate('/');
</script>
</body>
</html>
)rawhtml";

namespace sumi {

bool WifiTransfer::begin(const char* ssid, const char* password) {
  if (ssid && ssid[0] != '\0') {
    Serial.printf("[WIFI] Connecting to %s\n", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
      delay(250);
    }
    if (WiFi.status() == WL_CONNECTED) {
      ip_ = WiFi.localIP().toString();
      apMode_ = false;
      Serial.printf("[WIFI] Connected. IP: %s\n", ip_.c_str());
      if (MDNS.begin("sagatalu")) {
        MDNS.addService("http", "tcp", 80);
        Serial.println("[WIFI] mDNS started: http://sagatalu.local");
      }
    }
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WIFI] Starting AP: SagaTalu-Transfer");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("SUMI-Transfer", "sagatalupass");
    ip_ = WiFi.softAPIP().toString();
    apMode_ = true;
    Serial.printf("[WIFI] AP started. IP: %s\n", ip_.c_str());
  }

  server_ = new WebServer(80);
  setupRoutes();
  server_->begin();
  running_ = true;
  Serial.printf("[WIFI] Server started at http://%s\n", ip_.c_str());
  return !apMode_;
}

void WifiTransfer::stop() {
  if (server_) {
    server_->stop();
    delete server_;
    server_ = nullptr;
  }
  MDNS.end();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  running_ = false;
  Serial.println("[WIFI] Transfer server stopped");
}

void WifiTransfer::handleClient() {
  if (server_) server_->handleClient();
}

void WifiTransfer::setupRoutes() {
  server_->on("/", HTTP_GET, [this]() { handleRoot(); });
  server_->on("/list", HTTP_GET, [this]() { handleFileList(); });
  server_->on("/download", HTTP_GET, [this]() { handleDownload(); });
  server_->on("/export-bg", HTTP_POST, [this]() { handleExportBg(); });
  server_->on("/delete", HTTP_POST, [this]() { handleDelete(); });
  server_->on("/mkdir", HTTP_POST, [this]() { handleMkdir(); });
  server_->on("/upload", HTTP_POST,
    [this]() { server_->send(200, "text/plain", "OK"); },
    [this]() { handleUpload(); }
  );
  auto portalOk = [this]() { server_->send(200, "text/plain", "OK"); };
  server_->on("/generate_204", HTTP_GET, portalOk);
  server_->on("/gen_204", HTTP_GET, portalOk);
  server_->on("/connectivity-check.html", HTTP_GET, portalOk);
  server_->on("/ncsi.txt", HTTP_GET, portalOk);
  server_->on("/connecttest.txt", HTTP_GET, portalOk);
  server_->on("/hotspot-detect.html", HTTP_GET, portalOk);
  server_->onNotFound([this]() {
    server_->sendHeader("Location", "http://192.168.4.1", true);
    server_->send(302, "text/plain", "");
  });
}

void WifiTransfer::handleRoot() {
  server_->send_P(200, "text/html", UPLOAD_HTML);
}

void WifiTransfer::handleUpload() {
  HTTPUpload& upload = server_->upload();
  static FsFile uploadFile;
  static String uploadPath;

  if (upload.status == UPLOAD_FILE_START) {
    // Get target path from query parameter
    uploadPath = "/";
    if (server_->hasArg("path")) {
      uploadPath = server_->arg("path");
    }
    if (!uploadPath.endsWith("/")) uploadPath += "/";
    String filePath = uploadPath + upload.filename;
    Serial.printf("[WIFI] Upload start: %s\n", filePath.c_str());
    SdMan.remove(filePath.c_str());
    if (!SdMan.openFileForWrite("WIFI", filePath.c_str(), uploadFile)) {
      Serial.printf("[WIFI] Failed to open for write: %s\n", filePath.c_str());
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
      Serial.printf("[WIFI] Upload complete: %s (%u bytes)\n",
                    upload.filename.c_str(), upload.totalSize);
    }
  } else if (upload.status == UPLOAD_FILE_ABORTED) {
    if (uploadFile) uploadFile.close();
    Serial.println("[WIFI] Upload aborted");
  }
}

void WifiTransfer::handleFileList() {
  String path = "/";
  if (server_->hasArg("path")) path = server_->arg("path");

  String json = "[";
  bool first = true;
  FsFile dir = SdMan.open(path.c_str());
  if (dir && dir.isDirectory()) {
    FsFile entry = dir.openNextFile();
    while (entry) {
      char nameBuf[64];
      entry.getName(nameBuf, sizeof(nameBuf));
      String name = String(nameBuf);
      // Skip hidden system folders
      if (!name.startsWith(".")) {
        if (!first) json += ",";
        if (entry.isDirectory()) {
          json += "{\"name\":\"" + name + "\",\"isDir\":true,\"size\":0}";
        } else {
          json += "{\"name\":\"" + name + "\",\"isDir\":false,\"size\":" + entry.fileSize() + "}";
        }
        first = false;
      }
      entry.close();
      entry = dir.openNextFile();
    }
    dir.close();
  }
  json += "]";
  server_->send(200, "application/json", json);
}

void WifiTransfer::handleDelete() {
  if (!server_->hasArg("path")) {
    server_->send(400, "text/plain", "Missing path");
    return;
  }
  String path = server_->arg("path");
  SdMan.remove(path.c_str());
  server_->send(200, "text/plain", "Deleted");
}

void WifiTransfer::handleMkdir() {
  if (!server_->hasArg("path") || !server_->hasArg("name")) {
    server_->send(400, "text/plain", "Missing path or name");
    return;
  }
  String path = server_->arg("path");
  String name = server_->arg("name");
  if (!path.endsWith("/")) path += "/";
  String full = path + name;
  SdMan.mkdir(full.c_str());
  server_->send(200, "text/plain", "Created");
}
void WifiTransfer::handleDownload() {
  if (!server_->hasArg("path")) {
    server_->send(400, "text/plain", "Missing path");
    return;
  }
  String path = server_->arg("path");
  FsFile file = SdMan.open(path.c_str());
  if (!file || file.isDirectory()) {
    server_->send(404, "text/plain", "File not found");
    return;
  }

  // Get filename from path for Content-Disposition header
  String filename = path;
  int slash = filename.lastIndexOf('/');
  if (slash >= 0) filename = filename.substring(slash + 1);

  // Determine MIME type
  String mime = "application/octet-stream";
  if (filename.endsWith(".txt")) mime = "text/plain";
  else if (filename.endsWith(".epub")) mime = "application/epub+zip";
  else if (filename.endsWith(".bmp")) mime = "image/bmp";
  else if (filename.endsWith(".html")) mime = "text/html";

  server_->sendHeader("Content-Disposition",
    "attachment; filename=\"" + filename + "\"");
  server_->sendHeader("Content-Length", String(file.fileSize()));
  server_->setContentLength(file.fileSize());
  server_->send(200, mime, "");

  // Stream file in chunks
  uint8_t buf[512];
  WiFiClient client = server_->client();
  while (file.available() && client.connected()) {
    int bytes = file.read(buf, sizeof(buf));
    if (bytes > 0) client.write(buf, bytes);
  }
  file.close();
}
void WifiTransfer::handleExportBg() {
  // Write SumiHomeBg (800x480 1-bit raw framebuffer) as a
  // proper 1-bit BMP in portrait orientation (480x800)
  // so it opens correctly in image editors

  const char* outPath = "/config/themes/default_bg_export.bmp";

  // BMP file for 480x800 1-bit image
  // Row size must be padded to 4-byte boundary
  // 480 pixels / 8 = 60 bytes per row, already divisible by 4
  const int W = 480, H = 800;
  const int rowBytes = W / 8;  // 60 bytes
  const int pixelDataSize = rowBytes * H;  // 48000 bytes
  const int fileSize = 54 + 8 + pixelDataSize;  // header + palette + pixels

  FsFile file;
  if (!SdMan.openFileForWrite("WIFI", outPath, file)) {
    server_->send(500, "text/plain", "Failed to create file");
    return;
  }

  // BMP file header (14 bytes)
  uint8_t header[54 + 8] = {0};
  // Signature
  header[0] = 'B'; header[1] = 'M';
  // File size
  header[2] = fileSize & 0xFF;
  header[3] = (fileSize >> 8) & 0xFF;
  header[4] = (fileSize >> 16) & 0xFF;
  header[5] = (fileSize >> 24) & 0xFF;
  // Pixel data offset = 54 + 8 (header + palette)
  header[10] = 62;
  // DIB header size = 40
  header[14] = 40;
  // Width = 480
  header[18] = W & 0xFF;
  header[19] = (W >> 8) & 0xFF;
  // Height = 800 (positive = bottom-up)
  header[22] = H & 0xFF;
  header[23] = (H >> 8) & 0xFF;
  // Color planes = 1
  header[26] = 1;
  // Bits per pixel = 1
  header[28] = 1;
  // Image size
  header[34] = pixelDataSize & 0xFF;
  header[35] = (pixelDataSize >> 8) & 0xFF;
  header[36] = (pixelDataSize >> 16) & 0xFF;
  header[37] = (pixelDataSize >> 24) & 0xFF;
  // Colors in table = 2
  header[46] = 2;
  // Palette: color 0 = white (0xFF,0xFF,0xFF), color 1 = black (0,0,0)
  // BMP palette entries are BGRA
  header[54] = 0xFF; header[55] = 0xFF; header[56] = 0xFF; header[57] = 0x00;  // white
  header[58] = 0x00; header[59] = 0x00; header[60] = 0x00; header[61] = 0x00;  // black

  file.write(header, sizeof(header));

  // The source data is 800x480 landscape (native display orientation)
  // We need to rotate 90° CCW to get 480x800 portrait
  // Source: row=y (0-479), col=x (0-799), 1 bit per pixel
  // Source byte: SumiHomeBg[y * 100 + x/8], bit: 7-(x%8)
  // Dest: 480x800 portrait, stored bottom-to-top (standard BMP)
  // Dest row r (from bottom) = portrait row (799-r)
  // Portrait pixel (px, py) comes from landscape pixel (py, 479-px)

  uint8_t rowBuf[rowBytes];
  // Write BMP rows bottom to top
  for (int r = H - 1; r >= 0; r--) {
    // r is the portrait row from top (0=top, 799=bottom)
    // BMP bottom-to-top means we write r=799 first, r=0 last
    memset(rowBuf, 0, rowBytes);
    for (int px = 0; px < W; px++) {
      // Portrait pixel (px, r) comes from landscape pixel (r, 479-px)
      int srcY = 479 - px;  // landscape row
      int srcX = r;          // landscape col
      // Source byte in 800-wide landscape (100 bytes per row)
      int srcByte = srcY * 100 + srcX / 8;
      int srcBit = 7 - (srcX % 8);
      uint8_t srcVal = pgm_read_byte(&SumiHomeBg[srcByte]);
      uint8_t bit = (srcVal >> srcBit) & 1;
      // Write to dest row
      int dstBit = 7 - (px % 8);
      if (bit) rowBuf[px / 8] |= (1 << dstBit);
    }
    file.write(rowBuf, rowBytes);
  }

  file.close();
  server_->send(200, "text/plain", "Exported to /config/themes/default_bg_export.bmp");
}
}  // namespace sumi