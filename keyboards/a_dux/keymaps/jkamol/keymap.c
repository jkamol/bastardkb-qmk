// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H

// Seniply layout
// https://stevep99.github.io/seniply

enum layer_names {
    _MAIN,
    _QWERTY,
    _SHORTCUT,
    _SYM,
    _NAV,
    _FN,
    _NUMPAD,
    _MACRO,
};

// Start Super ALT↯TAB and Mac/Win mode
bool is_alt_tab_active = false; // ADD this near the beginning of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.

enum custom_keycodes {          // Make sure have the awesome keycode ready
  ALT_TAB = SAFE_RANGE,
  ALT_SFT_TAB,
  MAC_MODE,
  WIN_MODE,
  SHCT_TG,
  HTTPS,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) { // This will do most of the grunt work with the keycodes.
    case ALT_TAB:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
    case ALT_SFT_TAB:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_LSFT);
        register_code(KC_TAB);
      } else {
        unregister_code(KC_LSFT);
        unregister_code(KC_TAB);
      }
      break;
    case MAC_MODE:
      if (record->event.pressed) {
        keymap_config.swap_lalt_lgui = true;
        keymap_config.swap_rctl_rgui = true;
      }
      break;
    case WIN_MODE:
      if (record->event.pressed) {
        keymap_config.swap_lalt_lgui = false;
        keymap_config.swap_rctl_rgui = false;
      }
      break;
    case SHCT_TG:
      if (record->event.pressed) {
        if (layer_state_is(_SHORTCUT)) {
          layer_move(_MAIN);
        } else {
          layer_move(_SHORTCUT);
        }
      }
      break;
    case HTTPS:
      if (record->event.pressed) {
        SEND_STRING("https://");
      }
      break;
  }
  return true;
}

void matrix_scan_user(void) { // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}
// End Super ALT↯TAB and Mac/Win mode

enum tap_dance_codes {
    DANCE_QUIT,
    DANCE_TASK,
    DANCE_QWERTY,
    DANCE_PWR,
    DANCE_SCR,
    DANCE_FN,
    DANCE_MAIN,
    DANCE_NLK,
    DANCE_SHCT,
    DANCE_COPY,
};

enum combo_events {
    ENTER,
    ESC,
    SHORTCUT,
    FN_TO,
    LANG_SW,
    MACRO_CR,
    MACRO_LT,
    MACRO_RT,
    NUM_MO,
    NUM_TO,
    LOCK_NAV,
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM enter_combo[]    = {LALT_T(KC_R), RCTL_T(KC_S), COMBO_END};
const uint16_t PROGMEM escape_combo[]   = {KC_L, KC_D, COMBO_END};
const uint16_t PROGMEM shct_combo[]     = {LT(_SYM,KC_SPACE), RCTL_T(KC_TAB), COMBO_END};
const uint16_t PROGMEM fn_to_combo[]    = {KC_N, KC_H, KC_M, RGUI_T(KC_SCLN), COMBO_END};
const uint16_t PROGMEM lang_sw_combo[]  = {LSFT_T(KC_C), LALT_T(KC_R), COMBO_END};
const uint16_t PROGMEM macro_cr_combo[] = {LT(_SYM,KC_SPACE), LT(_NAV,KC_E), COMBO_END};
const uint16_t PROGMEM macro_lt_combo[] = {KC_G, LGUI_T(KC_K), COMBO_END};
const uint16_t PROGMEM macro_rt_combo[] = {KC_M, RGUI_T(KC_SCLN), COMBO_END};
const uint16_t PROGMEM num_mo_combo[]   = {KC_T, KC_W, COMBO_END};
const uint16_t PROGMEM num_to_combo[]   = {KC_T, KC_W, KC_G, LGUI_T(KC_K), COMBO_END};
const uint16_t PROGMEM lock_nav[]       = {LT(_NAV,KC_E), LT(_FN,KC_ENTER), COMBO_END};

combo_t key_combos[] = {
    [ENTER]    = COMBO(enter_combo, KC_ENTER),
    [ESC]      = COMBO(escape_combo, KC_ESCAPE),
    [SHORTCUT] = COMBO(shct_combo, TD(DANCE_SHCT)),
    [FN_TO]    = COMBO(fn_to_combo, TO(_FN)),
    [LANG_SW]  = COMBO(lang_sw_combo, LALT(KC_LSFT)),
    [MACRO_CR] = COMBO(macro_cr_combo, MO(_MACRO)),
    [MACRO_LT] = COMBO(macro_lt_combo, MO(_MACRO)),
    [MACRO_RT] = COMBO(macro_rt_combo, MO(_MACRO)),
    [NUM_MO]   = COMBO(num_mo_combo, MO(_NUMPAD)),
    [NUM_TO]   = COMBO(num_to_combo, TO(_NUMPAD)),
    [LOCK_NAV] = COMBO(lock_nav, TO(_NAV)),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_MAIN] = LAYOUT_split_3x5_2(
        C_S_T(KC_Q),    KC_L,           KC_D,           KC_P,           ALGR_T(KC_B),       /**/ ALGR_T(KC_J),      KC_F,           KC_O,           KC_U,           C_S_T(KC_QUOTE),
        LSFT_T(KC_C),   LALT_T(KC_R),   RCTL_T(KC_S),   KC_T,           KC_G,               /**/ KC_M,              KC_N,           KC_A,           KC_I,           LSFT_T(KC_Y),
        RCTL_T(KC_Z),   KC_X,           KC_V,           KC_W,           LGUI_T(KC_K),       /**/ RGUI_T(KC_SCLN),   KC_H,           KC_SLASH,       KC_COMMA,       RCTL_T(KC_DOT),
                                                        RCTL_T(KC_TAB), LT(_SYM,KC_SPACE),  /**/ LT(_NAV,KC_E),     LT(_FN,KC_ENTER)
    ),
    [_QWERTY] = LAYOUT_split_3x5_2(
        KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,               /**/ KC_Y,              KC_U,           KC_I,           KC_O,           KC_P,
        KC_A,           KC_S,           KC_D,           KC_F,           KC_G,               /**/ KC_H,              KC_J,           KC_K,           KC_L,           KC_SCLN,
        KC_Z,           KC_X,           KC_C,           KC_V,           KC_B,               /**/ KC_N,              KC_M,           KC_COMMA,       KC_DOT,         KC_SLASH,
                                                        _______,        _______,            /**/ _______,           _______
    ),
    [_SHORTCUT] = LAYOUT_split_3x5_2(
        TO(_MAIN),      KC_HOME,        KC_UP,          KC_END,         KC_MS_BTN2,         /**/ XXXXXXX,           XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,
        C(KC_A),        KC_LEFT,        KC_DOWN,        KC_RIGHT,       KC_MS_BTN1,         /**/ XXXXXXX,           KC_MS_BTN1,     KC_MS_BTN2,     XXXXXXX,        XXXXXXX,
        C(KC_Z),        KC_BSPC,        C(KC_C),        C(KC_V),        C(KC_Y),            /**/ XXXXXXX,           HTTPS,          XXXXXXX,        XXXXXXX,        XXXXXXX,
                                                        _______,        _______,            /**/ _______,           _______
    ),
    [_SYM] = LAYOUT_split_3x5_2(
        TD(DANCE_QUIT), KC_7,           TD(DANCE_TASK), KC_9,           KC_0,               /**/ KC_CIRCUMFLEX,     KC_AMPERSAND,   KC_ASTERISK,    KC_UNDERSCORE,  KC_DOLLAR,
        LSFT_T(KC_1),   KC_2,           KC_3,           KC_4,           KC_5,               /**/ KC_PERCENT,        KC_MINUS,       KC_AT,          KC_EQUAL,       LSFT_T(KC_GRAVE),
        RCTL_T(KC_KP_DOT), KC_BSPC,     TD(DANCE_SCR),  KC_DEL,         QK_REP,             /**/ KC_EXCLAIM,        KC_HASH,        KC_BACKSLASH,   KC_LBRC,        RCTL_T(KC_RBRC),
                                                        MO(_FN),        XXXXXXX,            /**/ MO(_MACRO),        _______
    ),
    [_NAV] = LAYOUT_split_3x5_2(
        RCTL(KC_LSFT),  KC_MS_WH_UP,    KC_MS_UP,       KC_MS_BTN3,     KC_LALT,            /**/ KC_ACL0,           KC_APPLICATION, KC_UP,          KC_PAGE_UP,     KC_CAPS_LOCK,
        TD(DANCE_COPY), KC_MS_LEFT,     KC_MS_DOWN,     KC_MS_RIGHT,    KC_INSERT,          /**/ KC_HOME,           KC_LEFT,        KC_DOWN,        KC_RIGHT,       LSFT_T(KC_END),
        KC_RCTL,        KC_MS_WH_DOWN,  C(KC_V),        C(KC_W),        KC_LGUI,            /**/ KC_BSPC,           KC_DELETE,      LALT(KC_LSFT),  KC_PAGE_DOWN,   KC_RCTL,
                                                        KC_MS_BTN2,     KC_MS_BTN1,         /**/ TO(_MAIN),         KC_ENTER
    ),
    [_FN] = LAYOUT_split_3x5_2(
        KC_1,           KC_2,           KC_3,           KC_4,           KC_5,               /**/ KC_SCROLL_LOCK,    TD(DANCE_PWR),  LCA(KC_DELETE), KC_VOLU,        KC_PAUSE,
        KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,              /**/ KC_F12,            KC_MPRV,        KC_MPLY,        KC_MUTE,        KC_MNXT,
        KC_F6,          KC_F7,          KC_F8,          KC_F9,          KC_F10,             /**/ KC_F11,            XXXXXXX,        KC_MEH,         KC_VOLD,        KC_RCTL,
                                                        MO(_NUMPAD),    KC_LALT,            /**/ KC_TAB,            TO(_MAIN)
    ),
    [_NUMPAD] = LAYOUT_split_3x5_2(
        KC_Q,           KC_M,           KC_F,           KC_P,           KC_T,               /**/ TD(DANCE_NLK),     KC_KP_7,        KC_KP_8,        KC_KP_9,        KC_KP_MINUS,
        KC_1,           KC_2,           KC_3,           KC_4,           TD(DANCE_MAIN),     /**/ KC_KP_PLUS,        KC_KP_4,        KC_KP_5,        KC_KP_6,        KC_KP_DOT,
        RCTL_T(KC_Z),   KC_X,           KC_C,           KC_S,           KC_B,               /**/ KC_BSPC,           KC_KP_1,        KC_KP_2,        KC_KP_3,        KC_KP_SLASH,
                                                        _______,        KC_LALT,            /**/ KC_KP_0,           _______
    ),
    [_MACRO] = LAYOUT_split_3x5_2(
        TD(DANCE_QWERTY), DM_REC1,      DM_REC2,        XXXXXXX,        XXXXXXX,            /**/ XXXXXXX,           MAC_MODE,       WIN_MODE,       XXXXXXX,        QK_SWAP_HANDS_TOGGLE,
        LSFT_T(KC_1),   KC_2,           KC_3,           KC_4,           KC_5,               /**/ KC_6,              KC_7,           KC_8,           KC_9,           LSFT_T(KC_0),
        DM_RSTP,        DM_PLY1,        DM_PLY2,        XXXXXXX,        XXXXXXX,            /**/ KC_BSPC,           KC_DELETE,      KC_SLASH,       KC_COMMA,       RCTL_T(KC_DOT),
                                                        LALT(KC_LEFT),  LALT(KC_RIGHT),     /**/ _______,           _______
    ),
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t'
static td_tap_t tap_state;

void on_DANCE_QUIT(tap_dance_state_t *state, void *user_data);
void DANCE_QUIT_finished(tap_dance_state_t *state, void *user_data);
void DANCE_QUIT_reset(tap_dance_state_t *state, void *user_data);

void on_DANCE_QUIT(tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_6);
        tap_code16(KC_6);
        tap_code16(KC_6);
    }
    if(state->count > 3) {
        tap_code16(KC_6);
    }
}

void DANCE_QUIT_finished(tap_dance_state_t *state, void *user_data) {
    tap_state.state = cur_dance(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         register_code16(KC_6); break;
        case TD_SINGLE_HOLD:        register_code16(LALT(KC_F4)); break;
        case TD_DOUBLE_TAP:         register_code16(KC_6); register_code16(KC_6); break;
        case TD_DOUBLE_HOLD:        register_code16(LALT(KC_F4)); break;
        case TD_DOUBLE_SINGLE_TAP:  tap_code16(KC_6); register_code16(KC_6); break;
        default: break;
    }
}

void DANCE_QUIT_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         unregister_code16(KC_6); break;
        case TD_SINGLE_HOLD:        unregister_code16(LALT(KC_F4)); break;
        case TD_DOUBLE_TAP:         unregister_code16(KC_6); break;
        case TD_DOUBLE_HOLD:        unregister_code16(LALT(KC_F4)); break;
        case TD_DOUBLE_SINGLE_TAP:  unregister_code16(KC_6); break;
        default: break;
    }
    tap_state.state = TD_NONE;
}


void on_DANCE_TASK(tap_dance_state_t *state, void *user_data);
void DANCE_TASK_finished(tap_dance_state_t *state, void *user_data);
void DANCE_TASK_reset(tap_dance_state_t *state, void *user_data);

void on_DANCE_TASK(tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_8);
        tap_code16(KC_8);
        tap_code16(KC_8);
    }
    if(state->count > 3) {
        tap_code16(KC_8);
    }
}

void DANCE_TASK_finished(tap_dance_state_t *state, void *user_data) {
    tap_state.state = cur_dance(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         register_code16(KC_8); break;
        case TD_SINGLE_HOLD:        register_code16(RCS(KC_ESC)); break;
        case TD_DOUBLE_TAP:         register_code16(KC_8); register_code16(KC_8); break;
        case TD_DOUBLE_HOLD:        register_code16(RCS(KC_ESC)); break;
        case TD_DOUBLE_SINGLE_TAP:  tap_code16(KC_8); register_code16(KC_8); break;
        default: break;
    }
}

void DANCE_TASK_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         unregister_code16(KC_8); break;
        case TD_SINGLE_HOLD:        unregister_code16(RCS(KC_ESC)); break;
        case TD_DOUBLE_TAP:         unregister_code16(KC_8); break;
        case TD_DOUBLE_HOLD:        unregister_code16(RCS(KC_ESC)); break;
        case TD_DOUBLE_SINGLE_TAP:  unregister_code16(KC_8); break;
        default: break;
    }
    tap_state.state = TD_NONE;
}


void DANCE_QWERTY_finished(tap_dance_state_t *state, void *user_data);
void DANCE_QWERTY_reset(tap_dance_state_t *state, void *user_data);

void DANCE_QWERTY_finished(tap_dance_state_t *state, void *user_data) {
    tap_state.state = cur_dance(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         layer_move(_MAIN); break;
        case TD_SINGLE_HOLD:        layer_move(_QWERTY); break;
        case TD_DOUBLE_TAP:         layer_move(_MAIN); break;
        case TD_DOUBLE_HOLD:        layer_move(_QWERTY); break;
        case TD_DOUBLE_SINGLE_TAP:  layer_move(_MAIN); break;
        default: break;
    }
}

void DANCE_QWERTY_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (tap_state.state) {
        default: break;
    }
    tap_state.state = TD_NONE;
}


void DANCE_PWR_finished(tap_dance_state_t *state, void *user_data);
void DANCE_PWR_reset(tap_dance_state_t *state, void *user_data);

void DANCE_PWR_finished(tap_dance_state_t *state, void *user_data) {
    tap_state.state = cur_dance(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         register_code16(KC_CALCULATOR); break;
        case TD_DOUBLE_HOLD:        register_code16(KC_SYSTEM_POWER); break;
        default: break;
    }
}

void DANCE_PWR_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         unregister_code16(KC_CALCULATOR); break;
        case TD_DOUBLE_HOLD:        unregister_code16(KC_SYSTEM_POWER); break;
        default: break;
    }
    tap_state.state = TD_NONE;
}


void on_DANCE_SCR(tap_dance_state_t *state, void *user_data);
void DANCE_SCR_finished(tap_dance_state_t *state, void *user_data);
void DANCE_SCR_reset(tap_dance_state_t *state, void *user_data);

void on_DANCE_SCR(tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_PRINT_SCREEN);
        tap_code16(KC_PRINT_SCREEN);
        tap_code16(KC_PRINT_SCREEN);
    }
    if(state->count > 3) {
        tap_code16(KC_PRINT_SCREEN);
    }
}

void DANCE_SCR_finished(tap_dance_state_t *state, void *user_data) {
    tap_state.state = cur_dance(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         register_code16(KC_PRINT_SCREEN); break;
        case TD_SINGLE_HOLD:        register_code16(LCTL(LSFT(KC_S))); break;
        case TD_DOUBLE_TAP:         register_code16(KC_PRINT_SCREEN); register_code16(KC_PRINT_SCREEN); break;
        case TD_DOUBLE_HOLD:        register_code16(LCTL(LSFT(KC_S))); break;
        case TD_DOUBLE_SINGLE_TAP:  tap_code16(KC_PRINT_SCREEN); register_code16(KC_PRINT_SCREEN); break;
        default: break;
    }
}

void DANCE_SCR_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         unregister_code16(KC_PRINT_SCREEN); break;
        case TD_SINGLE_HOLD:        unregister_code16(LCTL(LSFT(KC_S))); break;
        case TD_DOUBLE_TAP:         unregister_code16(KC_PRINT_SCREEN); break;
        case TD_DOUBLE_HOLD:        unregister_code16(LCTL(LSFT(KC_S))); break;
        case TD_DOUBLE_SINGLE_TAP:  unregister_code16(KC_PRINT_SCREEN); break;
        default: break;
    }
    tap_state.state = TD_NONE;
}


void on_DANCE_MAIN(tap_dance_state_t *state, void *user_data);
void DANCE_MAIN_finished(tap_dance_state_t *state, void *user_data);
void DANCE_MAIN_reset(tap_dance_state_t *state, void *user_data);

void on_DANCE_MAIN(tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_5);
        tap_code16(KC_5);
        tap_code16(KC_5);
    }
    if(state->count > 3) {
        tap_code16(KC_5);
    }
}

void DANCE_MAIN_finished(tap_dance_state_t *state, void *user_data) {
    tap_state.state = cur_dance(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         register_code16(KC_5); break;
        case TD_DOUBLE_TAP:         register_code16(KC_5); register_code16(KC_5); break;
        case TD_DOUBLE_HOLD:        layer_move(_MAIN); break;
        case TD_DOUBLE_SINGLE_TAP:  tap_code16(KC_5); register_code16(KC_5); break;
        default: break;
    }
}

void DANCE_MAIN_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         unregister_code16(KC_5); break;
        case TD_DOUBLE_TAP:         unregister_code16(KC_5); break;
        case TD_DOUBLE_SINGLE_TAP:  unregister_code16(KC_5); break;
        default: break;
    }
    tap_state.state = TD_NONE;
}


void on_DANCE_NLK(tap_dance_state_t *state, void *user_data);
void DANCE_NLK_finished(tap_dance_state_t *state, void *user_data);
void DANCE_NLK_reset(tap_dance_state_t *state, void *user_data);

void on_DANCE_NLK(tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_KP_ASTERISK);
        tap_code16(KC_KP_ASTERISK);
        tap_code16(KC_KP_ASTERISK);
    }
    if(state->count > 3) {
        tap_code16(KC_KP_ASTERISK);
    }
}

void DANCE_NLK_finished(tap_dance_state_t *state, void *user_data) {
    tap_state.state = cur_dance(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         register_code16(KC_KP_ASTERISK); break;
        case TD_DOUBLE_TAP:         register_code16(KC_KP_ASTERISK); register_code16(KC_KP_ASTERISK); break;
        case TD_DOUBLE_HOLD:        register_code16(KC_NUM_LOCK); break;
        case TD_DOUBLE_SINGLE_TAP:  tap_code16(KC_KP_ASTERISK); register_code16(KC_KP_ASTERISK); break;
        default: break;
    }
}

void DANCE_NLK_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         unregister_code16(KC_KP_ASTERISK); break;
        case TD_DOUBLE_TAP:         unregister_code16(KC_KP_ASTERISK); break;
        case TD_DOUBLE_HOLD:        unregister_code16(KC_NUM_LOCK); break;
        case TD_DOUBLE_SINGLE_TAP:  unregister_code16(KC_KP_ASTERISK); break;
        default: break;
    }
    tap_state.state = TD_NONE;
}

void DANCE_SHCT_finished(tap_dance_state_t *state, void *user_data);
void DANCE_SHCT_reset(tap_dance_state_t *state, void *user_data);

void DANCE_SHCT_finished(tap_dance_state_t *state, void *user_data) {
    tap_state.state = cur_dance(state);
    switch (tap_state.state) {
        case TD_SINGLE_TAP:         
            if (layer_state_is(_SHORTCUT)) {
                layer_move(_MAIN);
            } else {
                layer_move(_SHORTCUT);
            }
            break;
        case TD_SINGLE_HOLD:        layer_on(_SHORTCUT); break;
        case TD_DOUBLE_TAP:         
            if (layer_state_is(_SHORTCUT)) {
                layer_move(_MAIN);
            } else {
                layer_move(_SHORTCUT);
            }
            break;
        default: break;
    }
}

void DANCE_SHCT_reset(tap_dance_state_t *state, void *user_data) {
    switch (tap_state.state) {
        case TD_SINGLE_HOLD:        layer_off(_SHORTCUT); break;
        default: break;
    }
    tap_state.state = TD_NONE;
}

void on_DANCE_COPY(tap_dance_state_t *state, void *user_data);
void DANCE_COPY_finished(tap_dance_state_t *state, void *user_data); 
void DANCE_COPY_reset(tap_dance_state_t *state, void *user_data);

void on_DANCE_COPY(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1 && state->pressed) {
        register_code(KC_LSFT); // Instant Shift on hold
    }
}

void DANCE_COPY_finished(tap_dance_state_t *state, void *user_data) {
    tap_state.state = cur_dance(state);

    switch (tap_state.state) {
        case TD_SINGLE_TAP:
            // Temporarily drop Shift so we don't send Ctrl+Shift+C
            unregister_code(KC_LSFT);
            register_code(KC_LCTL);
            tap_code(KC_C);
            unregister_code(KC_LCTL);
            register_code(KC_LSFT); // Restore Shift if it was held
            break;

        case TD_SINGLE_HOLD:
            // Shift already registered in on_DANCE_COPY()
            break;

        default:
            break;
    }
}

void DANCE_COPY_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    unregister_code(KC_LCTL); // Safety: release Ctrl if stuck
    unregister_code(KC_C);    // Safety: release C if stuck
    unregister_code(KC_LSFT); // Always release Shift
    tap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [DANCE_QUIT]   = ACTION_TAP_DANCE_FN_ADVANCED(on_DANCE_QUIT, DANCE_QUIT_finished, DANCE_QUIT_reset),
    [DANCE_TASK]   = ACTION_TAP_DANCE_FN_ADVANCED(on_DANCE_TASK, DANCE_TASK_finished, DANCE_TASK_reset),
    [DANCE_QWERTY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, DANCE_QWERTY_finished, DANCE_QWERTY_reset),
    [DANCE_PWR]    = ACTION_TAP_DANCE_FN_ADVANCED(NULL, DANCE_PWR_finished, DANCE_PWR_reset),
    [DANCE_SCR]    = ACTION_TAP_DANCE_FN_ADVANCED(on_DANCE_SCR, DANCE_SCR_finished, DANCE_SCR_reset),
    [DANCE_MAIN]   = ACTION_TAP_DANCE_FN_ADVANCED(on_DANCE_MAIN, DANCE_MAIN_finished, DANCE_MAIN_reset),
    [DANCE_NLK]    = ACTION_TAP_DANCE_FN_ADVANCED(on_DANCE_NLK, DANCE_NLK_finished, DANCE_NLK_reset),
    [DANCE_SHCT]   = ACTION_TAP_DANCE_FN_ADVANCED(NULL, DANCE_SHCT_finished, DANCE_SHCT_reset),
    [DANCE_COPY]   = ACTION_TAP_DANCE_FN_ADVANCED(on_DANCE_COPY, DANCE_COPY_finished, DANCE_COPY_reset),
};
