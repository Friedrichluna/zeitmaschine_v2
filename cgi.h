#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "musicTimer.h"
#include "timerArrays.h"

#define RELAY_TIMER_AMOUNT 16
#define MUSIC_TIMER_AMOUNT 16
#define RELAY_ARR_OFFSET 5

int map_button(int index);
int map_relay(int index);

// CGI handler which is run when a request for /value.cgi is detected
const char * cgi_value_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    printf("index: %s\n", pcValue[0]);
    int index = atoi(pcValue[0]);

    printf("button: %s\n", pcValue[1]);
    int button = map_button(atoi(pcValue[1]));

    printf("frequency: %s\n", pcValue[2]);
    int frequency = atoi(pcValue[2]);
    
    printf("length: %s\n", pcValue[3]);
    int length = atoi(pcValue[3]);

    printf("toggleable: %s\n", pcValue[4]);
    bool toggleable;
    if (strcmp(pcValue[4], "1") == 0)
        toggleable = true;
    else
        toggleable = false;
    
    int numberRelaysSelected = 0;
    for(int i = 0; i < 16; i++){
        if (strcmp(pcValue[RELAY_ARR_OFFSET + i], "end_of_array") == 0) {
            numberRelaysSelected = i;
            printf("numberRelaysSelected: %d\n", i);
            break;
        }
    }

    int selectedRelays[numberRelaysSelected] = {};

    for(int j = 0; j < numberRelaysSelected; j++){

        selectedRelays[j] = map_relay(atoi(pcParam[RELAY_ARR_OFFSET + j]));
    }
    printf("numberrelysselected: %d\n",numberRelaysSelected);
    printf("selected Relays: ");
    for(int k = 0; k < numberRelaysSelected; k++){
        printf("%d ", selectedRelays[k]);
    }
    printf("\n");

    // Send the index page back to the user
    
    
    TimerArrays& ta = TimerArrays::getInstance();
    
    if(numberRelaysSelected == 0){
        RelayTimer relayTimer;
        ta.replaceRelayTimer(index, relayTimer);
    }
    else{
        RelayTimer relayTimer(button,frequency, length, selectedRelays, numberRelaysSelected, toggleable);
        ta.replaceRelayTimer(index, relayTimer);
    }

    printf("succsess");
    return "/index.shtml";
}

// CGI handler which is run when a request for /music.cgi is detected
const char * cgi_music_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    printf("index: %s\n", pcValue[0]);
    int indexMusic = atoi(pcValue[0]);
    
    printf("button for music: %s\n", pcValue[1]);
    int buttonMusic = atoi(pcValue[1]);

    printf("folder: %s\n", pcValue[2]);
    int folder = atoi(pcValue[2]);

    printf("track: %s\n", pcValue[3]);
    int track = atoi(pcValue[3]);

    bool repeat;
    if (strcmp(pcValue[4], "1") == 0) {
        repeat = true;
        printf("repeat: Yes");
    }
    else {
        repeat = false;
        printf("repeat: No");
    }
    bool toggleableMusic;
    if (strcmp(pcValue[5], "1") == 0) {
        toggleableMusic = true;
        printf("toggleableMusic: Yes");
    }
    else {
        toggleableMusic = false;
        printf("toggleableMusic: No");
    }
    
    TimerArrays& ta = TimerArrays::getInstance();
    
    if(strcmp(pcValue[6], "1") == 0){
        MusicTimer musicTimer;
        ta.replacemMusicTimer(indexMusic, musicTimer);
    }
    else{
        MusicTimer musicTimer(buttonMusic, folder, track, repeat, toggleableMusic);
        ta.replacemMusicTimer(indexMusic, musicTimer);
    }

    return "/index.shtml";
}

const char * cgi_start_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    printf("start\n");
    TimerArrays& ta = TimerArrays::getInstance();
    ta.set_timers_got_updated();
    return "/index.shtml";
}

// tCGI Struct
// Fill this with all of the CGI requests and their respective handlers
static const tCGI cgi_handlers[] = {
    {
        // Html request for "/values.cgi" triggers cgi_handler
        "/values.cgi", cgi_value_handler
    },
    {
        // Html request for "/music.cgi" triggers cgi_handler
        "/music.cgi", cgi_music_handler
    },
    {
        // Html request for "/music.cgi" triggers cgi_handler
        "/start.cgi", cgi_start_handler
    },
};

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 3);
}