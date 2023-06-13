// Copyright (c) 2023 Leonhard Baschang

#include <Arduino.h>
#include "EEPROM_Microchip_25.h"


EEPROM_Microchip_25 eeprom(13,18,19,16,8,16);


void setup() {


    // Reads data in 0xFF and add 1 and updated 0xFF with the new value
    uint8_t data = eeprom.readByte(0xFF);
    eeprom.updateByte(0xFF, data + 1);


    // Writes to 0x00 to 0x04 the pageData
    uint8_t pageData[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    eeprom.writePage(0x00, pageData, 5);

}


void loop() {

}
