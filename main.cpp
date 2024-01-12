#include "pico/stdlib.h"
#include "relayTimer.h"
#include "webInterface.h"
#include "musicTimer.h"
#include "dataSaver.h"
#include "timerArrays.h"
#include <map>
#include <string>

#define RELAY_TIMER_AMOUNT 16
#define MUSIC_TIMER_AMOUNT 16


int map_button(int index);
int map_relay(int index);
void printRelayTimer(RelayTimer relyTimer, int index);


int main() {
    stdio_init_all(); //initialisiert Pico
    sleep_ms(5000);
    

    RelayTimer relayTimer_arr[RELAY_TIMER_AMOUNT];
    MusicTimer musicTimer_arr[MUSIC_TIMER_AMOUNT];

    TimerArrays& ta = TimerArrays::getInstance();
    DataSaver ds;
    WebInterface wi = WebInterface(); //erstellt webinterface Objekt
    wi.init_webInterface(); //initialisiert Webinterface
    
    ds.print_buf_int(64*32);
    for (int i = 0; i < RELAY_TIMER_AMOUNT; i++){
        relayTimer_arr[i] = ds.recover_relayTimer(i); //zieht Objekt wieder aus flash memory
        
    }
    ta.setRelayTimer(relayTimer_arr);
    for (int i = 0; i < MUSIC_TIMER_AMOUNT; i++){
        musicTimer_arr[i] = ds.recover_musicTimer(i, RELAY_TIMER_AMOUNT); //zieht Objekt wieder aus flash memory
    }
    ta.setMusicTimer(musicTimer_arr);
    

    while (!ta.get_timers_got_updated()) {
        sleep_ms(100);
    }
    printf("Save and start\n");

    for (int i = 0; i < RELAY_TIMER_AMOUNT; i++){
        relayTimer_arr[i] = ta.getRelayTimer(i); //zieht Objekt wieder aus flash memory
    }
    for (int i = 0; i < MUSIC_TIMER_AMOUNT; i++){
        musicTimer_arr[i] = ta.getMusicTimer(i); //zieht Objekt wieder aus flash memory
    }

    if(ta.get_timers_got_updated()){
        //ds.erase_target_flash();
        ds.flash_objects(relayTimer_arr, RELAY_TIMER_AMOUNT, musicTimer_arr, MUSIC_TIMER_AMOUNT);
    }


    while(true) {
        sleep_ms(10);
        for (int b = 0; b < RELAY_TIMER_AMOUNT; b++) {
            relayTimer_arr[b].routine();
        }
        for (int b = 0; b < MUSIC_TIMER_AMOUNT; b++) {
            musicTimer_arr[b].routine();
        }
        printf("\n");
    };
}


int map_button(int index) {
    int button_arr[7] = {0,1,2,3,4,5,6};
    return button_arr[index-1];
}

int map_relay(int index) {
    int relay_arr[16] = {7,10,11,12,13,14,15,16,17,28,19,20,21,22,26,27};
    return relay_arr[index-1];
}

void printRelayTimer(RelayTimer relyTimer, int index){
    if(relyTimer.dummy){
        printf("relytimer %d is a dummy\n", index);
    }
    else {
        printf("relytimer %d is not a dummy\n", index);
    }
}