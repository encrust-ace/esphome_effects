# ESPHome Custom Addressable Effects

A collection of custom addressable LED effects for ESPHome.

## Installation

Add the following to your ESPHome configuration:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/AntorFr/esphome-addressable-effects
      ref: main
    components: [custom_addressable_effects]

custom_addressable_effects:
```

Then add the effects to your light configuration:

```yaml
light:
  - platform: neopixelbus  # or esp32_rmt_led_strip, fastled_clockless, etc.
    # ... your light config ...
    effects:
      - addressable_twinklefox:
          name: "TwinkleFox"
      - addressable_color_twinkles:
          name: "Color Twinkles"
      - addressable_stars:
          name: "Stars"
```

## Available Effects

### TwinkleFox

A twinkling effect inspired by the FastLED TwinkleFox pattern. LEDs randomly twinkle with colors from a selected palette.

```yaml
- addressable_twinklefox:
    name: "TwinkleFox"
    palette: party_colors      # Color palette (see below)
    twinkle_speed: 4           # Speed of twinkle animation (1-8, default: 4)
    twinkle_density: 5         # How many LEDs twinkle at once (1-8, default: 5)
    cool_like_incandescent: true  # Fade to warm colors like incandescent bulbs (default: true)
    auto_background: false     # Automatically set background from palette (default: false)
    color:                     # Background color (when auto_background is false)
      red: 0%
      green: 0%
      blue: 0%
```

#### Available Palettes

| Palette | Description |
|---------|-------------|
| `party_colors` | Vibrant party colors (default) |
| `rainbow_colors` | Full rainbow spectrum |
| `ocean_colors` | Blues and greens |
| `lava_colors` | Reds, oranges, and yellows |
| `forest_colors` | Greens and earth tones |
| `snow_colors` | Whites and light blues |
| `holly_colors` | Christmas red and green |
| `ice_colors` | Cool whites and blues |
| `fairy_light` | Warm white incandescent look |
| `retro_c9` | Classic C9 Christmas bulb colors |

### Color Twinkles

LEDs twinkle with colors from a selected palette, smoothly fading in and out.

```yaml
- addressable_color_twinkles:
    name: "Color Twinkles"
    palette: rainbow_colors    # Color palette (see below)
    starting_brightness: 64    # Initial brightness when a twinkle starts (0-255, default: 64)
    fade_in_speed: 8           # Speed of fade in (0-255, default: 8)
    fade_out_speed: 4          # Speed of fade out (0-255, default: 4)
    density: 80                # Probability of new twinkles (0-255, default: 80)
```

#### Available Palettes

| Palette | Description |
|---------|-------------|
| `rainbow_colors` | Full rainbow spectrum (default) |
| `cloud_colors` | Blues and whites, like clouds |
| `snow_colors` | Whites and light blues |
| `incandescent` | Warm incandescent colors |
| `party_colors` | Vibrant party colors |
| `ocean_colors` | Ocean blues and greens |
| `forest_colors` | Forest greens |
| `lava_colors` | Lava reds and oranges |

### Stars

LEDs randomly light up and fade like twinkling stars.

```yaml
- addressable_stars:
    name: "Stars"
    stars_probability: 10%     # Probability of a new star appearing (default: 10%)
    color:                     # Star color (uses light color if all zeros)
      red: 0%
      green: 0%
      blue: 0%
      white: 0%
```

## Compatibility

- ESPHome 2025.11.0 and later
- Works with all addressable LED platforms (neopixelbus, fastled_clockless, esp32_rmt_led_strip, etc.)
- Tested on ESP8266 and ESP32

## License

MIT License - See [LICENSE](LICENSE) file for details.