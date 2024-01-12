#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"
#include "timerArrays.h"


#define RELAY_TIMER_AMOUNT 16
#define MUSIC_TIMER_AMOUNT 16

// SSI tags - tag length limited to 8 bytes by default
const char * ssi_tags[] = {"rt1","rt2","rt3","rt4","rt5","rt6","rt7","rt8","rt9","rt10","rt11","rt12","rt13","rt14","rt15","rt16",
"mt1","mt2","mt3","mt4","mt5","mt6","mt7","mt8","mt9","mt10","mt11","mt12","mt13","mt14","mt15","mt16"};

u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
  size_t printed;
  TimerArrays& ta = TimerArrays::getInstance();

  //printf("SSI Index: %d\n", iIndex);
  if(0 <= iIndex && iIndex < RELAY_TIMER_AMOUNT){
    //printf("Trying to SSI print RelayTimer, %d\n", iIndex);
    printed = snprintf(pcInsert, iInsertLen, ta.getRelayTimer(iIndex).toString());
  }
  else if(RELAY_TIMER_AMOUNT <= iIndex && iIndex < MUSIC_TIMER_AMOUNT+RELAY_TIMER_AMOUNT){
    //printf("Trying to SSI print MusicTimer, %d\n", iIndex);
    printed = snprintf(pcInsert, iInsertLen, ta.getMusicTimer(iIndex-RELAY_TIMER_AMOUNT).toString());
  }
  else{
    printed = 0;
  }
  return (u16_t)printed;
}

// Initialise the SSI handler
void ssi_init() {
  // Initialise ADC (internal pin)
  adc_init();
  adc_set_temp_sensor_enabled(true);
  adc_select_input(4);

  http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}