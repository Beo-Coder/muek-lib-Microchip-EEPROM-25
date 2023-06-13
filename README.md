# Microchip EEPROM library

## Arduino library for Microchip EEPROMs (25...)

# Features

- read/write single byte
- read/write page of bytes
- updated bytes/pages
- Works on AVR and RP2040 based boards (Arduino Uno, Nano, Mega, ... and Raspberry Pi Pico, RP2040)

# Usage

### Initialize EEPROM:
``EEPROM_Microchip_24 eeprom(<csPin>, <sckPin>, <mosiPin>, <misoPin>, <addressBytes>, <maxPageWrite>);``

- **csPin**: chip select pin on EEPROM
- **sckPin**: pin of sck pin (only for boards that support different spi pins)
- **mosiPin**: pin of mosi (si) pin (only for boards that support different spi pins)
- **misoPin**: pin of miso (so) pin (only for boards that support different spi pins)
- **addressBytes**: number of bits of the address for the eeprom (e.g. 8 for 2 KBit version; 16 for 128 KBit version)
- **maxPageWrite**: maximum number of bytes that can be written at once (e.g. 16 for the 2 KBit version)



### Write single byte
``eeprom.writeByte(<address>, <data>);``

- **address**: the memory address of the byte e.g. (0xFF)
- **data**: 8-bit data that should be stored


### Write page of bytes
``eeprom.writePage(<startAddress>, <data>, <dataLength>);``

- **startAddress**: the memory address for the first byte e.g. (0xFF)
- **data**: array of 8-bit data
- **dataLength**: the size of the data array (number of bytes)



### Read single byte
``eeprom.readByte(<address>);``

- **address**: the memory address of the byte e.g. (0xFF)

Returns the 8-bit data of the given address.



### Read multiple bytes
``eeprom.readSequentialByte(<startAddress>, <length>);``

- **startAddress**: the memory address of the first byte to read e.g. (0xFF)
- **length**: number of bytes to be read

Return a pointer to an array of 8-bit data with the given size.



### Update single byte
``eeprom.updateByte(<address>, <data>);``

- **address**: the memory address of byte e.g. (0xFF)
- **data**: data that should be written to the given address

Returns ``true`` or ``false`` depending on whether the byte was changed or not.




### Update multiple bytes
``eeprom.updatePage(<startAddress>, <data>, <length>);``

- **startAddress**: the memory address of the first byte e.g. (0xFF)
- **data**: data array of 8-bit data that should be written to the given address
- **length**: the size of the data array (number of bytes)

Returns ``true`` or ``false`` depending on whether a byte was changed or not.


# Notes

- ```update``` should be executed instead of ``write``, because EEPROM has only limited write cycles
- Tested on Raspberry Pi Pico and Arduino Uno
- Only tested with the 2-KBit version (25LC020) but should also work on all other capacities
