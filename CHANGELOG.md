# Changelog

All notable changes to this project are documented here.

---

## [2.9.2] – 2026-03-16 – ADV Edition

### Fixed
- **Stuck keys / Auto-pressing (F11)**: Fixed a race condition where the hotkey handler in `main.cpp` would consume the "key change" event before the USB/Bluetooth HID dispatchers could see it, causing key releases to be missed.
- **Keyboard Reliability**: Unified keyboard state detection to ensure consistent report delivery across all modes.

---

## [2.9.1] – 2026-03-16 – ADV Edition

### Fixed
- **Keyboard not working**: Key-release (zero) reports were never sent to the host because the `isChange()` guard blocked the empty-key state from being forwarded. Fixed both USB and BLE keyboard paths.
- **Landscape Gyro X-axis reversed**: Tilt left/right now correctly moves cursor left/right.
- **Gyro Y-axis inversion**: Up/Down in both Landscape and Portrait now matches physical tilt.

### Added
- **SD Card Configuration**: Place `/KBMousePuter/kb_mouse_config.txt` on your SD card to customise settings (e.g. `timeout=5`).
- **Full State Persistence (NVS)**: Remembers both connection type (USB/BT) *and* input mode (Keyboard/Mouse) across reboots.
- **Auto-Boot Timeout**: 2-second countdown on boot (SD-configurable).
- **Gyro Deadzone** (`0.05f`) to prevent cursor drift.
- **Portrait Gyro Mode** (`Fn + P`): One-handed vertical use. `Q` = left click, `1` = right click, `Ctrl` = re-center.
- **`Fn + G`** hotkey to toggle Gyro mode.
- **F-Key Layer**: `Fn + 1–0` → F1–F10, `-` → F11, `=` → F12.
- **Ergonomic mouse clicks**: `` ` `` = left click, `Backspace` = right click.

---

## [2.8] – 2026-03-16

### Added
- State persistence for USB/Bluetooth mode selection using ESP32 NVS (`Preferences`).
- 3-second auto-boot countdown on the mode selection screen.

---

## [2.7.1] – 2026-03-16

### Fixed
- **Gyro Y-axis inversion**: Up/Down movement in both Landscape and Portrait modes now correctly maps to physical tilt direction.

---

## [2.7] – 2026-03-15

### Added
- **Gyro Deadzone** (`GYRO_DEADZONE = 0.05f`) to prevent cursor jitter from small movements.
- **`Fn + G`** hotkey to toggle Gyro mode without leaving the keyboard.
- **Portrait Gyro Mode** (`Fn + P`): Hold the Cardputer vertically. `Q` = left click, `1` = right click, `Ctrl` = re-center.
- **Gyro mode UI indicator**: Blue "GYRO" label and portrait indicator when enabled.

### Fixed
- Gyro X-axis direction corrected for Landscape mode.
- UI border no longer clipped when clearing the GYRO text overlay.

---

## [2.6] – 2026-03-14

### Changed
- Remapped mouse left/right click to `` ` `` (backtick) and `Backspace` for ergonomic one-handed use.

---

## [2.5] – 2026-03-13

### Added
- Arrow-key and gyro-based mouse motion in USB mode.
- F-key layer via `Fn` modifier (`Fn + 1` → F1, etc.).

---

## [2.0] – 2026-03-12

### Added
- USB HID support alongside Bluetooth HID.
- Mode selection screen on boot (USB vs Bluetooth).
- Combined keyboard + mouse HID descriptors for both USB and BLE.

---

## [1.0] – Initial Release

- Bluetooth HID keyboard and mouse emulation.
- Arrow keys for mouse movement, GO button to toggle modes.
