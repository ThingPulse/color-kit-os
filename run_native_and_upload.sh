#!/usr/bin/env bash
set -euo pipefail

# 1) Build + start your native app (don’t use native:upload; it blocks).
#    If you already have a custom non-blocking target, use it here.
pio run -e emulator_ckpro_mac -t upload &   # launches and returns immediately
PID_NATIVE=$!

# 2) Build + flash ESP32 in parallel
pio run -e ckprov2 -t upload &
PID_ESP32=$!

# Wait for both; exit non-zero if either fails
wait $PID_NATIVE
wait $PID_ESP32
