cmake_minimum_required(VERSION 3.24)
project(AirQ4 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(JUCE)

juce_add_binary_data(BinaryData SOURCES
    Assets/background.png
    Assets/btn_on_off_strip.png
    Assets/signal_on_off_strip.png
    Assets/signal_peak_strip.png
    Assets/sub_knob_strip.png
    Assets/40_knob_strip.png
    Assets/160_knob_strip.png
    Assets/650_knob_strip.png
    Assets/2k5_knob_strip.png
    Assets/air_gain_strip.png
    Assets/air_knob_strip.png
)

# =====================================================
# MONO VERSION
# =====================================================
juce_add_plugin(AirQ4Mono
    COMPANY_NAME "TAudio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    JUCE_VST3_CAN_REPLACE_VST2 FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE "TAud"
    PLUGIN_CODE "AQ4M"                # 🔹 mã riêng cho Mono
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "AirQ4 (Mono)"
)

# =====================================================
# STEREO VERSION
# =====================================================
juce_add_plugin(AirQ4Stereo
    COMPANY_NAME "TAudio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    JUCE_VST3_CAN_REPLACE_VST2 FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE "TAud"
    PLUGIN_CODE "AQ4S"                # 🔹 mã riêng cho Stereo
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "AirQ4 (Stereo)"
)

# =====================================================
# COMMON SOURCES
# =====================================================
set(AIRQ4_SOURCES
    Source/modules/dsp/dsp_helper.h
    Source/modules/dsp/soft_saturator.cpp
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

target_sources(AirQ4Mono PRIVATE ${AIRQ4_SOURCES})
target_sources(AirQ4Stereo PRIVATE ${AIRQ4_SOURCES})

# =====================================================
# DEFINES
# =====================================================
target_compile_definitions(AirQ4Mono PRIVATE 
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    AIRQ4_MONO=1                     # 🔹 flag để phân biệt trong code
)

target_compile_definitions(AirQ4Stereo PRIVATE 
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    AIRQ4_STEREO=1                   # 🔹 flag để phân biệt trong code
)

# =====================================================
# LINK LIBRARIES
# =====================================================
target_link_libraries(AirQ4Mono PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_audio_basics
    juce::juce_dsp
    juce::juce_gui_extra
    BinaryData          
)

target_link_libraries(AirQ4Stereo PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_audio_basics
    juce::juce_dsp
    juce::juce_gui_extra
    BinaryData          
)
