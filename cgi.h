#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "musicTimer.h"

#define RELAY_TIMER_AMOUNT 16
#define MUSIC_TIMER_AMOUNT 16
#define RELAY_ARR_OFFSET 4



// CGI handler which is run when a request for /value.cgi is detected
const char * cgi_value_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    printf("button: %s\n", pcValue[0]);
    int button = atoi(pcValue[0]);

    printf("frequency: %s\n", pcValue[1]);
    int frequency = atoi(pcValue[1]);
    
    printf("length: %s\n", pcValue[2]);
    int length = atoi(pcValue[2]);

    printf("toggleable: %s\n", pcValue[3]);
    bool toggleable;
    if (strcmp(pcValue[3], "1") == 0)
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

        selectedRelays[j] = atoi(pcParam[RELAY_ARR_OFFSET + j]);
    }
    
    printf("selected Relays: ");
    for(int k = 0; k < numberRelaysSelected; k++){
        printf("%d ", selectedRelays[k]);
    }
    printf("\n");

    // Send the index page back to the user
    RelayTimer relayTimer(button, frequency, length, selectedRelays, numberRelaysSelected, toggleable);
    int index = 1; //TODOOOOOOOOOOOOOOOOOO

    RelayTimer relayTimer_arr[RELAY_TIMER_AMOUNT];


    relayTimer_arr[index] = relayTimer;
    
    return "/index.shtml";
}

// CGI handler which is run when a request for /music.cgi is detected
const char * cgi_music_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    printf("button for music: %s\n", pcValue[0]);
    int buttonMusic = atoi(pcValue[0]);

    printf("folder: %s\n", pcValue[1]);
    int folder = atoi(pcValue[1]);

    printf("track: %s\n", pcValue[2]);
    int track = atoi(pcValue[2]);

    bool repeat;
    if (strcmp(pcValue[3], "1") == 0) {
        repeat = true;
        printf("repeat: Yes");
    }
    else {
        repeat = false;
        printf("repeat: No");
    }
    bool toggleableMusic;
    if (strcmp(pcValue[4], "1") == 0) {
        toggleableMusic = true;
        printf("toggleableMusic: Yes");
    }
    else {
        toggleableMusic = false;
        printf("toggleableMusic: No");
    }
    MusicTimer musicTimer(buttonMusic, folder, track, repeat, toggleableMusic);

    MusicTimer musicTimer_arr[MUSIC_TIMER_AMOUNT];

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
};

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 2);
}