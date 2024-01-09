#include "pico/stdlib.h"
#include "relayTimer.h"
#include "webInterface.h"
#include "musicTimer.h"
#include "dataSaver.h"
#include <map>
#include <string>

#define RELAY_TIMER_AMOUNT 16
#define MUSIC_TIMER_AMOUNT 16


int map_button(int index);
int map_relay(int index);

extern RelayTimer relayTimer_arr[RELAY_TIMER_AMOUNT]; //array mit leeren relayTimern
extern MusicTimer musicTimer_arr[MUSIC_TIMER_AMOUNT];

int main() {
    stdio_init_all(); //initialisiert Pico

    

    RelayTimer relayTimer_arr[RELAY_TIMER_AMOUNT];
    MusicTimer musicTimer_arr[MUSIC_TIMER_AMOUNT];

    DataSaver ds;
    ds.flash_objects(relayTimer_arr, RELAY_TIMER_AMOUNT); //speichert alle Obejkte
    
    

    WebInterface wi = WebInterface(); //erstellt webinterface Objekt
    wi.init_webInterface(); //initialisiert Webinterface

    while (wi.waiting_for_Input()) {
        sleep_ms(100);
    }
    
    for (int i = 0; i < RELAY_TIMER_AMOUNT; i++){
        relayTimer_arr[i] = ds.recover_relayTimer(1); //zieht Objekt wieder aus flash memory
    }
    
    

    
    
    int relay_arr1[3] = {map_relay(2), map_relay(1), map_relay(3)};
    int relay_arr2[1] = {map_relay(4)};
    RelayTimer newRelayTimer1(map_button(1), 2, 0, relay_arr1, 3, false);
    RelayTimer newRelayTimer2(map_button(2), 3, 0, relay_arr2, 1, true);
    relayTimer_arr[0] = newRelayTimer1;
    relayTimer_arr[1] = newRelayTimer2;

    
    MusicTimer newMusicTimer1(17, 1, 1, true, false);
    musicTimer_arr[0] = newMusicTimer1;
    newMusicTimer1.doSomethingMusic();

    

    while(true) {
        sleep_ms(10);
        for (int b = 0; b < RELAY_TIMER_AMOUNT; b++) {
            relayTimer_arr[b].routine();
        }
        for (int b = 0; b < MUSIC_TIMER_AMOUNT; b++) {
            musicTimer_arr[b].routine();
        }
        //printf("\n");
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