# M5 ADV - KB-Mouse – USB & Bluetooth Keyboard/Mouse Emulator

> **Version 2.11.9 – ADV Edition**

**Credit:** This project is a major technical enhancement of the original [Cardputer Keyboard/Mouse Emulator](https://github.com/tp53/Bluetooth-Keyboard-Mouse-Emulator) created by **Geo (tp53.geo)**. This "ADV Edition" extends Geo's professional-grade HID foundation with customized UI logic, persistence, and additional ergonomics.

Turn your M5Stack Cardputer into a full USB **and** Bluetooth HID keyboard *and* mouse — with gyro control, portrait mode, SD-configurable auto-boot, and persistent state across reboots.

---

## 🚀 Why use the ADV Edition?

This repository (ADV Edition) is a major evolution of the foundation created by **Geo**. While the base version provides professional Bluetooth and USB connectivity, the ADV Edition focuses on specialized UI and automated usability features:

| Feature | Original Version (Geo) | **ADV Edition (v2.11.9)** |
|---|---|---|
| **Connectivity** | Bluetooth Only | **USB + Bluetooth** |
| **Mode Swap** | Requires reboot | **G0 Long-Press** |
| **Mouse Control** | Arrow keys only | **Arrow keys + Gyro (Tilt)** |
| **Persistence** | None (starts fresh) | **Saved states (remembers modes)** |
| **Startup** | Manual selection | **Auto-boot with countdown** |
| **Configurability** | Hardcoded | **SD Card overrides** |
| **Ergonomics** | Limited | **Adaptive Portrait UI + Optimized Clicks** |
| **Compatibility** | Basic layers | **Full F-Key mapping (Fn+1-0)** |

### Major Enhancements:
- **Full USB HID Support**: Acts as a physical wired mouse/keyboard.
- **Precision Gyro Control**: High-refresh IMU mapping with deadzone to prevent drift.
- **State Memory**: Powers on exactly how you left it (e.g., if you were using it as a Bluetooth Mouse, it boots as a Bluetooth Mouse).
- **One-Handed Use**: Portrait mode allows you to hold the Cardputer like a remote control.
- **SD Config**: Customize the boot delay via `/KBMousePuter/kb_mouse_config.txt`.

---

## ✨ Features

| Feature | Details |
|---|---|
| **Dual Connection** | Choose USB HID or Bluetooth HID at boot |
| **Keyboard Mode** | Full Cardputer key input forwarded as HID keycodes |
| **Mouse Mode** | Arrow-key or gyro-based cursor control |
| **Gyro Mouse (Landscape)** | Tilt to move — deadzone prevents drift |
| **Gyro Mouse (Portrait)** | Hold the Cardputer vertically for one-handed use |
| **State Persistence** | Remembers your last connection type *and* KB/Mouse mode |
| **Auto-Boot Countdown** | 2-second countdown on startup; auto-launches last state |
| **SD Card Config** | Override timeout and other settings via a text file |
| **F-Key Layer** | `Fn + 1–0` maps to F1–F10, etc. |

---

## 🔌 Installation

### Option A – M5Burner (Easiest)
1. Open [M5Burner](https://docs.m5stack.com/en/download).
2. Search for **"Keyboard Mouse Emulator"** in the **CARDPUTER** section.
3. Click **Burn**.

### Option B – OTA Flash (Recommended)
1. Go to the [Releases](../../releases) section of this repository.
2. Download the latest `.bin` file (e.g., `M5-Keyboard-Mouse-v2.11.4-OTA.bin`).
3. Use the [M5Stack OTA Tool](https://flow.m5stack.com/) or `esptool.py` to flash.

### Option C – Build from Source
```bash
# Requires PlatformIO
pio run --target upload
```

---

## 🎮 Usage

### Boot Screen
On startup, choose **USB** or **Bluetooth** with `.` or `;`:
- **`Enter`** – Confirm selection
- **Auto-launch** – If no key is pressed, the device auto-boots into the last used mode after **2 seconds**

### Input Mode Toggle
| Button | Action |
|---|---|
| **G0 (Side Button)** | **Short Press**: Toggle Keyboard/Mouse |
| **G0 (Side Button)** | **Long Press (>500ms)**: Toggle USB/Bluetooth |
| **Ctrl + G** | Toggle Gyro Tilt Control |
| **Ctrl + P** | Toggle Portrait UI Rotation |
| **Fn + Arrow Keys** | Mouse Scrolling / Horizontal Pan |
| **Fn + G0** | (Legacy) Toggle Gyro |

### Keyboard Mode
Full keyboard input is forwarded as USB/BLE HID keycodes. The layout depends on the host OS configuration.

**F-Key Layer (`Fn` held):**

| Key | Sends |
|---|---|
| `1` – `0` | F1 – F10 |
| `-` | F11 |
| `=` | F12 |
| `Del` | Delete |
| `Enter` | Insert |

### Mouse Mode

**Arrow-key control:**

| Key | Action |
|---|---|
| `` ` `` | Left Click |
| `Backspace` | Right Click |
| `Arrow keys` | Move cursor |
| `Fn + Arrow Up/Down` | Scroll |

**Gyro control (Landscape):**

| Tilt Direction | Action |
|---|---|
| Left / Right | Move cursor left / right |
| Forward / Back | Move cursor up / down |

**Gyro control (Portrait – `Fn + P`):**

| Control | Action |
|---|---|
| Tilt left/right | Move cursor |
| `Q` | Left click |
| `1` | Right click |
| `Ctrl` | Re-center |

---

## ⚙️ SD Card Configuration

Place a file at `/KBMousePuter/kb_mouse_config.txt` on your SD card to customise behaviour:

```text
timeout=5
```

| Key | Default | Description |
|---|---|---|
| `timeout` | `2` | Auto-boot countdown in seconds |

---

## 💾 State Persistence (NVS)

The following are remembered across reboots automatically — no SD card required:

- **Connection type** – USB or Bluetooth
- **Input mode** – Keyboard or Mouse

---

## 🛠️ Building

Requires [PlatformIO](https://platformio.org/). Dependencies are managed via `lib_deps` in `platformio.ini`.

```bash
pio run          # Build
pio run -t upload  # Build and upload via USB
```

---

## 📄 Changelog

See [CHANGELOG.md](CHANGELOG.md).

---

## 📜 License

This project is licensed under the MIT License – see [LICENSE](LICENSE) for details.