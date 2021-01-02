#pragma once

// eventually you'll want to enum the layers here 

#define BS_CTL MT(MOD_LCTL, KC_BSPC)
#define SP_NAV LT(1, KC_SPC)


// tap dance stuff

// from https://docs.qmk.fm/#/feature_tap_dance
typedef struct {
  bool is_press_action;
  uint8_t state;
} tap;

enum {
  S_TAP = 1,
  S_HOLD,
  D_TAP,
  DS_TAP,
  D_HOLD
};

enum {
  // tap: KC_ENT;     hold: rshift; dtap: TG layer; dhold: MO layer 
  RS_TG2,
  // tap: open paren; hold: lshift; dtap: TG layer; dhold: MO layer 
  LS_TG1
};

uint8_t cur_dance(qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    //if (state->interrupted || !state->pressed) return S_TAP;
    if (!state->pressed) return S_TAP;
    else return S_HOLD;
  } else if (state->count == 2) {
    if (state->interrupted) return DS_TAP;
    else if (state->pressed) return D_HOLD;
    else return D_TAP;
  } else return 8;
}

/* DANCE FOR RIGHT SHIFT */
static tap rs_tap_state = {
  .is_press_action = true,
  .state = 0
};

void rs_finished(qk_tap_dance_state_t *state, void *user_data) {
  rs_tap_state.state = cur_dance(state);
  switch (rs_tap_state.state) {
    case S_TAP:
      register_code(KC_ENT);
      break;
    case S_HOLD:
      register_code(KC_RSFT);
      break;
    case D_TAP:
      layer_invert(2);
      break;
    case D_HOLD:
      layer_on(2);
      break;
    case DS_TAP:
      tap_code(KC_ENT);
      register_code(KC_ENT);
      break;
  }
}

void rs_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (rs_tap_state.state) {
    case S_TAP:
      unregister_code(KC_ENT);
      break;
    case S_HOLD:
      unregister_code(KC_RSFT);
      break;
    case D_TAP:
      break;
    case D_HOLD:
      layer_off(2);
      break;
    case DS_TAP:
      unregister_code(KC_ENT);
      break;
  }
}


/* DANCE FOR LEFT SHIFT */
static tap ls_tap_state = {
  .is_press_action = true,
  .state = 0
};

void ls_finished(qk_tap_dance_state_t *state, void *user_data) {
  ls_tap_state.state = cur_dance(state);
  switch (ls_tap_state.state) {
    case S_TAP:
      register_code(KC_LSFT);
      register_code(KC_9);
      break;
    case S_HOLD:
      register_code(KC_LSFT);
      break;
    case D_TAP:
      layer_invert(1);
      break;
    case D_HOLD:
      layer_on(1);
      break;
    case DS_TAP:
      register_code(KC_LSFT);
      tap_code(KC_9);
      unregister_code(KC_LSFT);
      register_code(KC_LSFT);
      register_code(KC_9);
      break;
  }
}

void ls_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (ls_tap_state.state) {
    case S_TAP:
      unregister_code(KC_LSFT);
      unregister_code(KC_9);
      break;
    case S_HOLD:
      unregister_code(KC_LSFT);
      break;
    case D_TAP:
      break;
    case D_HOLD:
      layer_off(1);
      break;
    case DS_TAP:
      unregister_code(KC_LSFT);
      unregister_code(KC_9);
      break;
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [RS_TG2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rs_finished, rs_reset),
  [LS_TG1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ls_finished, ls_reset)
};
