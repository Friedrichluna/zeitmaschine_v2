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


int main() {

    stdio_init_all(); //initialisiert Pico

    WebInterface wi = WebInterface(); //erstellt webinterface Objekt
    wi.init_webInterface(); //initialisiert Webinterface

    while (wi.waiting_for_Input()) {
        sleep_ms(100);
    }
    
    


    
    RelayTimer relayTimer_arr[RELAY_TIMER_AMOUNT]; //array mit leeren relayTimern
    int relay_arr1[1] = {2};
    int relay_arr2[1] = {3};
    RelayTimer newRelayTimer1(16, 2, 0, relay_arr1, 1, false);
    RelayTimer newRelayTimer2(15, 3, 0, relay_arr2, 1, true);
    relayTimer_arr[0] = newRelayTimer1;
    relayTimer_arr[1] = newRelayTimer2;
    MusicTimer musicTimer_arr[MUSIC_TIMER_AMOUNT];
    MusicTimer newMusicTimer1(17, 1, 1, true, false);
    musicTimer_arr[0] = newMusicTimer1;

    DataSaver ds;
    ds.flash_objects(relayTimer_arr, RELAY_TIMER_AMOUNT); //speichert alle Obejkte
    //relayTimer_arr[2] = ds.recover_relayTimer(1); //zieht Objekt wieder aus flash memory

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
    return button_arr[index];
}

int map_relay(int index) {
    int relay_arr[16] = {7,10,11,12,13,14,15,16,17,28,19,20,21,22,26,27};
    return relay_arr[index];
}