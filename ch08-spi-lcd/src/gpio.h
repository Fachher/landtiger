//
// Created by Fachher Syed on 26.08.23.
//

#ifndef HELLOWORLD_GPIO_H
#define HELLOWORLD_GPIO_H

void digitalWrite(int port, int pin, int val);

#define digitalWritePort0(pin1, val) digitalWrite(0, pin1, val)
#define digitalWritePort1(pin1, val) digitalWrite(1, pin1, val)
#define digitalWritePort2(pin1, val) digitalWrite(2, pin1, val)
#define digitalWritePort3(pin1, val) digitalWrite(3, pin1, val)
#define digitalWritePort4(pin1, val) digitalWrite(4, pin1, val)


#endif //HELLOWORLD_GPIO_H
