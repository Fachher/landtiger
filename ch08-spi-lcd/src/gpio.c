#include "lpc17xx.h"
#include "gpio.h"

LPC_GPIO_TypeDef *LPC_GPIO[] = {LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3, LPC_GPIO4};

void digitalWrite(int port, int pin, int val) {
    if (port >= 0 && port <= 4) {
        if (val) {
            LPC_GPIO[port]->FIOSET |= (1 << pin);
        } else {
            LPC_GPIO[port]->FIOCLR |= (1 << pin);
        }
    }
}


