//Quellen: https://www.makermatrix.com/blog/read-and-write-data-with-the-pi-pico-onboard-flash/
#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include <hardware/sync.h>
#include "hardware/flash.h"
#include "flash_utils.h"


class DataSaver {
private:
    int flash_array_length = 64;
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
                printf("%d\n",i);
        }
    }

    void print_buf_int(int len) {
        u_int16_t* buf = (u_int16_t*) getAddressPersistent();
        int counter = 0;
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
            if (i % 32 == 31) {
                counter++;
                printf("%d\n",counter);
            }
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


    void flash_objects(RelayTimer* relayTimer_array, int length_rt, MusicTimer* musicTimer_array, int length_mt) {
        int saver_array[(flash_array_length/2)*(length_rt+length_mt)];
        printf("\nstarting flash\n");
        
        for (int i = 0; i < length_rt; i++) {
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
        for (int i = 0; i < length_mt; i++) {
            int shift = i*32 + (flash_array_length/2)*length_rt;
            
            saver_array[0 + shift] = musicTimer_array[i].button;
            saver_array[1 + shift] = musicTimer_array[i].folder;
            saver_array[2 + shift] = musicTimer_array[i].track;
            if (musicTimer_array[i].repeat) {
                saver_array[3 + shift] = 1;
            }
            else {
                saver_array[3 + shift] = 0;
            }

            if (musicTimer_array[i].toggleable) {
                saver_array[4 + shift] = 1;
            }
            else {
                saver_array[4 + shift] = 0;
            }

            if (musicTimer_array[i].dummy) {
                saver_array[5 + shift] = 1;
            }
            else {
                saver_array[5 + shift] = 0;
            }
        }
        u_int8_t saver_array_int8[flash_array_length*(length_rt+length_mt)];
        for (int i = 0; i < flash_array_length*(length_rt+length_mt); i++) {
            saver_array_int8[2 * i + 1] = (u_int8_t) (saver_array[i] >> 8);
            saver_array_int8[2 * i] = (u_int8_t) (saver_array[i]);
        }



        erase_target_flash();
        printf("\nProgramming target region...\n");
        uint32_t ints_ = save_and_disable_interrupts(); 
        flash_range_program(flash_target_offset, saver_array_int8 , flash_array_length*(length_rt+length_mt));
        restore_interrupts(ints_);
        printf("done\n");
        print_buf_int(flash_array_length*(length_rt+length_mt));
        


        uint8_t random_data[FLASH_PAGE_SIZE]; //wenn man diese Zeilen löscht, chrashed das Programm
        for (int i = 0; i < FLASH_PAGE_SIZE; ++i)
            random_data[i] = 0;
    }


    RelayTimer recover_relayTimer(int index) {
        int shift = index*(flash_array_length/2);
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
            printf("recovered: relayTimer[%d]\n",index);
            return relayTimer;
        }
        return relayTimer;
    }

    MusicTimer recover_musicTimer(int index, int length_rt) {
        int shift = index*32 + (flash_array_length/2)*length_rt;
        u_int16_t* this_relays_pointer = (u_int16_t*) getAddressPersistent();

        int button = (int) this_relays_pointer[0 + shift];
        int folder = (int) this_relays_pointer[1 + shift];
        int track = (int) this_relays_pointer[2 + shift];
        bool repeat;
        if (this_relays_pointer[3 + shift] == 1) {
            repeat = true;
        }
        else {
            repeat = false;
        }

        bool toggleable;
        if (this_relays_pointer[4 + shift] == 1) {
            toggleable = true;
        }
        else {
            toggleable = false;
        }
        
        bool dummy;
        if (this_relays_pointer[5 + shift] != 0) {
            printf("MusicTimer %d is dummy\n", index);
            dummy = true;
            MusicTimer musicTimer;  //erstellt dummy objekt
            return musicTimer;
        }
        printf("pointer: %d of index: %d\n",this_relays_pointer[5 + shift], index);
        MusicTimer musicTimer(button, folder, track, repeat, toggleable);
        printf("recovered: MusicTimer[%d]\n",index);
        return musicTimer;
    }
};




