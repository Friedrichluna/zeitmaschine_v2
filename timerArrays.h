#ifndef TIMERARRAYS_H
#define TIMERARRAYS_H
#include "musicTimer.h"
#include "relayTimer.h"

#define RELAY_TIMER_AMOUNT 16
#define MUSIC_TIMER_AMOUNT 16


class TimerArrays{
    private:
        TimerArrays(){}
        RelayTimer relayTimer_arr[RELAY_TIMER_AMOUNT]; //array mit leeren relayTimern
        MusicTimer musicTimer_arr[MUSIC_TIMER_AMOUNT];

        bool timers_got_updated = false;
    public:
        static TimerArrays& getInstance(){
                static TimerArrays instance;
                return instance;
            }
        TimerArrays(TimerArrays const&) = delete;
        void operator=(TimerArrays const&) = delete;

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

        void set_timers_got_updated(){
        timers_got_updated = true;
        }
        bool get_timers_got_updated(){
            return timers_got_updated;
        }

        void printRelayTimer(RelayTimer relyTimer, int index){
            if(relyTimer.dummy){
                printf("relytimer %d is a dummy\n", index);
            }
            else {
                printf("relytimer %d is not a dummy\n", index);
            }
        }
};


#endif /* TIMERARRAYS_H */