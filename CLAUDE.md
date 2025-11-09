# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

AirQ4 is a JUCE-based audio plugin that implements a 6-band equalizer with fixed frequencies optimized for audio mastering and mixing. The plugin is built in two variants: Mono and Stereo.

**Plugin Details:**
- Company: TAudio
- Formats: VST3, AU, Standalone
- Plugin Codes: AQ4M (Mono), AQ4S (Stereo)
- C++ Standard: C++17

## Build System

This project uses CMake with JUCE framework. JUCE is automatically downloaded via CPM (CMake Package Manager) during the first configuration - no manual setup required.

**Dependency Management:**
- CPM downloads JUCE from GitHub on first configure ([CMakeLists.txt:18-22](CMakeLists.txt#L18-L22))
- JUCE is cached in `build/_deps/` and gitignored
- Internet connection required only for initial setup

### Build Commands

```bash
# Configure (first time or after CMakeLists.txt changes)
# CPM will download JUCE automatically on first run
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build both Mono and Stereo versions
cmake --build build --config Release

# Build only Mono version
cmake --build build --config Release --target AirQ4Mono

# Build only Stereo version
cmake --build build --config Release --target AirQ4Stereo

# Clean build (preserves downloaded dependencies)
cmake --build build --target clean

# Full clean (removes downloaded dependencies - will re-download on next configure)
rm -rf build
```

The build outputs VST3, AU, and Standalone versions and automatically copies them to system plugin directories (controlled by `COPY_PLUGIN_AFTER_BUILD` in CMakeLists.txt).

## Architecture

### Dual-Build Strategy

The project builds two separate plugins from shared source code using preprocessor definitions:
- `AIRQ4_MONO=1` for mono version
- `AIRQ4_STEREO=1` for stereo version

The PluginProcessor constructor uses these definitions to configure mono vs stereo bus layouts ([PluginProcessor.cpp:6-18](Source/PluginProcessor.cpp#L6-L18)).

### Core Components

**PluginProcessor** ([PluginProcessor.h](Source/PluginProcessor.h))
- Main DSP processor class
- Contains 6 IIR filters for each frequency band (lines 50-55)
- Slider values stored as 0-1 normalized floats (lines 58-63)
- Air frequency selector for high shelf band

**PluginEditor** ([PluginEditor.h](Source/PluginEditor.h))
- GUI using JUCE with custom RealisticKnobLook
- Background image loaded from BinaryData
- 7 rotary sliders (6 bands + air frequency selector)

**EQGraphComponent** ([EQGraphComponent.h](Source/EQGraphComponent.h))
- Real-time visualization of Q and Gain curves
- Updates at 30Hz via Timer
- Red curve = Q mapping, Green curve = Gain mapping
- Yellow line = current slider position

### Module Structure

**Source/modules/dsp/**
- `dsp_helper.h`: Filter parameter calculations with snap-to-step quantization
  - Maps normalized slider values (0-1) to actual Q and Gain parameters
  - Each band has unique Q/Gain ranges and behaviors
  - `SLIDER_STEP = 0.5f` for discrete control values
- `soft_saturator.cpp`: Oversampled asymmetrical saturation (tube-like)

**Source/modules/gui/**
- `RealisticKnobLook.h/cpp`: Custom LookAndFeel for filmstrip-based rotary knobs
  - Reads multi-frame PNG strips from BinaryData
  - Draws appropriate frame based on slider position

### EQ Band Specifications

Fixed frequencies defined in [PluginProcessor.h:7-12](Source/PluginProcessor.h#L7-L12):

1. **SUB (25Hz)**: High-pass filter, Q only (0.4-2.6)
2. **40Hz**: Low shelf, Gain -5 to +5 dB, Q reversed 0.1-1.0
3. **160Hz**: Bell, Gain -5 to +5 dB, Q 0.5-1.5
4. **650Hz**: Bell, Gain -5 to +5 dB, Q 0.5-1.5
5. **2.5kHz**: Bell, Gain -5 to +5 dB, Q 0.5-1.5
6. **Air (8kHz+)**: High shelf, Gain 0 to +10 dB, Q 0.5-1.5, selectable frequency

### Binary Assets

All GUI assets are embedded via `juce_add_binary_data` ([CMakeLists.txt:24-34](CMakeLists.txt#L24-L34)):
- `background.png`: Main GUI background
- `*_knob_strip.png`: Filmstrip images for each knob (multi-frame vertical strips)
- `btn_on_off_strip.png`, `signal_*_strip.png`: UI indicators

Access via `BinaryData::` namespace in code.

## Development Workflow

### Adding/Modifying Assets

1. Place PNG files in `Assets/` directory
2. Add to `juce_add_binary_data` section in [CMakeLists.txt:24-34](CMakeLists.txt#L24-L34)
3. Reconfigure CMake: `cmake -B build`
4. Access in code via `BinaryData::filename_png` and `BinaryData::filename_pngSize`

### Modifying Filter Parameters

Filter parameter calculations are centralized in [Source/modules/dsp/dsp_helper.h](Source/modules/dsp/dsp_helper.h):
- `calcSubLowCutQ()`: SUB band Q calculation
- `calcLowShelf40Hz()`: 40Hz shelf gain/Q
- `calcBell()`: Bell filters (160Hz, 650Hz, 2.5kHz)
- `calcHighShelfAir()`: Air band gain/Q

All use `snapToStep()` for discrete 0.5-step quantization.

### Adding a New EQ Band

1. Add frequency constant in [PluginProcessor.h](Source/PluginProcessor.h)
2. Add `juce::dsp::IIR::Filter<float>` member to PluginProcessor
3. Add corresponding slider value float member
4. Create calculation function in `dsp_helper.h`
5. Add slider to PluginEditor and wire up in constructor
6. Update filter in `prepareToPlay()` and `processBlock()`
7. Add knob filmstrip PNG to Assets/ and BinaryData

### GUI Customization

The GUI uses fixed-position layout set in `PluginEditor::resized()`. To modify:
- Knob positions: Edit `setBounds()` calls in resized()
- Background: Replace `Assets/background.png`
- Knob appearance: Replace filmstrip PNGs (ensure frame count matches RealisticKnobLook constructor)

## Important Notes

- **Filter State**: Filters are reset in `prepareToPlay()` when sample rate or block size changes
- **Thread Safety**: Slider value members are accessed from both audio and GUI threads; consider atomics for production
- **Oversampling**: SoftSaturator uses 2x oversampling to reduce aliasing
- **Parameter Automation**: Current implementation uses raw float members; JUCE AudioProcessorValueTreeState recommended for DAW automation support
