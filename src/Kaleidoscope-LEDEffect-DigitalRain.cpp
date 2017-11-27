#include "Kaleidoscope-LEDEffect-DigitalRain.h"
#include <stdlib.h>

namespace kaleidoscope {
	uint8_t LEDDigitalRainEffect::DROP_TICKS = 28;
	uint8_t LEDDigitalRainEffect::NEW_DROP_PROBABILITY = 18;
	uint8_t LEDDigitalRainEffect::PURE_GREEN_INTENSITY = 0xd0;
	uint8_t LEDDigitalRainEffect::MAXIMUM_BRIGHTNESS_BOOST = 0xc0;

	void LEDDigitalRainEffect::update(void) {
		uint8_t col;
		uint8_t row;
        uint8_t under;
		
		// Decay intensities and possibly make new raindrops
		for (col = 0; col < COLS; col++) {
			for (row = 0; row < ROWS; row++) {
				if (row == 0 && drop == 0 && rand() < RAND_MAX / NEW_DROP_PROBABILITY) {
					// This is the top row, pixels have just fallen,
					// and we've decided to make a new raindrop in this column
					map[col][row] = 0xff;
				} else if (map[col][row] > 0 && map[col][row] < 0xff) {
					// Pixel is neither full brightness nor totally dark;
					// decay it
					map[col][row]--;
				}

				// Set the colour for this pixel
				::LEDControl.setCrgbAt(row, col, getColor(map[col][row]));
			}
		}
        // set underglow
        for (under = LEFT_KEYS + RIGHT_KEYS; under < LEFT_KEYS + RIGHT_KEYS + LEFT_UNDERGLOW_LEDS + RIGHT_UNDERGLOW_LEDS; under ++)
            ::LEDControl.setCrgbAt(under, {0, underglow_breathe, 0 });

        underglow_breathe += underglow_sign;
        if (underglow_breathe >= PURE_GREEN_INTENSITY || underglow_breathe <= 0) {
            underglow_sign = -underglow_sign;
        }


		// Drop the raindrops one row periodically
		if (++drop >= DROP_TICKS) {
			// Reset the drop timer
			drop = 0;

			for (row = ROWS - 1; row > 0; row--) {
				for (col = 0; col < COLS; col++) {
					// If this pixel is on the bottom row and bright,
					// allow it to start decaying
					if (row == ROWS - 1 && map[col][row] == 0xff) {
						map[col][row]--;
					}

					// Check if the pixel above is bright
					if (map[col][row - 1] == 0xff) {
						// Allow old bright pixel to decay
						map[col][row - 1]--;

						// Make this pixel bright
						map[col][row] = 0xff;
					}
				}
			}
		}
	}

	cRGB LEDDigitalRainEffect::getColor(uint8_t intensity) {
		uint8_t boost;

		// At high intensities start at light green
		// but drop off very quickly to full green
		if (intensity > PURE_GREEN_INTENSITY) {
			boost = (uint8_t) ((uint16_t) MAXIMUM_BRIGHTNESS_BOOST
					* (intensity - PURE_GREEN_INTENSITY)
					/ (0xff - PURE_GREEN_INTENSITY));
			return {boost, 0xff, boost};
		}
		return {0, (uint8_t) ((uint16_t) 0xff * intensity / PURE_GREEN_INTENSITY), 0};
	}
}

kaleidoscope::LEDDigitalRainEffect LEDDigitalRainEffect;
