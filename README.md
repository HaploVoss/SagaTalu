# SagaTalu

**Focused firmware for the Xteink X4.**

SagaTalu is custom firmware for the Xteink X4 e-ink device, forked from 
SUMI. Where SUMI is a broad platform with games, apps, and a plugin system, 
SagaTalu strips that back to a focused writing and reading experience with 
robust wireless file management.

---

## What's Different from SUMI

- **Notes-first** — Notes is the primary app, at the top of the main menu
- **Games and Apps removed** — all plugin/games overhead stripped out, 
  freeing significant memory
- **Notes overhauled** — proportional font rendering, accurate cursor 
  positioning, system font consistency, and expanded Bluetooth keyboard 
  device support
- **Portrait orientation** — display now runs in portrait mode in Notes
- **WiFi file management** — freed memory from removed apps allows full 
  WiFi support. Access via `sagatalu.local` or IP address from any browser 
  on your network. Upload, download, and delete files anywhere on the SD 
  card (full root access, not just `/books`)
- **Access Point fallback** — when home WiFi is unavailable, SagaTalu 
  creates its own hotspot for direct connection
- **Updated branding** — new transition logo and sleep screen

---

## Flashing SagaTalu

*(Web flasher coming soon)*

### Manual Flash

1. Download the latest `SagaTalu.bin` from [Releases](../../releases)
2. Go to [https://web.esptool.io/](https://web.esptool.io/)
3. Click **Connect** and select the USB serial port
4. Set flash address to `0x0`
5. Select the downloaded `.bin` file
6. Click **Program**

---

## First Boot

1. SagaTalu creates a WiFi hotspot called `SagaTalu-Setup-XXXX`
2. Connect to it from your phone or computer (no password)
3. Open `http://192.168.4.1` in your browser
4. Connect SagaTalu to your home WiFi
5. Once connected, access the portal at `http://sagatalu.local` or the 
   device's IP address from any device on the same network

---

## File Management

The web portal gives you full access to your SD card from any browser — 
no USB cable or special software needed. Upload books, notes, or any other 
files, browse folders, and delete files directly from the interface.

---

## SD Card Structure
```
SD Card Root/
├── config/      ← Themes / "Home Art" images
├── images/      ← BMP/JPEG images
├── notes/       ← Text notes
└── books/       ← EPUB files
```

---

## Hardware

- **Xteink X4** e-ink device
- **MicroSD card** required (8GB+ recommended)
- **USB-C** for flashing
- **Bluetooth keyboard** supported in Notes (broad device compatibility)

---

## Built On

SagaTalu is built on the work of the SUMI, Papyrix, and CrossPoint Reader 
projects. See [ATTRIBUTIONS.md](ATTRIBUTIONS.md) for full credits.

---

## License

MIT — see [LICENSE](LICENSE)