
#include QMK_KEYBOARD_H


#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define	 _BASE		0		
#define	 _FN_EMACS     	1	
#define	 _LH_FN		2	
#define	 _LH_NUMS	3	
#define	 _RH_NUMS	4	
#define	 _LH_MOVE	5	
#define	 _RH_MOVE	6	
#define	 _PLOVER	14	
#define	 _TOP 	        15

enum custom_keycodes {
  TC_CCQ = SAFE_RANGE
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

// Tap dance stuff

typedef struct {
	bool is_press_action;
	int state;
} tap;

enum {
	SINGLE_TAP = 1,
	SINGLE_HOLD = 2, 
	DOUBLE_TAP = 3,
	DOUBLE_HOLD = 4,
	DOUBLE_SINGLE_TAP = 5,
	TRIPLE_TAP = 6,
	TRIPLE_HOLD = 7
};

enum td_keycodes {
	//space, enter
	TD_SPC_ENT = 0,
	TD_COMM,
	TD_PER,
	TD_Q_TAB,
	TD_PAREN,
	TD_CPAREN,
	TD_SCL_APO,
	TD_RESET,
	TD_KILL
};

int cur_dance (qk_tap_dance_state_t *state) {
	if (state->count == 1) {
		//if key is interrupted or normally tapped, you want single tap
		//else, you're holding
		if (state->interrupted || !state->pressed) return SINGLE_TAP;
		else return SINGLE_HOLD;
	} else if (state->count == 2) {
		//if interrupted, you wanted a double single tap,
		//but if held, you want a double hold,
		//but if neither of those, you just want a normal double tap!
		if (state->interrupted) return DOUBLE_SINGLE_TAP;
		else if (state->pressed) return DOUBLE_HOLD;
		else return DOUBLE_TAP;
	}
	//I don't know why he didn't use an else if here
	if (state->count == 3) {
		if (state->interrupted || !state->pressed) return TRIPLE_TAP;
		else return TRIPLE_HOLD;
	} else {
		return 8; //because why not?
	}
}


void hit_comma_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 2) {
		register_code (KC_LSFT);
		register_code (KC_1);
	} else {
		register_code (KC_COMM);
	}
}

void hit_comma_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 2) {
		unregister_code (KC_LSFT);
		unregister_code (KC_1);
	} else {
		unregister_code (KC_COMM);
	}
}


void hit_period_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 2) {
		register_code (KC_LSFT);
		register_code (KC_SLSH);
	} else {
		register_code (KC_DOT);
	}
}

void hit_period_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 2) {
		unregister_code (KC_LSFT);
		unregister_code (KC_SLSH);
	} else {
		unregister_code (KC_DOT);
	}
}


void hit_paren_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 2) {
		register_code (KC_LBRC);
	} else if (state->count == 3) {
		register_code (KC_LSFT);
		register_code (KC_LBRC);
	} else {
		register_code (KC_LSFT);
		register_code (KC_9);
	}
}

void hit_paren_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 2) {
		unregister_code (KC_LBRC);
	} else if (state->count == 3) {
		unregister_code (KC_LSFT);
		unregister_code (KC_LBRC);
	} else {
		unregister_code (KC_LSFT);
		unregister_code (KC_9);
	}
}


void hit_cparen_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 2) {
		register_code (KC_RBRC);
	} else if (state->count == 3) {
		register_code (KC_LSFT);
		register_code (KC_RBRC);
	} else {
		register_code (KC_LSFT);
		register_code (KC_0);
	}
}

void hit_cparen_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count == 2) {
		unregister_code (KC_RBRC);
	} else if (state->count == 3) {
		unregister_code (KC_LSFT);
		unregister_code (KC_RBRC);
	} else {
		unregister_code (KC_LSFT);
		unregister_code (KC_0);
	}
}


static tap td_scl_apo_state = {
	.is_press_action = true,
	.state = 0
};

void hit_scl_finished (qk_tap_dance_state_t *state, void *user_data) {
	td_scl_apo_state.state = cur_dance(state);
	switch (td_scl_apo_state.state) {
		case SINGLE_TAP: register_code(KC_QUOT); break;
		case SINGLE_HOLD: register_code(KC_RCTL); break;
		case DOUBLE_TAP: register_code(KC_SCLN); break;
		case DOUBLE_HOLD: register_code(KC_RCTL); break;
		case DOUBLE_SINGLE_TAP: break;
	}
}

void hit_scl_reset (qk_tap_dance_state_t *state, void *user_data) {
	td_scl_apo_state.state = cur_dance(state);
	switch (td_scl_apo_state.state) {
		case SINGLE_TAP: unregister_code(KC_QUOT); break;
		case SINGLE_HOLD: unregister_code(KC_RCTL); break;
		case DOUBLE_TAP: unregister_code(KC_SCLN); break;
		case DOUBLE_HOLD: unregister_code(KC_RCTL); break;
		case DOUBLE_SINGLE_TAP: break;
	}
	unregister_code(KC_RCTL);
	td_scl_apo_state.state = 0;
}


void hit_reset_finished (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count >= 5) {
		reset_keyboard();
		reset_tap_dance(state);
	} else {
		register_code(KC_ESC);
	}
}

void hit_reset_reset (qk_tap_dance_state_t *state, void *user_data) {
	if (state->count >= 5) {
		reset_keyboard();
		reset_tap_dance(state);
	} else {
		unregister_code(KC_ESC);
	}
}


void kill (qk_tap_dance_state_t *state, void *user_data) {
// This function is meant to replicate the kill command (naievely)

	// move the cursor to the beginning of the line (good if you want a delete line function)
	//register_code (KC_HOME);
	//unregister_code (KC_HOME);

	// shift select the whole line
	register_code (KC_LSFT);
	register_code (KC_END);
	unregister_code (KC_END);
	unregister_code (KC_LSFT);

	// the cut command could go here, but the clipboard on Windows is trash, 
	// so I'm not going to enable it by default, but I will put the copy option
	// here just in case
	//register_code (KC_COPY); // actually, the copy keycode doesn't seem to work

	// delete the line
	register_code (KC_BSPC);
	unregister_code (KC_BSPC);
}


qk_tap_dance_action_t tap_dance_actions[] = {
	// hit once for SPC, twice for ENT
	// [TD_SPC_ENT] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, hit_spc_finished, hit_spc_reset),

	// hit the comma key 1x for comma, 2x for question mark
	[TD_COMM] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, hit_comma_finished, hit_comma_reset),

	// hit period once for period, twice for exclamation mark
	[TD_PER] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, hit_period_finished, hit_period_reset),

	// hit Q once for q, twice for tab
	// [TD_Q_TAB] = ACTION_TAP_DANCE_DOUBLE (KC_Q, KC_TAB),

	// open and close parentheses
	[TD_PAREN] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, hit_paren_finished, hit_paren_reset),
	[TD_CPAREN] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, hit_cparen_finished, hit_cparen_reset),

	// send apostrophe when tapped, semicoln on double tap
	[TD_SCL_APO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, hit_scl_finished, hit_scl_reset),

	// escape until tapdance 5 times, then reset
	[TD_RESET] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, hit_reset_finished, hit_reset_reset),

	// kill function
	[TD_KILL] = ACTION_TAP_DANCE_FN (kill)
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT( \

      TD(TD_RESET),		KC_Q,		KC_W,		KC_E,	KC_R,		KC_T,	KC_Y,	KC_U,		KC_I,			KC_O,		KC_P,		TD(TD_PAREN),	\

      LT(_LH_NUMS,KC_TAB),	CTL_T(KC_A),	ALT_T(KC_S),	KC_D,	LGUI_T(KC_F),	KC_G,	KC_H,	RGUI_T(KC_J),	LT(_RH_NUMS,KC_K),	RALT_T(KC_L),	TD(TD_SCL_APO),	TD(TD_CPAREN),	\

      LT(_LH_MOVE,KC_GRAVE),	SFT_T(KC_Z),	KC_X,		KC_C,	KC_V,		KC_B,	KC_N,	KC_M,		TD(TD_COMM),		TD(TD_PER),	SFT_T(KC_SLSH),	KC_BSLS,	\

				      	XXXXXXX,	LT(_LH_FN,KC_ENT),	KC_BSPC,	  LT(_LH_FN,KC_SPC),	LT(_FN_EMACS,KC_MINS),	KC_ENT \
  ),

  [_FN_EMACS] = LAYOUT( \
 
      KC_ESC,	XXXXXXX,	C(KC_C),	KC_END,		XXXXXXX,		XXXXXXX,	C(KC_V),	XXXXXXX,	XXXXXXX,	XXXXXXX,	KC_UP,		_______,	\

      _______,	CTL_T(KC_HOME),	C(KC_F),	KC_DEL,		LGUI_T(KC_RIGHT),	KC_ESC,		_______,	_______,	TD(TD_KILL),	_______,	RCTL_T(KC_APP),	_______,	\
 	
      _______,	XXXXXXX,	KCZ_LEAD,	_______,	KC_PGDN,		KC_LEFT,	KC_DOWN,	_______,	_______,	_______,	C(KC_Z),	_______,	\

      							  _______,	_______,	_______,	_______,	_______,	_______	\
      
  ),

  [_LH_FN] = LAYOUT( \
 
      _______,	KC_HOME,	KC_LEFT,	C(KC_A),	KC_END,		KC_RIGHT,	_______,	_______,	_______,	KC_PGUP,	_______,	_______,	\
 	
      _______,	CTL_T(C(KC_A)),	_______,	KC_DEL,		C(KC_F),	_______,	KC_HOME,	KC_LEFT,	KC_DOWN,	KC_UP,		KC_RGHT,	KC_END,		\
 	
      _______,	C(KC_Z),	_______,	_______,	_______,	_______,	_______,	_______,	KC_PGDN,	_______,	C(KC_Z),	_______,	\
 
      _______,	_______,	_______,	_______,	_______,	_______	\
  ),

  [_LH_NUMS] = LAYOUT( \

      _______,	KC_1,		KC_2,		KC_3,		KC_DOT,		KC_PPLS,	_______,	_______,	_______,	_______,	_______,	_______,	\

      _______,	KC_4,		KC_5,		KC_6,		KC_COMM,	KC_PMNS,	_______,	_______,	_______,	_______,	_______,	_______,	\

      _______,	KC_7,		KC_8,		KC_9,		KC_EQL,		KC_PAST,	_______,	_______,	_______,	_______,	_______,	_______,	\

                                                KC_0,		KC_PSLS,	_______,	_______,	_______,	_______	\
  ),

  [_RH_NUMS] = LAYOUT( \

      _______,	KC_DOT,		KC_1,		KC_2,		KC_3,		KC_PPLS,	_______,	_______,	_______,	_______,	_______,	_______,	\

      _______,	KC_COMM,	KC_4,		KC_5,		KC_6,		KC_PMNS,	_______,	_______,	_______,	_______,	_______,	_______,	\

      _______,	KC_EQL,		KC_7,		KC_8,		KC_9,		KC_PAST,	_______,	_______,	_______,	_______,	_______,	_______,	\

                                                KC_PSLS,	KC_0,		_______,	_______,	_______,	_______	\
  ),

  [_LH_MOVE] = LAYOUT( \
 
      _______,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	_______,	_______,	_______,	_______,	_______,	_______,	\
 
      KC_ENT,	KC_HOME,	KC_UP,	KC_END,	KC_PGUP,	XXXXXXX,	_______,	_______,	_______,	_______,	_______,	_______,	\
 	
      _______,	KC_LEFT,	KC_DOWN,	KC_RIGHT,	KC_PGDN,	XXXXXXX,	MO(_LH_NUMS),	MO(_LH_MOVE),	_______,	_______,	_______,	_______,	\
 
						   KC_LSFT,	_______,	_______,	_______,	_______,	_______	\
  ),

  [_RH_MOVE] = LAYOUT( \
 
      _______,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	_______,	_______,	_______,	_______,	_______,	_______,	\
 
      KC_ENT,	KC_HOME,	KC_UP,	KC_END,	KC_PGUP,	XXXXXXX,	_______,	_______,	_______,	_______,	_______,	_______,	\
 	
      _______,	KC_LEFT,	KC_DOWN,	KC_RIGHT,	KC_PGDN,	XXXXXXX,	MO(_LH_NUMS),	MO(_LH_MOVE),	_______,	_______,	_______,	_______,	\
 
											KC_LSFT,	_______,	_______,	_______,	_______,	_______	\
  ),

  [_PLOVER] = LAYOUT( \

      _______, _______,	_______, _______, _______, _______,        _______,	_______,	_______,	_______,	_______,	_______,	\

      _______, _______,	_______, _______, _______, _______,        _______,	_______,	_______,	_______,	_______,	_______,	\

      _______, _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______, _______,\

                                          _______, _______, _______,    _______, _______, _______ \
  ),

  [_TOP] = LAYOUT( \

      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,\

      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,\

      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,\

                                          _______, _______, _______,    _______, _______, _______ \
		   )
};

// Below is all the stuff left from the default map

int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

LEADER_EXTERNS();

void matrix_scan_user(void) {
   iota_gfx_task();

   LEADER_DICTIONARY() {
     leading = false;
     leader_end();

     SEQ_EXEC_MACRO(KC_E) {
       register_code(DYN_MACRO_PLAY1);
       unregister_code(DYN_MACRO_PLAY1);
     }
   }
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    // If you want to change the display of OLED, you need to change here
    matrix_write_ln(matrix, read_layer_state());
    matrix_write_ln(matrix, read_keylog());
    //matrix_write_ln(matrix, read_keylogs());
    //matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
    //matrix_write_ln(matrix, read_host_led_state());
    //matrix_write_ln(matrix, read_timelog());
  } else {
    matrix_write(matrix, read_logo());
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}
#endif//SSD1306OLED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  	if (record->event.pressed) {
#ifdef SSD1306OLED
    	set_keylog(keycode, record);
#endif
    // set_timelog();
  	}

  switch (keycode) {
    default:
    	return true;
  }
}
