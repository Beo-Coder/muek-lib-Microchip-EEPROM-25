// Copyright (c) 2023 Leonhard Baschang

#ifndef INC_24_LIB_EEPROM_MICROCHIP_24_H
#define INC_24_LIB_EEPROM_MICROCHIP_24_H

#include "SPI.h"

#define DEFAULT_ADDRESS_BITS 8
#define MAX_PAGE_WRITE 16


class EEPROM_Microchip_25 {
#if defined(ARDUINO_RASPBERRY_PI_PICO)
    MbedSPI *spi;
#else
    SPIClass *spi;
#endif

    uint8_t addressBytes;
    uint8_t maxPageWrite;
    uint8_t *writeData = nullptr;
    uint8_t *buffer = nullptr;

    uint8_t csPin;


    void enableWrite();

    void disableWrite();

    uint8_t readStatusRegister();

    void waitUntilWriteDone();

public:

    EEPROM_Microchip_25(uint8_t csPin, uint8_t sckPin=PIN_SPI_SCK, uint8_t mosiPin= PIN_SPI_MOSI, uint8_t misoPin = PIN_SPI_MISO, uint8_t addressBits = DEFAULT_ADDRESS_BITS, uint8_t maxPageWrite = MAX_PAGE_WRITE);


    uint8_t *readSequentialByte(uint32_t address, uint8_t byteCount);

    uint8_t readByte(uint32_t address);

    void writeByte(uint32_t address, uint8_t data);

    void writePage(uint32_t address, uint8_t *data, uint8_t dataLength);

    bool updateByte(uint32_t address, uint8_t data);

    bool updatePage(uint32_t startAddress, uint8_t *data, uint8_t length);

};


#endif //INC_24_LIB_EEPROM_MICROCHIP_24_H
