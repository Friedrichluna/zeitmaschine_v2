#include "pico/stdlib.h"

#include "dfPlayer/dfPlayer.h"

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
 
    // Set the GPIO pin mux to the UART - 8 is TX, 9 is RX
    gpio_set_function(8, GPIO_FUNC_UART);
    gpio_set_function(9, GPIO_FUNC_UART);
}

inline void DfPlayerPico::uartSend(uint8_t* a_cmd)
{
    uart_write_blocking(uart1, a_cmd, SERIAL_CMD_SIZE);
}