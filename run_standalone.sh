#!/bin/bash
# Quick launcher for Standalone app

APP_PATH="build/AirQ4Mono_artefacts/Release/Standalone/AirQ4 Mono.app/Contents/MacOS/AirQ4 Mono"

if [ -f "$APP_PATH" ]; then
    echo "🚀 Launching AirQ4 Standalone..."
    "$APP_PATH"
else
    echo "❌ Standalone app not found. Build it first:"
    echo "   cmake --build build --config Release --target AirQ4Mono_Standalone"
fi
