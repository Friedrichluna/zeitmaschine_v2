//Kopiert von Github
#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"


#define MAX_RELAY_TIMERS 32
#define MAX_RELAY_AMOUNT 16

#define WIFI_SSID "TABLET"
#define WIFI_PASSWORD "12345678"

#define CONNECTION_TIMEOUT_MS 10000



class WebInterface {
private:
   bool notconnected;

public:
    WebInterface(){
        //const char WIFI_SSID[] = "TABLET";
        //const char WIFI_PASSWORD[] = "12345678";
    }

    bool init_webInterface() {
        stdio_init_all(); 

        cyw43_arch_init();

        cyw43_arch_enable_sta_mode();

        // Connect to the WiFI network - loop until connected
        printf("Attempting to connect...\n");
        notconnected = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, CONNECTION_TIMEOUT_MS);
        // Print a success message once connected
        if(!notconnected){
            printf("Connected! \n");
        
            // Initialise web server
            httpd_init();
            printf("Http server initialised\n");

            // Configure SSI and CGI handler
            ssi_init(); 
            printf("SSI Handler initialised\n");
            cgi_init();
            printf("CGI Handler initialised\n");

            return !notconnected;
        }
        else{
            printf("No Connection found for SSID: %s and Password: %s!\n", WIFI_SSID, WIFI_PASSWORD);
            return !notconnected;
        } 


    }
};
