#pragma once

// eventually you'll want to enum the layers here 

#define BS_CTL MT(MOD_LCTL, KC_BSPC)
#define SP_CTL MT(MOD_RCTL, KC_SPC)


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
  // tap: KC_ENT; hold: rshift; dtap: TG layer; dhold: MO layer 
  RS_MO_TG,
  LS_MO_TG
};

uint8_t cur_dance(qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) return S_TAP;
    else return S_HOLD;
  } else if (state->count == 2) {
    if (state->interrupted) return DS_TAP;
    else if (state->pressed) return D_HOLD;
    else return D_TAP;
  } else return 8;
}

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
      layer_invert(2);
      break;
    case D_HOLD:
      layer_off(2);
      break;
    case DS_TAP:
      tap_code(KC_ENT);
      unregister_code(KC_ENT);
      break;
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [RS_MO_TG] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rs_finished, rs_reset)
};
