# Quick commands for AirQ4 development

.PHONY: help standalone run all clean watch

help:
	@echo "🎚️  AirQ4 Development Commands"
	@echo ""
	@echo "Quick build:"
	@echo "  make standalone    - Build Standalone app only (nhanh)"
	@echo "  make run          - Build & run Standalone"
	@echo "  make all          - Build tất cả (VST3, AU, Standalone)"
	@echo ""
	@echo "Advanced:"
	@echo "  make clean        - Clean build"
	@echo "  make watch        - Auto-rebuild khi file thay đổi"
	@echo ""
	@echo "💡 Tip: Dùng 'make standalone' khi đang dev GUI!"

standalone:
	@echo "⚡ Building Standalone (quick)..."
	@cmake --build build --config Release --target AirQ4Mono_Standalone
	@echo "✅ Done! Run with: make run"

run: standalone
	@echo "🚀 Launching Standalone..."
	@./run_standalone.sh

all:
	@echo "🔨 Building all formats..."
	@cmake --build build --config Release
	@echo "✅ Done!"

clean:
	@echo "🧹 Cleaning build..."
	@cmake --build build --target clean
	@echo "✅ Done!"

watch:
	@echo "👀 Starting watch mode..."
	@./watch_build.sh
