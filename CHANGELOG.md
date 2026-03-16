# Changelog

All notable changes to this project are documented here.

---

## [2.11.6] – 2026-03-16 – ADV Edition

### Fixed
- **Hotkey Consistency**: Fixed Ctrl-based hotkeys (`Ctrl+P` for Portrait, `Ctrl+G` for Gyro) to work reliably using `keysState()`.
- **UI Restoration**: Moved USB and Bluetooth status indicators back to the top of the screen in Landscape mode.

### Changed
- **Landscape Layout**: Restored dual horizontal status boxes at the top and side-by-side panel widths.

---

## [2.11.5] – 2026-03-16 – ADV Edition

### Changed
- **Repo Cleanup**: Migrated all `.bin` files out of the git repository to GitHub Releases.
- **Repository Optimization**: Updated `.gitignore` to prevent binary tracking.

---

## [2.11.4] – 2026-03-16 – ADV Edition

### Fixed
- **Credits**: Specifically credited **Geo (tp53.geo)** as the original creator of the foundation.
- **Version Parity**: Bumped version to v2.11.4 across all files.

---

## [2.11.3] – 2026-03-16 – ADV Edition

### Fixed
- **Credits**: Added credit for the original project and clarified that this is an enhanced version.
- **Version Parity**: Bumped version to v2.11.3 across all files.

---

## [2.11.2] – 2026-03-16 – ADV Edition

### Fixed
- **G0 Consistency**: Fixed an issue where G0 short-press (KB/Mouse toggle) would sometimes fail.
- **Ctrl Hotkeys**: Fixed `Ctrl+P` and `Ctrl+G` modifiers to work correctly without needing the Fn key.

### Changed
- **Landscape UI Parity**: Removed the "G0 switch" text box from Landscape mode.
- **Dual Landscape Indicators**: Added a dual USB/BT status sidebar to the right side of the screen in Landscape mode (matching Portrait behavior).
- **Layout Adjustments**: Realigned panels and icons in Landscape mode to accommodate the new sidebar.

---

## [2.11.1] – 2026-03-16 – ADV Edition

### Changed
- **G0 Overhaul**: Short press now toggles Keyboard/Mouse (standard), and **Long Press (>500ms)** toggles USB/Bluetooth mode.
- **Ctrl Toggles**: Configuration shortcuts (Portrait, Gyro) now use the **Ctrl** key instead of Fn (`Ctrl+P` and `Ctrl+G`).
- **Hotkey Cleanup**: Removed `Fn+M` mode swap shortcut in favor of the easier G0 long-press.

---

## [2.11.0] – 2026-03-16 – ADV Edition

### Added
- **Dynamic Mode Swap (Fn+M)**: Switch between USB and Bluetooth in real-time without needing to reboot the device.
- **Dual Connection Sidebar**: Portrait mode now shows both USB and BT status simultaneously in the right-side bar.
- **Refined Portrait Header**: Tightened text spacing to ensure "M5 ADV - KB-Mouse" fits on a single line in Portrait mode.

---

## [2.10.1] – 2026-03-16 – ADV Edition

### Added
- **Portrait UI Refinements**: Main panels now shrink and center for better ergonomics.
- **Vertical Connection Indicator**: USB/BT status is now a sleek vertical side-bar in Portrait mode.
- **Updated Branding**: Header changed to **"M5 ADV - KB-Mouse"** across all modes.

---

## [2.10.0] – 2026-03-16 – ADV Edition

### Added
- **Portrait UI Overhaul**: Full screen rotation (90 degrees) and adaptive layouts.
- **Adaptive Layouts**: UI panels now intelligently stack vertically in Portrait mode and side-by-side in Landscape.
- **Orientation-Aware Icons**: Key icons and text now re-center and resize based on screen orientation.

---

## [2.9.5] – 2026-03-16 – ADV Edition

### Added
- **User Guide**: Comprehensive documentation for all keyboard layers, mouse controls, and SD settings in `README.md`.
- **Key Discovery**: Documented `Opt` key as **GUI (Win/Cmd)** and all `Fn` shortcuts.

---

## [2.9.4] – 2026-03-16 – ADV Edition

### Added
- **SD Auto-Setup**: Now automatically creates the `/KBMousePuter/` folder and `kb_mouse_config.txt` on the SD card if missing.
- **Boot Optimization**: Default auto-boot timeout lowered to **1 second**.

---

## [2.9.3] – 2026-03-16 – ADV Edition

### Added
- **Fn Layer Hotkeys (Case Layout)**: Added `Fn` combinations matching the physical labels on the Cardputer case:
  - `Fn + ` ` -> **Escape**
  - `Fn + ;` -> **Up Arrow**
  - `Fn + .` -> **Down Arrow**
  - `Fn + ,` -> **Left Arrow**
  - `Fn + /` -> **Right Arrow**

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
