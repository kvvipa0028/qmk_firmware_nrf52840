#include "ninjonas.h"

__attribute__((weak))
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) { return true; }

__attribute__((weak))
bool process_record_secrets(uint16_t keycode, keyrecord_t *record) { return true; }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    #ifdef SSD1306OLED
    set_keylog(keycode, record);
    #endif
  }

  switch (keycode) {

    // Sends pyenv to activate 'jira' environment
    case M_PYNV:
      if (record->event.pressed) {
        SEND_STRING("pyenv activate jira" SS_TAP(X_ENTER));
      }
      break;

    // Sends  + alt + shift to a keycode to activate shiftit. See: https://github.com/fikovnik/ShiftIt
    case M_SHFT:
      if (record->event.pressed) {
        register_code(KC_LGUI);
        register_code(KC_LALT);
        register_code(KC_LCTL);
      } else {
        unregister_code(KC_LGUI);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
      }
      break;

    // Sends QMK make command to compile keyboard
    case M_MAKE:
     if (record->event.pressed) {
        SEND_STRING("rm -f *.hex && rm -rf .build/ && make " QMK_KEYBOARD ":" QMK_KEYMAP SS_TAP(X_ENTER));
      }
      break;

    // Sends QMK make command with the correct bootloader
    case M_FLSH:
      if (!record->event.pressed) {
        SEND_STRING("rm -f *.hex && rm -rf .build/ && make " QMK_KEYBOARD ":" QMK_KEYMAP
          #if  (defined(BOOTLOADER_DFU) || defined(BOOTLOADER_LUFA_DFU) || defined(BOOTLOADER_QMK_DFU))
            ":dfu "
          #elif defined(BOOTLOADER_CATERINA)
            ":avrdude "
          #endif
          SS_TAP(X_ENTER)
        );

        // Send reset_keyboard command instead of pressing reset button
        #if  (defined(BOOTLOADER_DFU) || defined(BOOTLOADER_LUFA_DFU) || defined(BOOTLOADER_QMK_DFU))
        reset_keyboard();
        #endif
      }
      break;

    // Sends QMK version
    case M_VRSN:
      if (record->event.pressed) {
        SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION ", Built on: " QMK_BUILDDATE);
      }
      break;

    // Opens Visual Studio Code on current directory
    case M_CODE:
      if (record->event.pressed) {
        SEND_STRING("code ." SS_TAP(X_ENTER));
      }
      break;

    // BEGIN: Layer macros
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      break;
    case DVORAK:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_DVORAK);
      }
      break;
    case COLEMAK:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_COLEMAK);
      }
      break;
    // END: Layer macros
  }

  return process_record_keymap(keycode, record) && process_record_secrets(keycode, record);
}
