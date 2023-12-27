//Quellen: https://www.makermatrix.com/blog/read-and-write-data-with-the-pi-pico-onboard-flash/
#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include <hardware/sync.h>
#include "hardware/flash.h"
#include "flash_utils.h"


class DataSaver {
private:
    
public:
    DataSaver(){
        data_saver_init();
    };

    const uint8_t* flash_target_contents  = (uint8_t*) getAddressPersistent();
    uint32_t flash_target_offset_incl_RAM = (uint32_t) flash_target_contents; 
    uint32_t flash_target_offset = flash_target_offset_incl_RAM - XIP_BASE;

    void data_saver_init() {
        char addr[80];
        printf("adressPersistent: %p\n", getAddressPersistent());

        sprintf(addr, "address = %x", getAddressPersistent()); 
    }
    
    void print_buf(const uint8_t *buf, size_t len) {
        for (size_t i = 0; i < len; ++i) {
            printf("%02x", buf[i]);
            if (i % 16 == 15)
                printf("\n");
            else
                printf(" ");
            if (i % 64 == 63)
                printf("\n");
        }
    }

    void print_buf_int(int len) {
        u_int16_t* buf = (u_int16_t*) getAddressPersistent();
        for (size_t i = 0; i < len; ++i) {
            printf("%05d", buf[i]);
            if (i % 8 == 7)
                printf("\n");
            else if (i == 17)
                printf("|");
            else if (i == 2)
                printf("|");
            else
                printf(" ");
            if (i % 32 == 31)
                printf("\n");
            
        }
    }

    void erase_target_flash() {
        printf("\nErasing target region: %d ...\n", flash_target_offset);
        uint32_t ints = save_and_disable_interrupts(); 
        flash_range_erase(flash_target_offset, FLASH_SECTOR_SIZE);
        restore_interrupts (ints);
        printf("Done.\n");
        //print_buf_int(FLASH_SECTOR_SIZE);
    }


    void flash_objects(RelayTimer* relayTimer_array, int length) {
        int flash_array_length = 64;
        int saver_array[(flash_array_length/2)*length];
        
        
        for (int i = 0; i < length; i++) {
            int shift = i*32;
            
            saver_array[0 + shift] = relayTimer_array[i].gpio_pin_switch;
            saver_array[1 + shift] = relayTimer_array[i].frequency;
            saver_array[2 + shift] = relayTimer_array[i].length_sec;
            for (int j = 0; j < relayTimer_array[i].relay_amount; j++) {
                saver_array[j + 3 + shift] = relayTimer_array[i].relay_arr[j];
            }
            saver_array[18 + shift] = relayTimer_array[i].relay_amount;
            if (relayTimer_array[i].toggleable) {
                saver_array[19 + shift] = 1;
            }
            else {
                saver_array[19 + shift] = 0;
            }
            if (relayTimer_array[i].dummy) {
                saver_array[20 + shift] = 1;
            }
            else {
                saver_array[20 + shift] = 0;
            }
        }
        u_int8_t saver_array_int8[flash_array_length*length];
        for (int i = 0; i < flash_array_length*length; i++) {
            saver_array_int8[2 * i + 1] = (u_int8_t) (saver_array[i] >> 8);
            saver_array_int8[2 * i] = (u_int8_t) (saver_array[i]);
        }



        erase_target_flash();
        printf("\nProgramming target region...\n");
        uint32_t ints_ = save_and_disable_interrupts(); 
        flash_range_program(flash_target_offset, saver_array_int8 , flash_array_length*length);
        restore_interrupts(ints_);
        printf("done\n");
        //print_buf_int(FLASH_SECTOR_SIZE);;
        


        uint8_t random_data[FLASH_PAGE_SIZE]; //wenn man diese Zeilen löscht, chrashed das Programm
        for (int i = 0; i < FLASH_PAGE_SIZE; ++i)
            random_data[i] = 0;
    }


    RelayTimer recover_relayTimer(int index) {
        int shift = index*32;
        u_int16_t* this_relays_pointer = (u_int16_t*) getAddressPersistent();

        int gpio_pin_switch = (int) this_relays_pointer[0 + shift];
        int frequency = (int) this_relays_pointer[1 + shift];
        int length_sec = (int) this_relays_pointer[2 + shift];
        int relay_amount = (int) this_relays_pointer[18 + shift];
        int relay_arr[16];
        for (int j = 0; j < relay_amount; j++) {
            relay_arr[j] = (int) this_relays_pointer[j + 3 + shift];
        }
        bool toggleable;
        if (this_relays_pointer[19 + shift] == 1) {
            toggleable = true;
        }
        else {
            toggleable = false;
        }
        RelayTimer relayTimer; //erstellt dummy objekt
        bool dummy;
        if (this_relays_pointer[20 + shift] == 1) {
            dummy = true;
        }
        else {
            dummy = false;
            RelayTimer relayTimer(gpio_pin_switch, frequency, length_sec, relay_arr, relay_amount, toggleable);
        }
        return relayTimer;
    }
};




