#include "pico/stdlib.h"

#include "relayTimer.h"
#include "webInterface.h"
#include "musicTimer.h"
#include "dataSaver.h"

#define RELAY_TIMER_AMOUNT 16



int main() {

    stdio_init_all(); //initialisiert Pico

    WebInterface wi = WebInterface(); //erstellt webinterface Objekt
    wi.init_webInterface(); //initialisiert Webinterface
    
    doSomethingMusic();
    
    DataSaver ds;

    int relayTimer_Amount = RELAY_TIMER_AMOUNT;
    RelayTimer relayTimer_arr[relayTimer_Amount]; //array mit leeren relayTimern

    int relay_arr1[1] = {2};
    int relay_arr2[1] = {3};
    RelayTimer newRelayTimer1(16, 2, 0, relay_arr1, 1, false);
    RelayTimer newRelayTimer2(15, 3, 0, relay_arr2, 1, true);
    relayTimer_arr[0] = newRelayTimer1;
    relayTimer_arr[1] = newRelayTimer2;
    
    ds.flash_objects(relayTimer_arr, relayTimer_Amount); //speichert alle Obejkte
    //relayTimer_arr[2] = ds.recover_relayTimer(1); //zieht Objekt wieder aus flash memory

    while(true) {
        sleep_ms(10);
        for (int b = 0; b < relayTimer_Amount; b++) {
            relayTimer_arr[b].routine();
        }
        printf("\n");
    };
}


