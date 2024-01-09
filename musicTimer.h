#ifndef MUSICTIMER_H
#define MUSICTIMER_H
#include "pico/stdlib.h"
#include "dfPlayerDriver.h"

class MusicTimer
{
private:
    bool dummy;
    int button;
    int folder;
    int track;
    bool repeat;
    bool toggleable;
public:
    MusicTimer(){
        bool dummy = true;
    }
    MusicTimer(int button_, int folder_, int track_, bool repeat_, bool toggleable_) {
        button = button_;
        folder = folder_;
        track = track_; 
        repeat = repeat_; //song wiederholt sich (bis er abgebrochen wird)
        toggleable = toggleable_; //false: schalter gedrückt halten | true: einmal drücken --an, 2. mal drücken -->aus
    }

    void routine() {
        if (dummy) {
            return;
        }
        //wenn der Schalter gedrückt wurde
    }

    void doSomethingMusic() {
        //DfPlayerPico
        DfPlayerPico dfp;

        dfp.reset(); 
        sleep_ms(1500); //braucht 1,5sek um SD auszulesen

        dfp.specifyVolume(18); //0 bis 30
        sleep_ms(100); //100ms zum befehl verarbeiten


        dfp.sendCmd(dfPlayer::cmd::SPECIFY_FOLDER_PLAYBACK,5); //wählt ordner "00" in SD-Karte aus
        sleep_ms(100); 
        dfp.sendCmd(dfPlayer::cmd::SPECIFY_TRACKING,5); //wählt song: "000.mp3" aus ordner aus
        sleep_ms(100); 
        dfp.sendCmd(dfPlayer::cmd::SPECIFY_PLAYBACK_MODE,0); //(0/1/2/3) Repeat/folder repeat/single repeat/random 
        
    }
};

#endif /* MUSICTIMER_H */