# Quick Development Workflow

Các cách để code GUI nhanh hơn (giống HMR web development):

## 🔥 Cách 1: Auto-rebuild với Watch Script (Khuyên dùng)

### Cài đặt fswatch (chỉ cần 1 lần):
```bash
brew install fswatch
```

### Chạy watch mode:
```bash
./watch_build.sh
```

**Workflow:**
1. Chạy `./watch_build.sh` trong terminal
2. Chạy Standalone app: `./run_standalone.sh` (terminal khác)
3. Sửa code trong VSCode
4. Script tự động rebuild
5. Tắt app → Mở lại để thấy thay đổi

---

## ⚡ Cách 2: VSCode Tasks (Phím tắt)

### Build nhanh:
- **Cmd+Shift+B**: Build Standalone (nhanh, chỉ 20-30s)
- **Cmd+Shift+R**: Build & Run Standalone

### Hoặc dùng Command Palette:
1. Nhấn `Cmd+Shift+P`
2. Gõ "Tasks: Run Task"
3. Chọn "Build Standalone (Quick)"

---

## 🚀 Cách 3: Build thủ công (Đơn giản nhất)

### Build chỉ Standalone (nhanh):
```bash
cmake --build build --config Release --target AirQ4Mono_Standalone
```

### Chạy Standalone:
```bash
./run_standalone.sh
```

### Build tất cả (VST3, AU, Standalone):
```bash
cmake --build build --config Release
```

---

## 💡 Tips để Dev nhanh hơn:

### 1. Chỉ build Standalone khi đang dev GUI
```bash
# Nhanh hơn 3-4 lần so với build all
cmake --build build --config Release --target AirQ4Mono_Standalone
```

### 2. Workflow 2 màn hình:
- **Màn 1**: VSCode (code)
- **Màn 2**: Standalone app running
- Mỗi lần thay đổi: Build → Tắt app → Mở lại

### 3. Dùng Git để test nhanh:
```bash
# Thử nghiệm
git stash

# Hoàn tác
git stash pop
```

### 4. Build incremental (chỉ build file thay đổi):
CMake tự động làm điều này, nên build lần 2 trở đi rất nhanh!

---

## 📝 File cấu hình đã tạo:

- `watch_build.sh` - Auto-rebuild khi file thay đổi
- `run_standalone.sh` - Launcher nhanh cho Standalone
- `.vscode/tasks.json` - VSCode tasks
- `.vscode/keybindings.json` - Phím tắt (Cmd+Shift+B, Cmd+Shift+R)

---

## ⚙️ Advanced: Build chỉ file cụ thể

### Nếu chỉ sửa PluginEditor.cpp:
```bash
# Xcode sẽ chỉ compile file này (nhanh!)
cmake --build build --config Release --target AirQ4Mono_Standalone
```

### Clean build (khi gặp lỗi lạ):
```bash
cmake --build build --target clean
# Hoặc xóa hoàn toàn:
rm -rf build && cmake -B build -DCMAKE_BUILD_TYPE=Release
```

---

## 🎯 So sánh thời gian build:

| Build type | Thời gian | Khi nào dùng |
|------------|-----------|--------------|
| Standalone only (incremental) | 5-10s | Dev GUI thường xuyên |
| Standalone only (full) | 20-30s | Lần đầu hoặc thay đổi lớn |
| All formats (VST3+AU+Standalone) | 60-120s | Trước khi release |
| Clean + Full rebuild | 2-3 phút | Khi có lỗi linking lạ |

---

## 🐛 Troubleshooting:

### Standalone không cập nhật GUI:
1. Đảm bảo đã tắt hoàn toàn app cũ
2. Build lại: `Cmd+Shift+B`
3. Chạy lại: `./run_standalone.sh`

### Build lỗi "target not found":
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

### fswatch không hoạt động:
```bash
brew reinstall fswatch
```
