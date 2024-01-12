#ifndef MUSICTIMER_H
#define MUSICTIMER_H
#include "pico/stdlib.h"
#include "dfPlayerDriver.h"

class MusicTimer
{
private:
    bool buttonLastIteration;
    bool buttonOn = false;
    bool activateMusic = false;
    bool musicOn = false;
    int counter = 0;
    bool folderSelect = false;
    bool trackSelect = false;
    bool volumeSelect = false;
    bool repeatCheck = false;
     
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

    //Routine-Programm, wird von der main aufgerufen 
    //erkennt, ob button gedrückt wird und schaltet entsprechend Musik an und aus
    //dabei Unterscheidung zwischen Toogle- und Hold-Modus
    void routine(){
        if (dummy) { //checkt ob das Objekt benutzt werden soll
            return;
        }
        //printf("I exist\n");
        //Bestimmung, ob Timer aktiv sein soll (Toggle- oder Hold-Modus)
        if(toggleable){//Toggle-Modus
		    //Wenn Schalter an ist und in der vorherigen Iteration aus war, dann wird activateTimer getogglet
            buttonOn = !gpio_get(button);
		    if(buttonOn && !buttonLastIteration){
                //toggles activateTimer
			    activateMusic = !activateMusic;
                //printf("activate Toggle \n");
		    }   
		buttonLastIteration = buttonOn;
	    }
	    else{//Hold-Modus
            //Wenn toggleable auf false ist, wird activateTimer true, solange button gedrückt ist
		    activateMusic = !gpio_get(button);
            //printf("aM = %d \n", activateMusic);
	    }

        //printf("ac=%d ", activateMusic);
        if(activateMusic){//Anschalten der Musik
            counter =counter+1;
            //printf("%d ", counter);
            musicAn();//Funktion zum Anschalten der Musik
            //printf("musicOn = %d, \n", musicOn);
        }
        if(!activateMusic){//Stoppen der MusiK
            musicAus();
            //printf("musicOn = %d ,", musicOn);
        }
    }

    //Funktion zum Anschalten der Musik: falls Musik noch nicht läuft, wird Musik angeschaltet
    void musicAn(){
        if(!musicOn){
            //printf("musik an \n");
            playMusic();//Funktion zum Abspielen der Musik 
        }
    }

    //Funktion zum Ausschalten der Musik:falls Musik schon läuft, wird Musik ausgeschaltet
    void musicAus(){
        if(musicOn){
            stopMusic();//Funktion zum Unterbrechen der Musik
            musicOn = false;
        }
    }

    //Funktion zum Abspielen der Musik:
    //Zwischen allen Befehlen (comands) an den dfPlayer müssen 100ms gewartet werden. 
    //Um durch die Wartezeit nicht das main-Programm zu unterbrechen, wird die Ausführung der main (alle 10ms) 
    //mitgezählt. Nur jede 10. main-Ausführung wird der nächste Befehl an den dfPlayer weitergegeben
    void playMusic(){
        DfPlayerPico dfp;
        //printf("play ");
        if((counter-1)%10==0){//nur jede 10. Routine-Iteration Befehl ausführen
            if(!volumeSelect){//wenn Lautstärke noch nicht gewählt ist, Lautstärke wählen. Sonst weitere Einstellungen abarbeiten
                dfp.specifyVolume(15); //0 bis 30, setzt Lautstärke auf angegebenen Wert
                volumeSelect = true;
                //printf("volumeSelect \n");
            }
            else{
                if(!folderSelect){//wenn Ordner noch nicht gewählt ist, Ordner wählen. Sonst weitere Einstellungen abarbeiten
                    dfp.sendCmd(dfPlayer::cmd::SPECIFY_FOLDER_PLAYBACK,folder); //wählt ordner "01" in SD-Karte aus  
                    folderSelect = true;
                    //printf("folderSelect \n");
                }
                else{
                    if(!trackSelect){//wenn Track noch nicht gewählt ist, Track wählen.
                        dfp.sendCmd(dfPlayer::cmd::SPECIFY_TRACKING,track); //wählt song: "002.mp3" aus ordner aus
                        trackSelect=true;
                        //printf("trackselect \n");
                    }
                    else{
                        if(!repeatCheck){//wenn Modus (repeat/!repeat) noch nicht gewählt ist, Ordner wählen. Sonst weitere Einstellungen abarbeiten
                            if(repeat){//Falls repeat=true wird Soundtrack wiederholt, anderfalls nur einmal abgespielt
                                dfp.sendCmd(dfPlayer::cmd::SPECIFY_PLAYBACK_MODE,track);//wiederholt das Abspielen eines Soundtracks nach dessen Beendigung
                                repeatCheck=true;
                            }
                            else{
                                repeatCheck=true;
                            }
                        }
                        else{//Wenn auch schon Modus gewählt wurde, können alle bool zurückgesetzt werden und Musik läuft
                        volumeSelect=false;
                        folderSelect=false;
                        trackSelect=false;
                        repeatCheck=false;
                        musicOn =true;
                        counter = 0;
                        }
                    } 
                }
            }     
        }
    }

    //Funktion zum Unterbrechen der Musik
    void stopMusic(){
        DfPlayerPico dfp;
        //dfp.sendCmd(dfPlayer::cmd::STOP,00); //funktioniert nicht
        dfp.specifyVolume(0); //0 bis 30, setzt die Lautstärke auf 0 = Stoppen der Musik
        //printf("Stop \n");
    } 

    char* toString(){
        char* result = (char*)malloc(120); 
        if(!dummy){
            sniprintf(result, 120, "Schalter: %d  Ordner: %d   Track: %d  Wiederholen: %d   Schalterbetrieb: %d", button, folder, track, repeat, toggleable);
        }
        else{
            sniprintf(result, 120, "Timer nicht belegt!");
        }
        return result;
    }

};

#endif /* MUSICTIMER_H */