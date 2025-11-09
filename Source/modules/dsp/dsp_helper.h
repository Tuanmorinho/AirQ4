#pragma once

#include <utility>
#include <cmath>

constexpr float SLIDER_MIN = -5.0f;
constexpr float SLIDER_MAX = 5.0f;
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

// Normalize slider value (-5..5) to (0..1)
inline float normalizeSlider(float sliderVal)
{
    return (sliderVal - SLIDER_MIN) / (SLIDER_MAX - SLIDER_MIN);
}

// Helper: snap to step
inline float snapToStep(float val, float step = SLIDER_STEP)
{
    return std::round(val / step) * step;
}

// Sub LowCut: chỉ Q, map từ -5..5
inline float calcSubLowCutQ(float sliderVal)
{
    float normalized = normalizeSlider(sliderVal);
    return snapToStep(SUB_Q_MIN + normalized * (SUB_Q_MAX - SUB_Q_MIN));
}

// LowShelf 40Hz: Q reversed, gain linear
inline std::pair<float,float> calcLowShelf40Hz(float sliderVal)
{
    float normalized = normalizeSlider(sliderVal);
    float gain = snapToStep(LOWSHELF_GAIN_MIN + normalized * (LOWSHELF_GAIN_MAX - LOWSHELF_GAIN_MIN));
    float Q    = snapToStep(LOWSHELF_Q_MAX - normalized * (LOWSHELF_Q_MAX - LOWSHELF_Q_MIN));
    return {gain, Q};
}

// Bells: gain linear, Q decrease slightly as gain increases
inline std::pair<float,float> calcBell(float sliderVal)
{
    float normalized = normalizeSlider(sliderVal);
    float gain = snapToStep(BELL_GAIN_MIN + normalized * (BELL_GAIN_MAX - BELL_GAIN_MIN));
    
    // Q giảm nhẹ khi gain tăng (để bandwidth hẹp hơn khi boost nhiều)
    float gainNormalized = (gain - BELL_GAIN_MIN) / (BELL_GAIN_MAX - BELL_GAIN_MIN);
    float Q = snapToStep(BELL_Q_MAX - gainNormalized * (BELL_Q_MAX - BELL_Q_MIN) * 0.2f);
    
    return {gain, Q};
}

// HighShelf Air: gain 0..10, Q increase slightly with gain
inline std::pair<float,float> calcHighShelfAir(float sliderVal)
{
    // Air slider có range 0..10 thay vì -5..5
    float normalized = sliderVal / HIGHSHELF_GAIN_MAX;
    float gain = snapToStep(normalized * HIGHSHELF_GAIN_MAX);
    float Q    = snapToStep(HIGHSHELF_Q_MIN + normalized * (HIGHSHELF_Q_MAX - HIGHSHELF_Q_MIN) * 0.2f);
    return {gain, Q};
}