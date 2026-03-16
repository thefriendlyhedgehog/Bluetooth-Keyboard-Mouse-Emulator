# Changelog

All notable changes to this project are documented here.

---

## [3.0] – 2026-03-16 – ADV Edition

### Fixed
- **Landscape Gyro X-axis reversed**: Tilt left/right now correctly moves cursor left/right.

### Added
- **SD Card Configuration**: Place `/KBMousePuter/kb_mouse_config.txt` on your SD card to customise settings (e.g. `timeout=5`).
- **Full State Persistence (NVS)**: Remembers both connection type (USB/BT) *and* input mode (Keyboard/Mouse) across reboots.
- **Default Auto-Boot Timeout**: Reduced to 2 seconds (was 3s in v2.8).

---

## [2.9] – 2026-03-16

### Added
- SD card initialisation using `/KBMousePuter/kb_mouse_config.txt`.
- Persistence for `mouseMode` (Keyboard vs Mouse) via NVS.

### Changed
- Default auto-boot timeout reduced to 2 seconds.

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
