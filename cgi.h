#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>

// CGI handler which is run when a request for /value.cgi is detected
const char * cgi_value_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    printf("frequency: %s\n", pcValue[0]);
    
    printf("length: %s\n", pcValue[1]);

    printf("toggleable: %s\n", pcValue[2]);
    
    int numberRelaysSelected = 0;
    int counter = 0;
    for(int i = 0; i < 16; i++){
        numberRelaysSelected = numberRelaysSelected + atoi(pcValue[3 + i]); //"atoi" converts type "char *" to integer
    }

    int selectedRelays[numberRelaysSelected] = {};

    for(int j = 0; j < 16; j++){
        if(strcmp(pcValue[3 + j], "1") == 0){
            selectedRelays[counter] = j + 1;
            counter = counter + 1;
        }
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
        "/value.cgi", cgi_value_handler
    },
};

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 1);
}
