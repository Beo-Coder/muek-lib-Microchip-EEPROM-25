
// Copyright (c) 2023 Leonhard Baschang

#include "Arduino.h"
#include "SPI.h"
#include "EEPROM_Microchip_25.h"


EEPROM_Microchip_25::EEPROM_Microchip_25(uint8_t csPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
                                         uint8_t addressBits,
                                         uint8_t maxPageWrite) {

    this->maxPageWrite = maxPageWrite;
    this->csPin = csPin;

    addressBytes = (0x01 << addressBits) / 256;

    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);
#if defined(ARDUINO_RASPBERRY_PI_PICO)
    spi = new MbedSPI(misoPin, mosiPin, sckPin);
#else
    spi = new SPIClass();
#endif


    spi->begin();

}


uint8_t EEPROM_Microchip_25::readStatusRegister() {
    uint8_t writeData[0];
    writeData[0] = 0b00000101;
    digitalWrite(csPin, LOW);
    spi->transfer(writeData[0]);
    uint8_t data = spi->transfer(0x00);
    digitalWrite(csPin, HIGH);
    return data;

}

void EEPROM_Microchip_25::enableWrite() {
    digitalWrite(csPin, LOW);
    spi->transfer(0b00000110);
    digitalWrite(csPin, HIGH);
}

void EEPROM_Microchip_25::disableWrite() {
    digitalWrite(csPin, LOW);
    spi->transfer(0b00000100);
    digitalWrite(csPin, HIGH);
}

void EEPROM_Microchip_25::waitUntilWriteDone() {
    uint8_t status;
    do {
        status = readStatusRegister();

    } while ((status & 0x01) == 0x01);
}


uint8_t *EEPROM_Microchip_25::readSequentialByte(uint32_t startAddress, uint8_t byteCount) {

    writeData = new uint8_t[addressBytes + 1];


    writeData[0] = 0b00000011;
    for (int i = 1; i < addressBytes; i++) {
        writeData[i] = (startAddress >> ((addressBytes - 1 - i) * 8)) & 0xFF;
    }

    buffer = new uint8_t[byteCount];



    digitalWrite(csPin, LOW);

    spi->transfer(writeData[0]);
    for (int i = 1; i < addressBytes; i++) {
        spi->transfer(writeData[i]);
    }
    for (int i = 0; i < byteCount; i++) {
        buffer[i] = spi->transfer(0x00);
    }

    digitalWrite(csPin, HIGH);


    return buffer;
}

uint8_t EEPROM_Microchip_25::readByte(uint32_t address) {
    uint8_t *data;
    data = readSequentialByte(address, 1);
    return data[0];
}


void EEPROM_Microchip_25::writeByte(uint32_t address, uint8_t data) {
    waitUntilWriteDone();
    enableWrite();
    writeData = new uint8_t[addressBytes + 1];

    writeData[0] = 0b00000010;
    for (int i = 1; i < addressBytes; i++) {
        writeData[i] = (address >> ((addressBytes - 1 - i) * 8)) & 0xFF;
    }


    digitalWrite(csPin, LOW);

    spi->transfer(writeData[0]);
    for (int i = 1; i < addressBytes; i++) {
        spi->transfer(writeData[i]);
    }
    spi->transfer(data);

    digitalWrite(csPin, HIGH);

}

void EEPROM_Microchip_25::writePage(uint32_t address, uint8_t *data, uint8_t dataLength) {
    if (dataLength <= maxPageWrite) {


        waitUntilWriteDone();
        enableWrite();
        writeData = new uint8_t[addressBytes + 1];

        writeData[0] = 0b00000010;
        for (int i = 1; i < addressBytes; i++) {
            writeData[i] = (address >> ((addressBytes - 1 - i) * 8)) & 0xFF;
        }

        digitalWrite(csPin, LOW);
        spi->transfer(writeData[0]);
        for (int i = 1; i < addressBytes; i++) {
            spi->transfer(writeData[i]);
        }
        for (int i = 0; i < dataLength; i++) {
            spi->transfer(data[i]);
        }
        digitalWrite(csPin, HIGH);
    }

}


bool EEPROM_Microchip_25::updateByte(uint32_t address, uint8_t data) {
    uint8_t currentData = readByte(address);
    if (currentData != data) {
        writeByte(address, data);
        return true;
    } else {
        return false;
    }

}


bool EEPROM_Microchip_25::updatePage(uint32_t startAddress, uint8_t *data, uint8_t length) {
    bool changed = false;
    for (int i = 0; i < length; i++) {
        changed = updateByte(startAddress + i, data[i]);
    }

    return changed;

}









