#include "vma209.h"

uint8_t initVma209() {

  initUSART();

  if(initADC() == ADC_LIB_ERROR) {
      return VMA209_LIB_ERROR;
  }

  if(initButtons() == BUTTON_LIB_ERROR) {
      return VMA209_LIB_ERROR;
  }

  if(initBuzzer() == BUZZER_LIB_ERROR) {
      return VMA209_LIB_ERROR;
  }

  if(initDisplay() == DISPLAY_LIB_ERROR) {
      return VMA209_LIB_ERROR;
  }

  if(initLed() == LED_LIB_ERROR) {
      return VMA209_LIB_ERROR;
  }

  return VMA209_LIB_SUCCES;
}
