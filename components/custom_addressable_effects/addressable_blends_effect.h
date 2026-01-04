#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include <string>
#include "esphome/components/light/addressable_light_effect.h"

namespace esphome {
namespace light {

// Color Twinkles palette types
enum ColorTwinklesPaletteType {
  COLOR_TWINKLES_PALETTE_CLOUD_COLORS,
  COLOR_TWINKLES_PALETTE_RAINBOW_COLORS,
  COLOR_TWINKLES_PALETTE_SNOW_COLORS,
  COLOR_TWINKLES_PALETTE_INCANDESCENT,
  COLOR_TWINKLES_PALETTE_PARTY_COLORS,
  COLOR_TWINKLES_PALETTE_OCEAN_COLORS,
  COLOR_TWINKLES_PALETTE_FOREST_COLORS,
  COLOR_TWINKLES_PALETTE_LAVA_COLORS,
};

class AddressableBlendsEffect : public AddressableLightEffect {
 public:
  AddressableBlendsEffect(const char *name) : AddressableLightEffect(name) {}

  void set_cycle_s(uint8_t s) { cycle_s_ = s; }
  void set_scale(uint16_t scale) { scale_ = scale; }
  void set_palette(ColorTwinklesPaletteType palette) { palette_type_ = palette; }
  void set_palette(const char *palette) {
    std::string p(palette);
    if (p == "cloud_colors") this->palette_type_ = COLOR_TWINKLES_PALETTE_CLOUD_COLORS;
    else if (p == "rainbow_colors") this->palette_type_ = COLOR_TWINKLES_PALETTE_RAINBOW_COLORS;
    else if (p == "snow_colors") this->palette_type_ = COLOR_TWINKLES_PALETTE_SNOW_COLORS;
    else if (p == "incandescent") this->palette_type_ = COLOR_TWINKLES_PALETTE_INCANDESCENT;
    else if (p == "party_colors") this->palette_type_ = COLOR_TWINKLES_PALETTE_PARTY_COLORS;
    else if (p == "ocean_colors") this->palette_type_ = COLOR_TWINKLES_PALETTE_OCEAN_COLORS;
    else if (p == "forest_colors") this->palette_type_ = COLOR_TWINKLES_PALETTE_FOREST_COLORS;
    else if (p == "lava_colors") this->palette_type_ = COLOR_TWINKLES_PALETTE_LAVA_COLORS;
    else this->palette_type_ = COLOR_TWINKLES_PALETTE_RAINBOW_COLORS;
  }

  void start() override { setup_palette(); }

  void stop() override {}

  void apply(AddressableLight &it, const Color &current_color) override {
    const uint32_t now = millis();

    // Throttle updates for smooth animation
    if (now - last_update_ < 20) return;
    last_update_ = now;

    const size_t num_leds = it.size();

    // phase 0-255 over cycle_s_ (seconds) using millisecond resolution for smooth movement
    uint8_t phase = 0;
    if (cycle_s_ > 0) {
      uint32_t period_ms = (uint32_t)cycle_s_ * 1000U;
      phase = (uint8_t)(((uint64_t)(now % period_ms) * 256ULL) / period_ms);
    }

    // Blend amount per frame (0-255). Higher = less smooth. Tuned to feel "puffy".
    const uint8_t blend_amt = 48;

    size_t idx = 0;
    for (auto view : it) {
      uint8_t pal_index = ((idx * scale_) + phase) & 0xFF;  // 0-255
      Color target = color_from_palette(pal_index, 255);

      // Blend current pixel toward target for soft, puffy motion
      Color current = view.get();
      uint8_t r = (uint16_t(current.r) * (255 - blend_amt) + uint16_t(target.r) * blend_amt) >> 8;
      uint8_t g = (uint16_t(current.g) * (255 - blend_amt) + uint16_t(target.g) * blend_amt) >> 8;
      uint8_t b = (uint16_t(current.b) * (255 - blend_amt) + uint16_t(target.b) * blend_amt) >> 8;
      view = Color(r, g, b);
      idx++;
    }

    it.schedule_show();
  }

 protected:
  void setup_palette() {
    // Setup 16-color palette based on palette type
    switch (palette_type_) {
      case COLOR_TWINKLES_PALETTE_CLOUD_COLORS:
        // Blue and white, like clouds
        palette_[0] = Color(0, 0, 255);      // Blue
        palette_[1] = Color(0, 64, 255);
        palette_[2] = Color(64, 128, 255);
        palette_[3] = Color(128, 192, 255);
        palette_[4] = Color(192, 220, 255);
        palette_[5] = Color(255, 255, 255);  // White
        palette_[6] = Color(192, 220, 255);
        palette_[7] = Color(128, 192, 255);
        palette_[8] = Color(64, 128, 255);
        palette_[9] = Color(0, 64, 255);
        palette_[10] = Color(0, 0, 255);
        palette_[11] = Color(64, 128, 255);
        palette_[12] = Color(128, 192, 255);
        palette_[13] = Color(192, 220, 255);
        palette_[14] = Color(255, 255, 255);
        palette_[15] = Color(128, 192, 255);
        break;

      case COLOR_TWINKLES_PALETTE_RAINBOW_COLORS:
        // Full rainbow
        palette_[0] = Color(255, 0, 0);      // Red
        palette_[1] = Color(255, 64, 0);     // Orange
        palette_[2] = Color(255, 128, 0);
        palette_[3] = Color(255, 192, 0);
        palette_[4] = Color(255, 255, 0);    // Yellow
        palette_[5] = Color(128, 255, 0);
        palette_[6] = Color(0, 255, 0);      // Green
        palette_[7] = Color(0, 255, 128);
        palette_[8] = Color(0, 255, 255);    // Cyan
        palette_[9] = Color(0, 128, 255);
        palette_[10] = Color(0, 0, 255);     // Blue
        palette_[11] = Color(128, 0, 255);
        palette_[12] = Color(255, 0, 255);   // Magenta
        palette_[13] = Color(255, 0, 128);
        palette_[14] = Color(255, 0, 64);
        palette_[15] = Color(255, 0, 0);
        break;

      case COLOR_TWINKLES_PALETTE_SNOW_COLORS:
        // White and light blue, like snow
        palette_[0] = Color(255, 255, 255);  // White
        palette_[1] = Color(240, 248, 255);  // Alice Blue
        palette_[2] = Color(230, 240, 255);
        palette_[3] = Color(220, 235, 255);
        palette_[4] = Color(200, 220, 255);
        palette_[5] = Color(180, 200, 255);
        palette_[6] = Color(200, 220, 255);
        palette_[7] = Color(220, 235, 255);
        palette_[8] = Color(255, 255, 255);
        palette_[9] = Color(245, 250, 255);
        palette_[10] = Color(235, 245, 255);
        palette_[11] = Color(225, 240, 255);
        palette_[12] = Color(255, 255, 255);
        palette_[13] = Color(240, 248, 255);
        palette_[14] = Color(230, 245, 255);
        palette_[15] = Color(255, 255, 255);
        break;

      case COLOR_TWINKLES_PALETTE_INCANDESCENT:
        // Warm incandescent colors (fire orange/yellow)
        palette_[0] = Color(255, 147, 41);   // Candle
        palette_[1] = Color(255, 160, 50);
        palette_[2] = Color(255, 170, 60);
        palette_[3] = Color(255, 180, 70);
        palette_[4] = Color(255, 190, 80);
        palette_[5] = Color(255, 200, 90);
        palette_[6] = Color(255, 190, 80);
        palette_[7] = Color(255, 180, 70);
        palette_[8] = Color(255, 170, 60);
        palette_[9] = Color(255, 160, 50);
        palette_[10] = Color(255, 147, 41);
        palette_[11] = Color(255, 140, 35);
        palette_[12] = Color(255, 130, 30);
        palette_[13] = Color(255, 140, 35);
        palette_[14] = Color(255, 150, 45);
        palette_[15] = Color(255, 160, 55);
        break;

      case COLOR_TWINKLES_PALETTE_PARTY_COLORS:
        // Vibrant party colors
        palette_[0] = Color(85, 0, 171);     // Purple
        palette_[1] = Color(132, 0, 255);
        palette_[2] = Color(255, 0, 0);      // Red
        palette_[3] = Color(255, 85, 0);
        palette_[4] = Color(255, 170, 0);    // Orange
        palette_[5] = Color(255, 255, 0);    // Yellow
        palette_[6] = Color(0, 255, 0);      // Green
        palette_[7] = Color(0, 171, 85);
        palette_[8] = Color(0, 85, 171);
        palette_[9] = Color(0, 0, 255);      // Blue
        palette_[10] = Color(85, 0, 171);
        palette_[11] = Color(171, 0, 85);
        palette_[12] = Color(255, 0, 0);
        palette_[13] = Color(255, 85, 0);
        palette_[14] = Color(255, 170, 0);
        palette_[15] = Color(255, 255, 0);
        break;

      case COLOR_TWINKLES_PALETTE_OCEAN_COLORS:
        // Ocean blues and greens
        palette_[0] = Color(0, 0, 128);      // Navy
        palette_[1] = Color(0, 0, 170);
        palette_[2] = Color(0, 32, 192);
        palette_[3] = Color(0, 64, 200);
        palette_[4] = Color(0, 96, 200);
        palette_[5] = Color(0, 128, 200);    // Medium blue
        palette_[6] = Color(0, 150, 180);
        palette_[7] = Color(0, 170, 160);
        palette_[8] = Color(0, 180, 140);
        palette_[9] = Color(0, 190, 120);
        palette_[10] = Color(0, 200, 100);   // Aqua green
        palette_[11] = Color(0, 190, 120);
        palette_[12] = Color(0, 170, 160);
        palette_[13] = Color(0, 128, 200);
        palette_[14] = Color(0, 64, 200);
        palette_[15] = Color(0, 0, 170);
        break;

      case COLOR_TWINKLES_PALETTE_FOREST_COLORS:
        // Forest greens
        palette_[0] = Color(0, 64, 0);       // Dark green
        palette_[1] = Color(0, 96, 0);
        palette_[2] = Color(0, 128, 0);      // Green
        palette_[3] = Color(32, 160, 0);
        palette_[4] = Color(64, 192, 0);
        palette_[5] = Color(96, 200, 32);
        palette_[6] = Color(64, 192, 0);
        palette_[7] = Color(32, 160, 0);
        palette_[8] = Color(0, 128, 0);
        palette_[9] = Color(0, 96, 32);
        palette_[10] = Color(0, 64, 0);
        palette_[11] = Color(32, 80, 0);
        palette_[12] = Color(64, 96, 0);
        palette_[13] = Color(32, 80, 0);
        palette_[14] = Color(0, 64, 16);
        palette_[15] = Color(0, 80, 0);
        break;

      case COLOR_TWINKLES_PALETTE_LAVA_COLORS:
        // Lava reds and oranges
        palette_[0] = Color(128, 0, 0);      // Dark red
        palette_[1] = Color(170, 0, 0);
        palette_[2] = Color(200, 0, 0);
        palette_[3] = Color(255, 0, 0);      // Red
        palette_[4] = Color(255, 64, 0);
        palette_[5] = Color(255, 128, 0);    // Orange
        palette_[6] = Color(255, 192, 0);
        palette_[7] = Color(255, 255, 0);    // Yellow
        palette_[8] = Color(255, 192, 0);
        palette_[9] = Color(255, 128, 0);
        palette_[10] = Color(255, 64, 0);
        palette_[11] = Color(255, 0, 0);
        palette_[12] = Color(200, 0, 0);
        palette_[13] = Color(170, 0, 0);
        palette_[14] = Color(128, 0, 0);
        palette_[15] = Color(96, 0, 0);
        break;
    }
  }

  Color color_from_palette(uint8_t index, uint8_t brightness) {
    // Use 16-entry palette and interpolate between adjacent entries for smooth blending.
    uint8_t idx = index >> 4;          // base index 0-15
    uint8_t frac = index & 0x0F;       // 0-15 fractional part
    uint8_t next = (idx == 15) ? 0 : (idx + 1);

    const Color &c1 = palette_[idx];
    const Color &c2 = palette_[next];

    // Interpolate with 4-bit fraction; scale to 0-16 weight
    uint8_t w2 = frac;         // 0..15
    uint8_t w1 = 16 - w2;      // complementary weight

    uint16_t r = (c1.r * w1 + c2.r * w2) >> 4;
    uint16_t g = (c1.g * w1 + c2.g * w2) >> 4;
    uint16_t b = (c1.b * w1 + c2.b * w2) >> 4;

    // Apply brightness (0-255)
    uint8_t rr = (r * brightness) >> 8;
    uint8_t gg = (g * brightness) >> 8;
    uint8_t bb = (b * brightness) >> 8;

    return Color(rr, gg, bb);
  }

  uint8_t cycle_s_{20};
  uint16_t scale_{8};
  ColorTwinklesPaletteType palette_type_{COLOR_TWINKLES_PALETTE_RAINBOW_COLORS};
  uint32_t last_update_{0};
  Color palette_[16];
};

}  // namespace light
}  // namespace esphome