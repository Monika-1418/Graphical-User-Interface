# ISRO TELEMETRY — Image Acquisition Control Panel

A professional PyQt5 GUI application for controlling an image acquisition subsystem on Ubuntu Linux. Features a dark ISRO-style telemetry theme with both **Simulation Mode** (via ImageMagick) and **Real Mode** (external hardware interface).

---

## Project Structure

```
project/
├── gui.py              # Main PyQt5 GUI application
├── run_capture.sh      # Backend shell script (simulation + real mode)
├── images/             # Default output directory (auto-created)
└── README.md           # This file
```

---

## Prerequisites (Ubuntu)

Install all required system packages:

```bash
sudo apt-get update
sudo apt-get install -y python3 python3-pyqt5 imagemagick
```

> **Note:** `imagemagick` is required for Simulation Mode to generate dummy images.

---

## How to Run

```bash
cd /home/vsn/Documents/gui
chmod +x run_capture.sh
python3 gui.py
```

---

## GUI Controls
r
| Control              | Description                                                   |
|----------------------|---------------------------------------------------------------|
| **Save Path**        | Output folder for captured images (default: `images`)         |
| **Gain**             | Analog gain level — select 1, 2, or 4                        |
| **Integration Time** | Shutter / integration duration in milliseconds                |
| **File Name**        | Output filename (without `.png` extension)                    |
| **Simulation Mode**  | Checked = generate dummy image; Unchecked = call real hardware|
| **RUN**              | Execute capture with current settings                        |
| **SHOW IMAGE**       | Manually reload and display the last captured image           |

---

## Workflow

1. Fill in the capture parameters (or use defaults).
2. Ensure **Simulation Mode** is checked (unless real hardware is connected).
3. Click **RUN** — the backend script executes and the image is displayed automatically.
4. Click **SHOW IMAGE** to manually reload the image at any time (enabled after first successful RUN).

---

## Modes

### Simulation Mode
- Uses **ImageMagick** (`convert`) to generate a labeled test image.
- No hardware required — ideal for testing and development.

### Real Mode
- Invokes the external executable `./RPUInterfaceSim` with capture parameters.
- Requires the hardware interface binary to be present in the project directory.

---

## Shell Script Arguments

```bash
bash run_capture.sh <save_path> <gain> <integration_time> <file_name> <mode>
```

| Argument           | Example        |
|--------------------|----------------|
| `save_path`        | `images`       |
| `gain`             | `1` / `2` / `4`|
| `integration_time` | `100`          |
| `file_name`        | `capture_001`  |
| `mode`             | `simulation` or `real` |

---

## Troubleshooting

| Problem                          | Solution                                              |
|----------------------------------|-------------------------------------------------------|
| `ModuleNotFoundError: PyQt5`     | `pip3 install PyQt5` or `sudo apt install python3-pyqt5` |
| Dummy image not generated        | Install ImageMagick: `sudo apt install imagemagick`   |
| Permission denied on script      | Run `chmod +x run_capture.sh`                         |
| SHOW IMAGE button is greyed out  | Click RUN first to enable it                          |

---

## License

Internal use — ISRO Image Acquisition Subsystem.
# gui
