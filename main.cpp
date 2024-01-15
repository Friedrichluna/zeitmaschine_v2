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


void printRelayTimer(RelayTimer relyTimer, int index);


int main() {
    stdio_init_all(); //initialisiert Pico
    sleep_ms(5000);
    

    RelayTimer relayTimer_arr[RELAY_TIMER_AMOUNT];
    MusicTimer musicTimer_arr[MUSIC_TIMER_AMOUNT];

    TimerArrays& ta = TimerArrays::getInstance();
    DataSaver ds;
    WebInterface wi = WebInterface(); //erstellt webinterface Objekt

    ds.print_buf_int(32*32);
    for (int i = 0; i < RELAY_TIMER_AMOUNT; i++){
        relayTimer_arr[i] = ds.recover_relayTimer(i); //zieht Objekt wieder aus flash memory
        
    }
    ta.setRelayTimer(relayTimer_arr);
    for (int i = 0; i < MUSIC_TIMER_AMOUNT; i++){
        musicTimer_arr[i] = ds.recover_musicTimer(i, RELAY_TIMER_AMOUNT); //zieht Objekt wieder aus flash memory
        printf("Recoverd Timer: %s\n", musicTimer_arr[i].toString());
    }
    ta.setMusicTimer(musicTimer_arr);
    
    //initialisiert Webinterface
    bool connected = wi.init_webInterface();
    if(connected){
        while (!ta.get_timers_got_updated()) {
            sleep_ms(100);
        }
    }
    printf("Save and start\n");

    for (int i = 0; i < RELAY_TIMER_AMOUNT; i++){
        relayTimer_arr[i] = ta.getRelayTimer(i); //zieht Objekt wieder aus flash memory
        printf("Relay Timer: %s\n", relayTimer_arr[i].toString());
    }
    for (int i = 0; i < MUSIC_TIMER_AMOUNT; i++){
        musicTimer_arr[i] = ta.getMusicTimer(i); //zieht Objekt wieder aus flash memory
        printf("Music Timer: %s\n", musicTimer_arr[i].toString());
    }

    if(ta.get_timers_got_updated()){
        //ds.erase_target_flash();
        ds.flash_objects(relayTimer_arr, RELAY_TIMER_AMOUNT, musicTimer_arr, MUSIC_TIMER_AMOUNT);
    }

    printf("Starting Routine!");
    while(true) {
        sleep_ms(10);
        for (int b = 0; b < RELAY_TIMER_AMOUNT; b++) {
            relayTimer_arr[b].routine();
        }
        for (int b = 0; b < MUSIC_TIMER_AMOUNT; b++) {
            musicTimer_arr[b].routine();
        }
    };
}




