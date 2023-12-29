#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define RELAY_ARR_OFFSET 4

// CGI handler which is run when a request for /value.cgi is detected
const char * cgi_value_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{

    printf("frequency: %s\n", pcValue[0]);
    
    printf("length: %s\n", pcValue[1]);

    printf("toggleable: %s\n", pcValue[2]);

    printf("button: %s\n", pcValue[3]);
    
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
    return "/index.shtml";
}

// tCGI Struct
// Fill this with all of the CGI requests and their respective handlers
static const tCGI cgi_handlers[] = {
    {
        // Html request for "/led.cgi" triggers cgi_handler
        "/values.cgi", cgi_value_handler
    },
};

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 1);
}
