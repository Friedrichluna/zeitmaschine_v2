#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "musicTimer.h"
#include "timerArrays.h"

#define RELAY_TIMER_AMOUNT 16
#define MUSIC_TIMER_AMOUNT 16
#define RELAY_ARR_OFFSET 5


// CGI handler wird gestartet, wenn eine Anfrage für /values.cgi registriert wird
// Verarbeitung der auf der Webseite für Relay-Timer eingegebenen Werte, abspeichern und aktualisieren der Webseite
const char * cgi_value_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    printf("index: %s\n", pcValue[0]); //Index
    int index = atoi(pcValue[0]) - 1;

    printf("button: %s\n", pcValue[1]); //Schalter
    int button = map_button(atoi(pcValue[1]),0);

    printf("frequency: %s\n", pcValue[2]); //Frequenz
    int frequency = atoi(pcValue[2]);
    
    printf("length: %s\n", pcValue[3]); //Dauer
    int length = atoi(pcValue[3]);

    printf("toggleable: %s\n", pcValue[4]); //Art der Ansteuerung
    bool toggleable;
    if (strcmp(pcValue[4], "1") == 0)
        toggleable = true;
    else
        toggleable = false;
    
    //bestimmen der Anzahl ausgewählter Relais
    int numberRelaysSelected = 0;
    for(int i = 0; i < RELAY_TIMER_AMOUNT+2; i++){
        if (strcmp(pcValue[RELAY_ARR_OFFSET + i], "end_of_array") == 0) {
            numberRelaysSelected = i;
            printf("numberRelaysSelected: %d\n", i);
            break;
        }
    }

    //Abspeichern der Nummern der ausgewählten Relais nach Umwandlung zu tatsächlicher Pin-Belegung am PICO
    int selectedRelays[numberRelaysSelected] = {};
    for(int j = 0; j < numberRelaysSelected; j++){
        selectedRelays[j] = map_relay(atoi(pcParam[RELAY_ARR_OFFSET + j]), 0);
    }
    printf("numberrelysselected: %d\n",numberRelaysSelected);
    printf("selected Relays: ");
    for(int k = 0; k < numberRelaysSelected; k++){
        printf("%d ", selectedRelays[k]);
    }
    printf("\n");
    
    //Singleton von TimerArrays wird erstellt bzw. importiert
    TimerArrays& ta = TimerArrays::getInstance();
    
    //Abspeichern des vollständigen Relay-Timer-Objektes mit allen Attributen
    if(numberRelaysSelected == 0){
        RelayTimer relayTimer; //erzeugen eines Dummies, wenn keine Relais ausgewählt sind -> Überschreiben eines bestehenden Timers möglich
        ta.replaceRelayTimer(index, relayTimer);
    }
    else{
        RelayTimer relayTimer(button,frequency, length, selectedRelays, numberRelaysSelected, toggleable);
        ta.replaceRelayTimer(index, relayTimer);
    }

    printf("success");
    return "/index.shtml"; //Aktualisierung der Webseite
}

//CGI handler der auf die Anfrage /music.cgi reagiert
//Verarbeitet die auf der Webseite getätigten Eingaben für Music-Timer, speichert das Objekt und aktualisiert die Webseite
const char * cgi_music_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    printf("index: %s\n", pcValue[0]); //Index
    int indexMusic = atoi(pcValue[0]) - 1;
    
    printf("button for music: %s\n", pcValue[1]); //Schalter
    int buttonMusic = map_button(atoi(pcValue[1]),0);

    printf("folder: %s\n", pcValue[2]); //Ordner, in dem sich der Track befindet
    int folder = atoi(pcValue[2]);

    printf("track: %s\n", pcValue[3]); //Track
    int track = atoi(pcValue[3]);

    bool repeat; //Wiedergabemodus
    if (strcmp(pcValue[4], "1") == 0) {
        repeat = true;
        printf("repeat: Yes");
    }
    else {
        repeat = false;
        printf("repeat: No");
    }
    
    bool toggleableMusic; //Art der Ansteuerung
    if (strcmp(pcValue[5], "1") == 0) {
        toggleableMusic = true;
        printf("toggleableMusic: Yes");
    }
    else {
        toggleableMusic = false;
        printf("toggleableMusic: No");
    }
    
    TimerArrays& ta = TimerArrays::getInstance();
    
    //Abspeichern des Music-Timer-Objektes mit allen Attributen
    if(strcmp(pcValue[6], "1") == 0){
        MusicTimer musicTimer; //Leeren des Objektes mit dem angegeben Index, wenn auf Webseite "Timer Löschen" ausgewählt wurde
        ta.replacemMusicTimer(indexMusic, musicTimer);
    }
    else{
        MusicTimer musicTimer(buttonMusic, folder, track, repeat, toggleableMusic);
        ta.replacemMusicTimer(indexMusic, musicTimer);
    }

    return "/index.shtml"; //Aktualisierung der Webseite
}

//CGI handler der auf /start.cgi reagiert
//Gibt Signal zum Start der Routine an main.cpp
const char * cgi_start_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    printf("start\n");
    TimerArrays& ta = TimerArrays::getInstance();
    ta.set_timers_got_updated(); //Startsignal
    return "/index.shtml";
}

//CGI-Array, enthält die URIs und ihre zugeordneten Funktionen, die über CGI aufgerufen werden
static const tCGI cgi_handlers[] = {
    {
        //HTML-Anfrage an  "/values.cgi" aktiviert die Funktion "cgi_value_handler"
        "/values.cgi", cgi_value_handler
    },
    {
        //HTML-Anfrage an "/music.cgi" aktiviert die Funktion "cgi_music_handler"
        "/music.cgi", cgi_music_handler
    },
    {
        //HTML-Anfrage an "/music.cgi" aktiviert die Funktion "cgi_start_handler"
        "/start.cgi", cgi_start_handler
    },
};

//Festlegen des CGI-Arrays
void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 3);
}