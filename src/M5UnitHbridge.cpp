#include "M5UnitHbridge.h"

void M5UnitHbridge::writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                               uint8_t length) {
    _wire->beginTransmission(addr);
    _wire->write(reg);
    for (int i = 0; i < length; i++) {
        _wire->write(*(buffer + i));
    }
    _wire->endTransmission();
}

void M5UnitHbridge::readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                              uint8_t length) {
    uint8_t index = 0;
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->endTransmission();
    _wire->requestFrom(addr, length);
    for (int i = 0; i < length; i++) {
        buffer[index++] = _wire->read();
    }
}

bool M5UnitHbridge::begin(TwoWire *wire, uint8_t addr, uint8_t sda, uint8_t scl,
                          uint32_t speed) {
    _wire  = wire;
    _addr  = addr;
    _sda   = sda;
    _scl   = scl;
    _speed = speed;
    _wire->begin(_sda, _scl, _speed);
    delay(10);
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();
    if (error == 0) {
        return true;
    } else {
        return false;
    }
}

uint8_t M5UnitHbridge::getDriverDirection(void) {
    uint8_t data[4];
    readBytes(_addr, HBRIDGE_CONFIG_REG, data, 1);
    return data[0];
}

uint8_t M5UnitHbridge::getDriverSpeed8Bits(void) {
    uint8_t data[4];
    readBytes(_addr, HBRIDGE_CONFIG_REG + 1, data, 1);
    return data[0];
}

uint16_t M5UnitHbridge::getDriverSpeed16Bits(void) {
    uint8_t data[4];
    readBytes(_addr, HBRIDGE_CONFIG_REG + 2, data, 2);
    return (data[0] | (data[1] << 8));
}

uint16_t M5UnitHbridge::getDriverPWMFreq(void) {
    uint8_t data[4];
    readBytes(_addr, HBRIDGE_CONFIG_REG + 4, data, 2);
    return (data[0] | (data[1] << 8));
}

void M5UnitHbridge::setDriverPWMFreq(uint16_t freq) {
    uint8_t data[4];
    data[0] = (freq & 0xff);
    data[1] = ((freq >> 8) & 0xff);
    writeBytes(_addr, HBRIDGE_CONFIG_REG + 4, data, 2);
}

void M5UnitHbridge::setDriverDirection(hbridge_direction_t dir) {
    uint8_t data[4];
    data[0] = dir;
    writeBytes(_addr, HBRIDGE_CONFIG_REG, data, 1);
}

void M5UnitHbridge::setDriverSpeed8Bits(uint8_t speed) {
    uint8_t data[4];
    data[0] = speed;
    writeBytes(_addr, HBRIDGE_CONFIG_REG + 1, data, 1);
}

void M5UnitHbridge::setDriverSpeed16Bits(uint16_t speed) {
    uint8_t data[4];
    data[0] = speed;
    data[1] = (speed >> 8);
    writeBytes(_addr, HBRIDGE_CONFIG_REG + 2, data, 2);
}

uint16_t M5UnitHbridge::getAnalogInput(hbridge_anolog_read_mode_t bit) {
    if (bit == _8bit) {
        uint8_t data;
        readBytes(_addr, HBRIDGE_MOTOR_ADC_8BIT_REG, &data, 1);
        return data;
    } else {
        uint8_t data[2];
        readBytes(_addr, HBRIDGE_MOTOR_ADC_12BIT_REG, data, 2);
        return (data[0] | (data[1] << 8));
    }
}

// Only V1.1 can use this
float M5UnitHbridge::getMotorCurrent(void) {
    uint8_t data[4];
    float c;
    uint8_t *p;

    readBytes(_addr, HBRIDGE_MOTOR_CURRENT_REG, data, 4);
    p = (uint8_t *)&c;
    memcpy(p, data, 4);

    return c;
}

uint8_t M5UnitHbridge::getFirmwareVersion(void) {
    uint8_t data[4];
    readBytes(_addr, HBRIDGE_FW_VERSION_REG, data, 1);
    return data[0];
}

uint8_t M5UnitHbridge::getI2CAddress(void) {
    uint8_t data[4];
    readBytes(_addr, HBRIDGE_I2C_ADDRESS_REG, data, 1);
    return data[0];
}

// Only V1.1 can use this
void M5UnitHbridge::jumpBootloader(void) {
    uint8_t value = 1;

    writeBytes(_addr, HBRIDGE_JUMP_TO_BOOTLOADER_REG, (uint8_t *)&value, 1);
}
