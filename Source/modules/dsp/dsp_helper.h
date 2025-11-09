#pragma once

#include <utility>
#include <cmath>

constexpr float SLIDER_STEP = 0.5f;

constexpr float SUB_Q_MIN = 0.4f;
constexpr float SUB_Q_MAX = 2.6f;

constexpr float LOWSHELF_GAIN_MIN = -5.0f;
constexpr float LOWSHELF_GAIN_MAX = 5.0f;
constexpr float LOWSHELF_Q_MIN    = 0.1f;
constexpr float LOWSHELF_Q_MAX    = 1.0f;

constexpr float BELL_GAIN_MIN = -5.0f;
constexpr float BELL_GAIN_MAX = 5.0f;
constexpr float BELL_Q_MIN    = 0.5f;
constexpr float BELL_Q_MAX    = 1.5f;

constexpr float HIGHSHELF_GAIN_MIN = 0.0f;
constexpr float HIGHSHELF_GAIN_MAX = 10.0f;
constexpr float HIGHSHELF_Q_MIN    = 0.5f;
constexpr float HIGHSHELF_Q_MAX    = 1.5f;

inline float normalize(float val, float minVal, float maxVal)
{
    return (val - minVal) / (maxVal - minVal);
}

// Helper: snap to step
inline float snapToStep(float val, float step = SLIDER_STEP)
{
    return std::round(val / step) * step;
}

// Sub LowCut: chỉ Q
inline float calcSubLowCutQ(float sliderVal)
{
    return snapToStep(SUB_Q_MIN + sliderVal * (SUB_Q_MAX - SUB_Q_MIN));
}

// LowShelf 40Hz: Q reversed, gain linear
inline std::pair<float,float> calcLowShelf40Hz(float sliderVal)
{
    float gain = snapToStep(LOWSHELF_GAIN_MIN + sliderVal*(LOWSHELF_GAIN_MAX-LOWSHELF_GAIN_MIN));
    float Q    = snapToStep(LOWSHELF_Q_MAX - sliderVal*(LOWSHELF_Q_MAX-LOWSHELF_Q_MIN));
    return {gain, Q};
}

// Bells
inline std::pair<float,float> calcBell(float sliderVal)
{
    float gain = snapToStep(BELL_GAIN_MIN + sliderVal*(BELL_GAIN_MAX-BELL_GAIN_MIN));
    float Q    = snapToStep(BELL_Q_MAX - (gain-BELL_GAIN_MIN)/(BELL_GAIN_MAX-BELL_GAIN_MIN)*(BELL_Q_MAX-BELL_Q_MIN)*0.2f);
    return {gain, Q};
}

// HighShelf Air
inline std::pair<float,float> calcHighShelfAir(float sliderVal)
{
    float gain = snapToStep(HIGHSHELF_GAIN_MIN + sliderVal*(HIGHSHELF_GAIN_MAX-HIGHSHELF_GAIN_MIN));
    float Q    = snapToStep(HIGHSHELF_Q_MIN + (gain/(HIGHSHELF_GAIN_MAX-HIGHSHELF_GAIN_MIN))*(HIGHSHELF_Q_MAX-HIGHSHELF_Q_MIN)*0.2f);
    return {gain, Q};
}
