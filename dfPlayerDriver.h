#include "pico/stdlib.h"

#include "dfPlayer/dfPlayer.h"
#define TX 8
#define RX 9

class DfPlayerPico : public DfPlayer<DfPlayerPico>
{
    public:
    DfPlayerPico();
    inline void uartSend(uint8_t* a_cmd);
    
    private:

};

DfPlayerPico::DfPlayerPico()
{
    // Initialise UART 1
    uart_init(uart1, 9600);
 
    // Set the GPIO pin mux to the UART - 16 is TX, 17 is RX
    gpio_set_function(TX, GPIO_FUNC_UART);
    gpio_set_function(RX, GPIO_FUNC_UART);
}

inline void DfPlayerPico::uartSend(uint8_t* a_cmd)
{
    uart_write_blocking(uart1, a_cmd, SERIAL_CMD_SIZE);
}