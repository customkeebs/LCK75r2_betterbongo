/* Copyright 2021 Lyso1/ItsWaffle(oled code)
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "lck75.h"

bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) return false;
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return true;
}

#define IDLE_FRAMES 5
#define IDLE_SPEED 30
#define TAP_FRAMES 2
#define TAP_SPEED 40
#define ANIM_FRAME_DURATION 200
#define ANIM_SIZE 512

#ifdef OLED_ENABLE

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {
    // Paste the existing idle animation frames here
    // ...
};

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }

    static bool key_pressed = false;
    static uint8_t current_frame = 0;
    static uint32_t anim_timer = 0;
    static uint8_t current_idle_frame = 0;
    static uint32_t anim_sleep = IDLE_SPEED;

    if (keymap_is_pressed(KC_ANY)) {
        // Key press detected
        key_pressed = true;
        current_frame = 0;
    }

    if (key_pressed) {
        // Bongo cat hand-down animation
        oled_write_raw_P(idle[current_frame], ANIM_SIZE);
        if (timer_elapsed(anim_timer) > ANIM_FRAME_DURATION) {
            anim_timer = timer_read();
            current_frame++;
            if (current_frame >= IDLE_FRAMES) {
                current_frame = 0;
                key_pressed = false;
            }
        }
    } else {
        // Bongo cat idle animation
        if (timer_elapsed(anim_timer) > anim_sleep) {
            anim_timer = timer_read();
            current_idle_frame++;
            if (current_idle_frame >= IDLE_FRAMES) {
                current_idle_frame = 0;
            }
            anim_sleep = current_idle_frame == 0 ? IDLE_SPEED : TAP_SPEED;
        }
        oled_write_raw_P(idle[current_idle_frame], ANIM_SIZE);
    }

    return true;
}

#endif
