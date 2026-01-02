#pragma once

#include "esphome.h"
#include <vector>

namespace esphome {
namespace wled_effects {

class WLEDBlendsEffect : public light::AddressableLightEffect {
 public:
  explicit WLEDBlendsEffect(const std::string &name) : AddressableLightEffect(name) {}

  // "Targets" stores the destination color for each pixel
  std::vector<Color> targets;
  uint32_t last_cycle_time = 0;

  // Configuration: Adjust these to change the "Look"
  // 100 = fast changes, 800 = slow chill changes
  uint32_t SHIFT_DELAY = 100; 
  // 1 = very slow blend, 20 = instant snap
  uint8_t BLEND_SPEED = 5; 

  void apply(light::LightState &state, light::AddressableLight &it) override {
    // 1. Initialize buffer if strip size changes
    if (targets.size() != it.size()) {
      targets.resize(it.size(), Color::BLACK);
    }

    uint32_t now = millis();

    // 2. LOGIC: Pick new target colors occasionally
    if (now - last_cycle_time > SHIFT_DELAY) {
      for (int i = 0; i < it.size(); i++) {
         // 1 in 10 chance for a pixel to change its target color this cycle
         if ((random_uint32() % 10) == 0) {
            // Pick a random color (RGB)
            // Note: You could modify this to pick from a specific palette if desired
            targets[i] = Color(random_uint32() % 256, random_uint32() % 256, random_uint32() % 256);
         }
      }
      last_cycle_time = now;
    }

    // 3. RENDER: Smoothly move Current Color -> Target Color
    for (int i = 0; i < it.size(); i++) {
      Color current = it[i].get();
      Color target = targets[i];

      // If we are already at the target, skip math to save CPU
      if (current == target) continue;

      // Helper lambda to move one channel (R, G, or B) closer to target
      auto approach = [&](uint8_t curr, uint8_t tgt) -> uint8_t {
        if (curr == tgt) return curr;
        if (curr < tgt) return (tgt - curr < BLEND_SPEED) ? tgt : curr + BLEND_SPEED;
        else            return (curr - tgt < BLEND_SPEED) ? tgt : curr - BLEND_SPEED;
      };

      it[i] = Color(
          approach(current.r, target.r),
          approach(current.g, target.g),
          approach(current.b, target.b),
          approach(current.w, target.w) // Include White channel for SK6812 support
      );
    }
  }
};

} // namespace wled_effects
} // namespace esphome