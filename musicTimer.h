#ifndef MUSICTIMER_H
#define MUSICTIMER_H
#include "pico/stdlib.h"
#include "dfPlayerDriver.h"

class MusicTimer {
private:
    bool buttonLastIteration;
    bool buttonOn = false;
    bool activateMusic = false;
    bool musicOn = false;
    
    
public:
    MusicTimer(){//leeres Objekt der Klasse MusicTimer
        bool dummy = true;
    }
    MusicTimer(int button_, int folder_, int track_, bool repeat_, bool toggleable_) {
        button = button_;
        folder = folder_;
        track = track_; 
        repeat = repeat_; //song wiederholt sich (bis er abgebrochen wird)
        toggleable = toggleable_; //false: schalter gedrückt halten | true: einmal drücken --an, 2. mal drücken -->aus
        dummy = false;
        initGPIO();
    }

    bool dummy;
    int button;
    int folder;
    int track;
    bool repeat;
    bool toggleable;

    void initGPIO(){
        if (dummy) { //checkt ob das Objekt benutzt werden soll
            return;
        }
        //Initialize button
        gpio_init(button); //initiatilisieren
        gpio_set_dir(button, GPIO_IN); //setzten
        gpio_pull_up(button); //hochziehen
        buttonLastIteration = !gpio_get(button);//am Anfang false, wenn button noch nicht gedrückt wurde
    }

    void routine(){
        if (dummy) { //checkt ob das Objekt benutzt werden soll
            return;
        }
        //Bestimmung, ob Timer aktiv sein soll (Toggle oder Hold Modus)
        if(toggleable){
		    //Wenn Schalter an ist und in der vorherigen Iteration aus war, dann wird activateTimer getogglet
            buttonOn = !gpio_get(button);
		    if(buttonOn && !buttonLastIteration){
			    //toggles activateTimer
			    activateMusic = !activateMusic;
                //printf("activate Toggle \n");
                //printf("switch %d toggled\n", gpio_pin_switch);
		    }   
		//setzt switchOnLastIteration auf aktuellen Wert des Switches für die nächste Iteration
		buttonLastIteration = buttonOn;
	    }
	    else{
            //Wenn toggleable auf false ist, wird activateTimer true, solange button gedrückt ist
		    activateMusic = !gpio_get(button);
            //printf("aM = %d \n", activateMusic);
	    }

        if(activateMusic){//Anschalten der Musik
            musicAn();
            //printf("musicOn = %d, ", musicOn);
        }
        if(!activateMusic){//stoppen der MusiK
            musicAus();
            //printf("musicOn = %d ,", musicOn);
        }
    }

    void musicAn(){//falls musik noch nicht läuft, wird musik angeschaltet
        if(!musicOn){
            playMusic();
            musicOn =true;   
        }
    }

    void musicAus(){
        if(musicOn){
            stopMusic();
            musicOn = false;
            //printf("stop ");
        }
    }
    

    void ausgabeWerte(){
       //printf("aM= %d ", activateMusic);
       //printf("bO= %d \n", buttonOn);
    }

    void playMusic(){
        DfPlayerPico dfp;

        dfp.specifyVolume(15); //0 bis 30, setzt Lautstärke auf angegebenen Wert
        sleep_ms(100); //100ms zum befehl verarbeiten

        dfp.sendCmd(dfPlayer::cmd::SPECIFY_FOLDER_PLAYBACK,folder); //wählt ordner "01" in SD-Karte aus
        sleep_ms(100);
        dfp.sendCmd(dfPlayer::cmd::SPECIFY_TRACKING,track); //wählt song: "002.mp3" aus ordner aus
        //printf("play ");
        sleep_ms(100);
        if(repeat){
            dfp.sendCmd(dfPlayer::cmd::SPECIFY_PLAYBACK_MODE,track);
            sleep_ms(100);
        }
        //dfp.sendCmd(dfPlayer::cmd::PLAYBACK,00);
        //sleep_ms(100);
    }

    void stopMusic(){
        DfPlayerPico dfp;
        //dfp.sendCmd(dfPlayer::cmd::STOP,00); //funktioniert nicht
        dfp.specifyVolume(0); //0 bis 30, setzt die Lautstärke auf 0 = Stoppen der Musik
        //printf("Stop \n");
        sleep_ms(100);
    }
};

#endif /* MUSICTIMER_H */