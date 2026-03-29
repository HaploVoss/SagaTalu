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
- **Notes overhauled** — proportional font rendering, portrait orientation, 
  system font consistency, and expanded Bluetooth keyboard device support
- **WiFi file management** — freed memory from removed apps allows full 
  WiFi support. Access via `sagatalu.local` or IP address from any browser 
  on your network. Upload, download, and delete files anywhere on the SD 
  card (full root access, not just `/books`)
- **Access Point fallback** — when home WiFi is unavailable, SagaTalu 
  creates its own hotspot (`SagaTalu-Transfer`) for direct connection
- **WiFi setup via device menu** — configure your home WiFi directly from 
  Settings → WiFi Setup. Scans for networks, enter password with device 
  buttons, saves credentials permanently
- **E-ink refresh suppressed in Notes editor** — no more periodic screen 
  flashes while typing. Clean refresh only on exit
- **Updated branding** — Saga Talu compass star logo on boot and sleep screens, 
  custom home screen background

---

## Flashing SagaTalu

Use the web flasher (requires Chrome, Edge, or Opera):

**[https://haplovoss.github.io/SagaTalu/](https://haplovoss.github.io/SagaTalu/)**

Connect your Xteink X4 via USB-C before clicking Install.

---

## First Boot

1. Go to **Settings → WiFi Setup**
2. Select your home network from the scan list
3. Enter your WiFi password using the device buttons
4. On successful connection, credentials are saved automatically
5. Go to **Settings → Wireless Transfer** to start the file server
6. Access the file manager at `http://sagatalu.local` or the device IP from 
   any browser on the same network

---

## File Management

The web portal gives you full access to your SD card from any browser — 
no USB cable or special software needed. Upload books, notes, or any other 
files, browse folders, download, and delete files directly from the interface.

---

## SD Card Structure

```
SD Card Root/
├── config/
│   └── themes/  ← Home Art BMP images and .theme files
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
