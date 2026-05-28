import sys
import os
import subprocess
from datetime import datetime
from PyQt5.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout,
                             QHBoxLayout, QLabel, QLineEdit, QComboBox,
                             QPushButton, QMessageBox, QFrame, QFileDialog,
                             QSizePolicy, QPlainTextEdit, QGraphicsOpacityEffect)
from PyQt5.QtGui import QPixmap, QFont, QColor
from PyQt5.QtCore import Qt, QTimer, QPropertyAnimation, QEasingCurve


# ============================================================
#  ISRO TELEMETRY CONSOLE — Image Acquisition Control Panel
# ============================================================

class ImageAcquisitionApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("ISRO TELEMETRY | Image Acquisition Subsystem")
        self.resize(1100, 780)

        # Animation state
        self._anim_dots = 0
        self._anim_timer = QTimer(self)
        self._anim_timer.timeout.connect(self._update_activity_animation)

        # SpaceWire blink state
        self._spw_blink_on = True
        self._spw_blink_count = 0

        # ======================================================
        #  GLOBAL STYLESHEET — ISRO Dark Theme
        # ======================================================
        self.setStyleSheet("""
            QWidget {
                background-color: #0B0E14;
                color: #E0E6ED;
                font-family: 'Consolas', 'Courier New', monospace;
                font-size: 14px;
            }
            QLabel {
                font-weight: bold;
                color: #8C9BAB;
                margin-top: 2px;
                letter-spacing: 1px;
                background: transparent;
                border: none;
            }

            /* --- INPUT FIELDS --- */
            QLineEdit, QComboBox {
                padding: 8px;
                border: 1px solid #233446;
                border-radius: 4px;
                background-color: #05070A;
                color: #00E5FF;
                font-weight: bold;
            }
            QLineEdit:focus, QComboBox:focus {
                border: 1px solid #00E5FF;
                background-color: #0B101A;
            }

            /* --- BUTTONS --- */
            QPushButton {
                background-color: #FF5722;
                color: white;
                border: 1px solid #E64A19;
                padding: 12px;
                border-radius: 4px;
                font-weight: 900;
                font-size: 16px;
                letter-spacing: 2px;
            }
            QPushButton:hover {
                background-color: #FF6E40;
                border: 1px solid #FF8A65;
            }
            QPushButton:pressed {
                background-color: #D84315;
            }
            QPushButton:disabled {
                background-color: #3B2019;
                color: #5C6370;
                border: 1px solid #2A1712;
            }

            /* --- LOG PANEL --- */
            QPlainTextEdit {
                background-color: #05070A;
                color: #00E5FF;
                border: 1px solid #233446;
                border-radius: 4px;
                font-size: 12px;
                padding: 6px;
            }

            /* --- MESSAGE BOX --- */
            QMessageBox {
                background-color: #151B24;
            }
            QMessageBox QLabel {
                color: #E0E6ED;
            }
        """)

        # ======================================================
        #  ROOT LAYOUT
        # ======================================================
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        root_layout = QVBoxLayout()
        root_layout.setContentsMargins(16, 10, 16, 10)
        root_layout.setSpacing(8)
        central_widget.setLayout(root_layout)

        # ======================================================
        #  HEADER BAR  (Title + Mission Clock)
        # ======================================================
        header_bar = QHBoxLayout()
        header_bar.setContentsMargins(0, 0, 0, 0)

        app_title = QLabel("ISRO TELEMETRY \u2014 IMAGE ACQUISITION SUBSYSTEM")
        app_title.setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #4A5C6D; "
            "letter-spacing: 3px; padding: 0;"
        )
        header_bar.addWidget(app_title)

        header_bar.addStretch()

        # --- MISSION TIME CLOCK ---
        self.clock_label = QLabel("MISSION TIME: 00:00:00")
        self.clock_label.setStyleSheet(
            "font-size: 13px; font-weight: 900; color: #00E5FF; "
            "letter-spacing: 2px; padding: 4px 12px; "
            "border: 1px solid #233446; border-radius: 3px; "
            "background-color: #05070A;"
        )
        self.clock_label.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        header_bar.addWidget(self.clock_label)

        self._clock_timer = QTimer(self)
        self._clock_timer.timeout.connect(self._update_clock)
        self._clock_timer.start(1000)
        self._update_clock()

        root_layout.addLayout(header_bar)

        # ======================================================
        #  STATUS BAR
        # ======================================================
        self.status_frame = QFrame()
        self.status_frame.setFixedHeight(36)
        self.status_frame.setStyleSheet(
            "QFrame { background-color: #0D1520; border: 1px solid #233446; "
            "border-radius: 3px; }"
        )
        status_layout = QHBoxLayout()
        status_layout.setContentsMargins(12, 0, 12, 0)
        self.status_frame.setLayout(status_layout)

        self.status_label = QLabel("\u25cf  STATUS: WAITING")
        self.status_label.setStyleSheet(
            "font-size: 13px; font-weight: 900; color: #42A5F5; "
            "letter-spacing: 2px; border: none;"
        )
        status_layout.addWidget(self.status_label)
        status_layout.addStretch()

        # --- SpaceWire link indicator ---
        self.spw_indicator = QLabel("● SPW LINK: CHECKING")
        self.spw_indicator.setStyleSheet(
            "font-size: 13px; font-weight: 900; color: #555; "
            "letter-spacing: 2px; border: none;"
        )
        status_layout.addWidget(self.spw_indicator)

        # Poll SpaceWire device every 1 second, blink when connected
        self._spw_timer = QTimer(self)
        self._spw_timer.timeout.connect(self._update_spw_indicator)
        self._spw_timer.start(200)   # fast tick, but ON stays visible for 1.8s, OFF for 0.2s
        self._update_spw_indicator()

        root_layout.addWidget(self.status_frame)

        # ======================================================
        #  CONTENT AREA  (Controls | Display | Log)
        # ======================================================
        content_layout = QHBoxLayout()
        content_layout.setSpacing(12)
        root_layout.addLayout(content_layout, stretch=1)

        # ------ LEFT PANEL: CONTROLS ------
        control_panel = QFrame()
        control_panel.setFrameShape(QFrame.StyledPanel)
        control_panel.setFrameShadow(QFrame.Raised)
        control_panel.setStyleSheet(
            "QFrame { background-color: #151B24; border-radius: 4px; "
            "border: 1px solid #233446; padding: 10px; }"
        )
        control_layout = QVBoxLayout()
        control_panel.setLayout(control_layout)
        control_panel.setFixedWidth(280)

        title_label = QLabel("SYSTEM CONTROLS")
        title_label.setStyleSheet(
            "font-size: 16px; font-weight: bold; margin-bottom: 8px; "
            "color: #FF5722; letter-spacing: 2px;"
        )
        control_layout.addWidget(title_label)

        # Camera Mode
        control_layout.addWidget(QLabel("CAMERA MODE:"))
        self.mode_input = QComboBox()
        self.mode_input.addItem("Select Mode")
        self.mode_input.addItems(["PDS (Proximity Docking Sensor)", "RS (Remote Sensing)"])
        self.mode_input.currentTextChanged.connect(lambda val: print(f"[GUI] Camera Mode : {val}"))
        control_layout.addWidget(self.mode_input)

        # Save Path
        control_layout.addWidget(QLabel("SAVE PATH:"))
        path_row = QHBoxLayout()
        path_row.setSpacing(6)
        self.path_input = QLineEdit()
        self.path_input.setPlaceholderText("Enter save path")
        self.path_input.textChanged.connect(lambda val: print(f"[GUI] Save Path   : {val}"))
        path_row.addWidget(self.path_input)

        self.browse_button = QPushButton("Browse")
        self.browse_button.setCursor(Qt.PointingHandCursor)
        self.browse_button.setFixedWidth(70)
        self.browse_button.setStyleSheet(
            "QPushButton { background-color: #233446; color: #00E5FF; "
            "border: 1px solid #2E4A62; padding: 8px 4px; font-size: 12px; "
            "font-weight: bold; letter-spacing: 1px; }"
            "QPushButton:hover { background-color: #2E4A62; border: 1px solid #00E5FF; }"
            "QPushButton:pressed { background-color: #1A2836; }"
        )
        self.browse_button.clicked.connect(self.select_save_path)
        path_row.addWidget(self.browse_button)

        control_layout.addLayout(path_row)

        # Gain
        control_layout.addWidget(QLabel("GAIN:"))
        self.gain_input = QComboBox()
        self.gain_input.addItem("Select Gain")
        self.gain_input.addItems(["1", "2", "4"])
        self.gain_input.setCurrentIndex(0)
        self.gain_input.currentTextChanged.connect(lambda val: print(f"[GUI] Gain        : {val}"))
        control_layout.addWidget(self.gain_input)

        # Integration Time
        control_layout.addWidget(QLabel("INTEGRATION TIME (ms):"))
        self.time_input = QLineEdit()
        self.time_input.setPlaceholderText("Enter integration time (ms)")
        self.time_input.textChanged.connect(lambda val: print(f"[GUI] Integ. Time : {val} ms"))
        control_layout.addWidget(self.time_input)

        # File Name
        control_layout.addWidget(QLabel("FILE NAME:"))
        self.file_input = QLineEdit()
        self.file_input.setPlaceholderText("Enter file name")
        self.file_input.textChanged.connect(lambda val: print(f"[GUI] File Name   : {val}"))
        control_layout.addWidget(self.file_input)

        # --- RUN BUTTON ---
        self.run_button = QPushButton("\u25b6  RUN CAPTURE")
        self.run_button.setCursor(Qt.PointingHandCursor)
        self.run_button.clicked.connect(self.run_capture)
        control_layout.addWidget(self.run_button)

        # --- SHOW IMAGE BUTTON ---
        self.show_button = QPushButton("\u25a0  SHOW IMAGE")
        self.show_button.setCursor(Qt.PointingHandCursor)
        self.show_button.setEnabled(False)
        self.show_button.setStyleSheet(
            "QPushButton { background-color: #1565C0; border: 1px solid #0D47A1; }"
            "QPushButton:hover { background-color: #1E88E5; border: 1px solid #42A5F5; }"
            "QPushButton:pressed { background-color: #0D47A1; }"
            "QPushButton:disabled { background-color: #0D1B2A; color: #5C6370; border: 1px solid #172535; }"
        )
        self.show_button.clicked.connect(self.show_image)
        control_layout.addWidget(self.show_button)

        control_layout.addStretch()

        content_layout.addWidget(control_panel)

        # ------ CENTER PANEL: IMAGE DISPLAY ------
        display_panel = QFrame()
        display_panel.setFrameShape(QFrame.StyledPanel)
        display_panel.setStyleSheet(
            "QFrame { background-color: #000000; border-radius: 4px; "
            "border: 1px solid #233446; }"
        )
        display_layout = QVBoxLayout()
        display_layout.setContentsMargins(0, 0, 0, 0)
        display_panel.setLayout(display_layout)

        self.image_label = QLabel("[ NO DATA FEED ]\nSIGNAL READY \u2014 AWAITING COMMAND")
        self.image_label.setAlignment(Qt.AlignCenter)
        self.image_label.setStyleSheet(
            "color: #4A5C6D; font-size: 18px; font-weight: bold; "
            "letter-spacing: 2px; border: none;"
        )
        self.image_label.setSizePolicy(QSizePolicy.Ignored, QSizePolicy.Ignored)

        # Opacity effect for smooth fade-in
        self._opacity_effect = QGraphicsOpacityEffect(self.image_label)
        self._opacity_effect.setOpacity(1.0)
        self.image_label.setGraphicsEffect(self._opacity_effect)

        display_layout.addWidget(self.image_label)
        content_layout.addWidget(display_panel, stretch=1)

        # ------ RIGHT PANEL: SYSTEM LOG ------
        log_panel = QFrame()
        log_panel.setFrameShape(QFrame.StyledPanel)
        log_panel.setStyleSheet(
            "QFrame { background-color: #0D1117; border-radius: 4px; "
            "border: 1px solid #233446; padding: 6px; }"
        )
        log_layout = QVBoxLayout()
        log_layout.setContentsMargins(6, 6, 6, 6)
        log_layout.setSpacing(4)
        log_panel.setLayout(log_layout)
        log_panel.setFixedWidth(280)

        log_title = QLabel("SYSTEM LOG")
        log_title.setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #FF5722; "
            "letter-spacing: 2px; margin-bottom: 4px;"
        )
        log_layout.addWidget(log_title)

        self.log_output = QPlainTextEdit()
        self.log_output.setReadOnly(True)
        self.log_output.setMaximumBlockCount(500)
        log_layout.addWidget(self.log_output)

        content_layout.addWidget(log_panel)

        # --- Initial log entry ---
        self._log("System initialized.")
        self._log("Awaiting operator input.")

    # ==========================================================
    #  BROWSE — SELECT SAVE PATH
    # ==========================================================
    def select_save_path(self):
        folder = QFileDialog.getExistingDirectory(
            self, "Select Save Directory", os.path.expanduser("~"),
            QFileDialog.ShowDirsOnly | QFileDialog.DontResolveSymlinks
        )
        if folder:
            self.path_input.setText(folder)
            self._log(f"Save path set: {folder}")

    # ==========================================================
    #  SPACEWIRE LINK INDICATOR  (blinks green when connected)
    # ==========================================================
    def _update_spw_indicator(self):
        connected = os.path.exists("/dev/star_spw_usb00")
        if connected:
            self._spw_blink_count += 1
            # ON for 9 ticks (1.8s), OFF for 1 tick (0.2s) — very visible blink
            if self._spw_blink_count <= 9:
                self.spw_indicator.setText("● SPW LINK: CONNECTED")
                self.spw_indicator.setStyleSheet(
                    "font-size: 13px; font-weight: 900; color: #00E676; "
                    "letter-spacing: 2px; border: none; "
                    "background-color: #003320; padding: 2px 8px; border-radius: 3px;"
                )
            else:
                self.spw_indicator.setText("● SPW LINK: CONNECTED")
                self.spw_indicator.setStyleSheet(
                    "font-size: 13px; font-weight: 900; color: #0B0E14; "
                    "letter-spacing: 2px; border: none; "
                    "background-color: #0B0E14; padding: 2px 8px; border-radius: 3px;"
                )
                if self._spw_blink_count >= 10:
                    self._spw_blink_count = 0
        else:
            self._spw_blink_count = 0
            self.spw_indicator.setText("● SPW LINK: DISCONNECTED")
            self.spw_indicator.setStyleSheet(
                "font-size: 13px; font-weight: 900; color: #FF1744; "
                "letter-spacing: 2px; border: none; "
                "background-color: #1A0505; padding: 2px 8px; border-radius: 3px;"
            )

    # ==========================================================
    #  MISSION CLOCK
    # ==========================================================
    def _update_clock(self):
        now = datetime.now().strftime("%H:%M:%S")
        self.clock_label.setText(f"MISSION TIME: {now}")

    # ==========================================================
    #  STATUS BAR HELPERS
    # ==========================================================
    def _set_status(self, state):
        styles = {
            "waiting": ("\u25cf  STATUS: WAITING",  "#42A5F5"),
            "running": ("\u25cf  STATUS: RUNNING",  "#FFB300"),
            "success": ("\u25cf  STATUS: SUCCESS",  "#00E676"),
            "error":   ("\u25cf  STATUS: ERROR",    "#FF1744"),
        }
        text, color = styles.get(state, styles["waiting"])
        self.status_label.setText(text)
        self.status_label.setStyleSheet(
            f"font-size: 13px; font-weight: 900; color: {color}; "
            "letter-spacing: 2px; border: none;"
        )

    # ==========================================================
    #  SYSTEM LOG HELPER
    # ==========================================================
    def _log(self, message):
        timestamp = datetime.now().strftime("%H:%M:%S")
        self.log_output.appendPlainText(f"[{timestamp}]  {message}")
        # Auto-scroll to bottom
        scrollbar = self.log_output.verticalScrollBar()
        scrollbar.setValue(scrollbar.maximum())

    # ==========================================================
    #  ACTIVITY ANIMATION  (animated dots while running)
    # ==========================================================
    def _start_activity_animation(self):
        self._anim_dots = 0
        self._anim_timer.start(400)

    def _stop_activity_animation(self):
        self._anim_timer.stop()

    def _update_activity_animation(self):
        self._anim_dots = (self._anim_dots % 3) + 1
        dots = "." * self._anim_dots
        self.image_label.setText(f"COMMUNICATING WITH RPU{dots}")
        self.image_label.setStyleSheet(
            "color: #FFB300; font-size: 20px; font-weight: bold; "
            "letter-spacing: 2px; border: none;"
        )
        QApplication.processEvents()

    # ==========================================================
    #  INPUT VALIDATION  (red glow on invalid fields)
    # ==========================================================
    def _validate_inputs(self):
        valid = True
        error_style = "border: 1px solid #FF1744; background-color: #1A0505;"
        ok_style = ""  # revert to global stylesheet

        for widget in [self.path_input, self.time_input, self.file_input]:
            if not widget.text().strip():
                widget.setStyleSheet(error_style)
                valid = False
            else:
                widget.setStyleSheet(ok_style)

        if self.gain_input.currentIndex() == 0:
            self.gain_input.setStyleSheet(error_style)
            valid = False
        else:
            self.gain_input.setStyleSheet(ok_style)

        if self.mode_input.currentIndex() == 0:
            self.mode_input.setStyleSheet(error_style)
            valid = False
        else:
            self.mode_input.setStyleSheet(ok_style)

        return valid

    # ==========================================================
    #  FADE-IN EFFECT FOR IMAGE
    # ==========================================================
    def _fade_in_image(self):
        self._opacity_effect.setOpacity(0.0)
        self._fade_anim = QPropertyAnimation(self._opacity_effect, b"opacity")
        self._fade_anim.setDuration(600)
        self._fade_anim.setStartValue(0.0)
        self._fade_anim.setEndValue(1.0)
        self._fade_anim.setEasingCurve(QEasingCurve.InOutCubic)
        self._fade_anim.start()

    # ==========================================================
    #  RUN CAPTURE
    # ==========================================================
    def run_capture(self):
        save_path = self.path_input.text().strip()
        gain = self.gain_input.currentText()
        time_val = self.time_input.text().strip()
        file_name = self.file_input.text().strip()

        # Validation
        if not self._validate_inputs():
            self._log("ERROR: Missing required input fields.")
            self._set_status("error")
            QMessageBox.warning(self, "Input Error", "Please fill all required fields.")
            return

        # Print all parameters to terminal
        print("\n" + "="*50)
        print("  [GUI] RUN CAPTURE — Parameters")
        print("="*50)
        print(f"  Save Path        : {save_path}")
        print(f"  Gain             : {gain}")
        print(f"  Integration Time : {time_val} ms")
        print(f"  File Name        : {file_name}")
        print("="*50 + "\n")

        self._log(f"Parameters — Path: {save_path} | Gain: {gain} | "
                  f"Time: {time_val}ms | File: {file_name}")

        # Resolve paths
        script_dir = os.path.dirname(os.path.abspath(__file__))
        script_path = os.path.join(script_dir, "run_capture.sh")

        if not os.path.isabs(save_path):
            save_path = os.path.abspath(os.path.join(script_dir, save_path))

        if not os.path.exists(script_path):
            self._log("CRITICAL: run_capture.sh not found!")
            self._set_status("error")
            QMessageBox.critical(self, "Error",
                                 f"Could not find run_capture.sh at:\n{script_path}")
            return

        try:
            # --- BEGIN CAPTURE SEQUENCE ---
            self._set_status("running")
            self._log("Initializing capture sequence...")
            self._log("Sending command to RPU interface...")
            self._start_activity_animation()

            # Call the shell script
            mode = "1" if "PDS" in self.mode_input.currentText() else "0"
            process = subprocess.run(
                ["bash", script_path, save_path, gain, time_val, file_name, mode],
                capture_output=True, text=True, check=True
            )

            self._stop_activity_animation()
            self._log("Command executed successfully.")

            # Enable show image button
            self.show_button.setEnabled(True)

            # Load and display the image
            expected_image_path = os.path.join(save_path, f"{file_name}.png")
            if os.path.exists(expected_image_path):
                self.current_image_path = expected_image_path
                self._log(f"Image acquired: {expected_image_path}")
                self._set_status("success")
                self.image_label.setStyleSheet("border: none;")
                self.display_image()
                self._fade_in_image()
                self._log("Capture sequence complete.")
            else:
                self.image_label.setText("[ ERROR ] NO TARGET DUMP FOUND")
                self.image_label.setStyleSheet(
                    "color: #FF1744; font-size: 16px; font-weight: bold; "
                    "letter-spacing: 1px; border: none;"
                )
                self._set_status("error")
                self._log(f"ERROR: Image not found at {expected_image_path}")
                QMessageBox.warning(self, "Error",
                    f"Image not found at expected path:\n{expected_image_path}"
                    f"\n\nScript Output:\n{process.stdout}")

        except subprocess.CalledProcessError as e:
            self._stop_activity_animation()
            self.image_label.setText("[ OFFLINE ] NO HARDWARE CONNECTED")
            self.image_label.setStyleSheet(
                "color: #FF1744; font-size: 16px; font-weight: bold; "
                "letter-spacing: 1px; border: none;"
            )
            self._set_status("error")
            self._log(f"CRITICAL: Backend failed (exit code {e.returncode})")
            self._log(f"stderr: {e.stderr.strip()}" if e.stderr else "No stderr output.")
            QMessageBox.critical(self, "Hardware Error",
                "No hardware connected.\n\nPlease connect RPU hardware and try again.")
        except Exception as e:
            self._stop_activity_animation()
            self.image_label.setText("[ FAILED ] SYSTEM EXCEPTION")
            self.image_label.setStyleSheet(
                "color: #FF1744; font-size: 16px; font-weight: bold; "
                "letter-spacing: 1px; border: none;"
            )
            self._set_status("error")
            self._log(f"EXCEPTION: {str(e)}")
            QMessageBox.critical(self, "Error",
                                 f"An unexpected error occurred:\n{str(e)}")

    # ==========================================================
    #  SHOW IMAGE
    # ==========================================================
    def show_image(self):
        save_path = self.path_input.text().strip()
        file_name = self.file_input.text().strip()

        if not save_path or not file_name:
            QMessageBox.warning(self, "Input Error",
                                "Please fill in Save Path and File Name.")
            return

        script_dir = os.path.dirname(os.path.abspath(__file__))
        if not os.path.isabs(save_path):
            save_path = os.path.abspath(os.path.join(script_dir, save_path))

        expected_image_path = os.path.join(save_path, f"{file_name}.png")

        if os.path.exists(expected_image_path):
            self.image_label.setStyleSheet("border: none;")
            self.current_image_path = expected_image_path
            self.display_image()
            self._fade_in_image()
            self._log(f"Image loaded: {expected_image_path}")
        else:
            self._log("WARN: Requested image not available.")
            QMessageBox.warning(self, "Error",
                "Image not available.\n\nRun a capture first or verify the file path.")

    # ==========================================================
    #  DISPLAY / SCALE IMAGE
    # ==========================================================
    def display_image(self):
        if hasattr(self, 'current_image_path') and os.path.exists(self.current_image_path):
            pixmap = QPixmap(self.current_image_path)
            if not pixmap.isNull():
                scaled_pixmap = pixmap.scaled(
                    self.image_label.size(),
                    Qt.KeepAspectRatio,
                    Qt.SmoothTransformation
                )
                self.image_label.setPixmap(scaled_pixmap)

    def resizeEvent(self, event):
        super().resizeEvent(event)
        self.display_image()


# ==============================================================
#  APPLICATION ENTRY POINT
# ==============================================================
if __name__ == '__main__':
    # HiDPI attributes MUST be set before QApplication is created
    if hasattr(Qt, 'AA_EnableHighDpiScaling'):
        QApplication.setAttribute(Qt.AA_EnableHighDpiScaling, True)
    if hasattr(Qt, 'AA_UseHighDpiPixmaps'):
        QApplication.setAttribute(Qt.AA_UseHighDpiPixmaps, True)

    app = QApplication(sys.argv)
    window = ImageAcquisitionApp()
    window.showMaximized()
    sys.exit(app.exec_())
