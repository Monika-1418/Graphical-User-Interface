#!/bin/bash

# ============================================================
#  run_capture.sh — Calls RPUInterfaceSim (real hardware ONLY)
#  to capture images via STAR-Dundee SpaceWire Brick Mk3
#  Output: PGM format only — no conversion
#  Binary runs from its own directory, PGM copied to save path
# ============================================================

SAVE_PATH=$1
GAIN=$2
INTEGRATION_TIME=$3
FILE_NAME=$4
MODE=$5          # "1" = PDS, "0" = RS  (from gui.py)

mkdir -p "$SAVE_PATH"

# ---- RPUInterfaceSim binary location ----
RPU_DIR="/home/vsn/Documents/RPUInterfaceSIm"
RPU_BIN="${RPU_DIR}/RPUInterfaceSim"

# ---- Convert integration time: GUI sends ms, binary expects microseconds ----
INTEG_USEC=$(echo "$INTEGRATION_TIME * 1000" | bc)

# ---- selectCam: 1 = PDS, 0 = RS (matches the C binary's scanf) ----
SELECT_CAM="$MODE"

echo "Running RPUInterfaceSim..."
echo "  Camera     : $([ "$SELECT_CAM" = "1" ] && echo PDS || echo RS)"
echo "  Gain       : $GAIN"
echo "  Integ Time : $INTEG_USEC µs ($INTEGRATION_TIME ms)"
echo "  File Name  : $FILE_NAME"
echo "  Save Path  : $SAVE_PATH"

# Ensure STAR-Dundee shared libraries are available
export LD_LIBRARY_PATH="/usr/local/STAR-Dundee/STAR-System/lib:/lib/x86_64-linux-gnu:${LD_LIBRARY_PATH}"

# ---- Check real binary exists ----
if [ ! -f "$RPU_BIN" ]; then
    echo "ERROR: RPUInterfaceSim binary not found at $RPU_BIN" >&2
    exit 1
fi

# ---- Check SpaceWire device is connected ----
if [ ! -e /dev/star_spw_usb00 ]; then
    echo "ERROR: SpaceWire device /dev/star_spw_usb00 not found. Connect hardware and try again." >&2
    exit 1
fi

echo "INFO: SpaceWire device /dev/star_spw_usb00 detected."

# ---- Run binary from its own directory (exactly like terminal) ----
#   ./RPUInterfaceSim 1 1 1 1 1
#   Stdin (scanf order): selectCam, integTime(µs), gain, fileName
# ----

echo "INFO: Invoking RPUInterfaceSim from ${RPU_DIR}..."
printf "%s\n%s\n%s\n%s\n" "$SELECT_CAM" "$INTEG_USEC" "$GAIN" "$FILE_NAME" | \
    (cd "$RPU_DIR" && sudo -A ./RPUInterfaceSim 1 1 1 1 1)

EXIT_CODE=$?

# ---- Verify PGM output (created in RPU_DIR/images/) ----
PGM_SRC="${RPU_DIR}/images/${FILE_NAME}_0.pgm"

if [ $EXIT_CODE -ne 0 ]; then
    echo "ERROR: RPUInterfaceSim failed (exit code $EXIT_CODE)" >&2
    exit 1
fi

if [ ! -s "$PGM_SRC" ]; then
    echo "ERROR: No PGM image data produced at $PGM_SRC" >&2
    ls -la "${RPU_DIR}/images/" 2>/dev/null
    exit 1
fi

# ---- Copy all output files to save path ----
mkdir -p "${SAVE_PATH}/images"

SRC_BASE="${RPU_DIR}/images/${FILE_NAME}_0"
DST_BASE="${SAVE_PATH}/images/${FILE_NAME}_0"

for ext in "" ".test" ".pgm" ".txt"; do
    SRC="${SRC_BASE}${ext}"
    if [ -f "$SRC" ]; then
        cp "$SRC" "${DST_BASE}${ext}"
        echo "Copied: ${FILE_NAME}_0${ext}"
    else
        echo "WARN: ${FILE_NAME}_0${ext} not found in RPU output"
    fi
done

echo "Done. All files saved to ${SAVE_PATH}/images/"
exit 0
