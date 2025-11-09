#!/bin/bash
# Auto-rebuild AirQ4 Standalone khi file thay đổi

echo "🔄 Watching for changes in Source/..."
echo "Press Ctrl+C to stop"

# Install fswatch nếu chưa có: brew install fswatch

fswatch -o Source/ | while read change; do
    echo ""
    echo "📝 File changed, rebuilding Standalone..."
    echo "⏱️  $(date '+%H:%M:%S')"

    # Build only Standalone (nhanh hơn build all)
    if cmake --build build --config Release --target AirQ4Mono_Standalone 2>&1 | tail -5; then
        echo "✅ Build succeeded!"
        echo "🚀 Launch: build/AirQ4Mono_artefacts/Release/Standalone/AirQ4\ Mono.app/Contents/MacOS/AirQ4\ Mono"
    else
        echo "❌ Build failed!"
    fi
done
