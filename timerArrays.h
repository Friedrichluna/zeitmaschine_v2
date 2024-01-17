#ifndef TIMERARRAYS_H
#define TIMERARRAYS_H
#include "musicTimer.h"
#include "relayTimer.h"

#define RELAY_TIMER_AMOUNT 16
#define MUSIC_TIMER_AMOUNT 16


// Klasse TimerArrays wird zum Speichern der Timer während der Runtime verwendet
// damit die erstellten Timer aus cgi.h in die Main-Funktion gelangen
// Realisierung als Singleton von https://stackoverflow.com/questions/1008019/how-do-you-implement-the-singleton-design-pattern
class TimerArrays{
    private:
        TimerArrays(){}
        //Arrays zum Speichern der Timer
        RelayTimer relayTimer_arr[RELAY_TIMER_AMOUNT]; 
        MusicTimer musicTimer_arr[MUSIC_TIMER_AMOUNT];
        //bool für Startbedinung der Routine (durch Start-Button auf der Website in cgi.h aktiviert)
        bool timers_got_updated = false;
    public:
        // get Instance Funktion returned einzige Instanz
        static TimerArrays& getInstance(){
                static TimerArrays instance;
                return instance;
            }

        //nicht genutzte Methoden, die gelöscht werden, um sicherzugehen, das keine Kopien von Singleton auftauchen
        TimerArrays(TimerArrays const&) = delete;
        void operator=(TimerArrays const&) = delete;

        //Funktionen zum Ändern und Erhalten der Relay- und Musictimer
        RelayTimer getRelayTimer(int index){
            return relayTimer_arr[index];
        }
        void replaceRelayTimer(int index, RelayTimer relayTimer){
            relayTimer_arr[index] = relayTimer;
        }
        void setRelayTimer(RelayTimer* relayTimer_arr_){
            for (int i = 0; i < RELAY_TIMER_AMOUNT; i++) {
                relayTimer_arr[i] = relayTimer_arr_[i];
                //printRelayTimer(relayTimer_arr_[i], i);
            }
        }
        
        MusicTimer getMusicTimer(int index){
            return musicTimer_arr[index];
        }
        void replacemMusicTimer(int index, MusicTimer musicTimer){
            musicTimer_arr[index] = musicTimer;
        }
        void setMusicTimer(MusicTimer* musicTimer_arr_){
            for (int i = 0; i < MUSIC_TIMER_AMOUNT; i++) {
                musicTimer_arr[i] = musicTimer_arr_[i];
            }
        }

        // Setter und Getter für timers_got_updated
        void set_timers_got_updated(){
        timers_got_updated = true;
        }
        bool get_timers_got_updated(){
            return timers_got_updated;
        }

        // Funktion zum Debuggen
        void printRelayTimer(RelayTimer relyTimer, int index){
            if(relyTimer.dummy){
                printf("relytimer %d is a dummy\n", index);
            }
            else {
                printf("relytimer %d is not a dummy\n", index);
            }
        }
};

//Funktion, die die Schalter auf die verwendeten GPIO-Pins mappt
int map_button(int index, bool invert) {
    int button_arr[7] = {0,1,2,3,4,5,6};
    if(invert){
        for(int i = 0; i < sizeof(button_arr); i++){
            if(button_arr[i] == index){
                return i+1; //returned Schalternummer
            }
        }
        return -1; //Fehler, falls index nicht im button_arr enthalten
    }
    else{
        return button_arr[index-1]; //returned Pin
    }
}

// Funktion, die die Relais auf die verwendeten GPIO-Pins mappt
int map_relay(int index, bool invert) {
    int relay_arr[16] = {7,10,11,12,13,14,15,16,17,18,19,20,21,22,26,27};
    if(invert){
        for(int i = 0; i < sizeof(relay_arr); i++){
            if(relay_arr[i] == index){
                return i+1; //returned Relaynummer
            }
        }
        return -1; //Fehler, falls index nicht im relay_arr enthalten
    }
    else{
        return relay_arr[index-1]; //returned Pin
    }
}

#endif /* TIMERARRAYS_H */