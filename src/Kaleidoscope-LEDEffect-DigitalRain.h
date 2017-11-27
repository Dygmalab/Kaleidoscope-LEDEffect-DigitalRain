#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

namespace kaleidoscope {
	class LEDDigitalRainEffect : public LEDMode {
		public:
			LEDDigitalRainEffect(void) {}

			/**
			 * Number of ticks before raindrops fall.
			 */
			static uint8_t DROP_TICKS;

			/**
			 * Probability divisor for new drops.
			 *
			 * The inverse of this number (1/n) gives the probability
			 * each time DROP_TICKS frames have elapsed
			 * that a new raindrop will appear in a given column.
			 */
			static uint8_t NEW_DROP_PROBABILITY;

			/**
			 * Intensity at which full green is used.
			 *
			 * When a pixel is at this intensity,
			 * full green with no red nor blue is used.
			 * Above this intensity red and blue are added
			 * to lighten the colour.
			 */
			static uint8_t PURE_GREEN_INTENSITY;

			/**
			 * Maximum brightness boost.
			 *
			 * The maximum brightness boost added to pure green
			 * when a pixel is at full intensity.
			 *
			 * 0xff would give full white for full-intensity pixels,
			 * while zero would give pure green for all intensities
			 * above PURE_GREEN_INTENSITY.
			 */
			static uint8_t MAXIMUM_BRIGHTNESS_BOOST;

		protected:
			void update(void) final;

		private:
			/**
			 * Timer position for raindrops falling.
			 *
			 * This ticks upwards linearly and resets after DROP_TICKS.
			 */
			uint8_t drop = 0;
            uint8_t underglow_breathe = 0;
            int8_t underglow_sign = 1; //negative values when it's going backward

			/**
			 * Map of intensities for each pixel.
			 *
			 * Intensity 0xff stays for a full cycle of DROP_TICKS,
			 * then linearly drops off to zero with time.
			 */
			uint8_t map[COLS][ROWS] = {{0}};

			/**
			 * Get colour from intensity.
			 */
			cRGB getColor(uint8_t intensity);
	};
}

extern kaleidoscope::LEDDigitalRainEffect LEDDigitalRainEffect;
