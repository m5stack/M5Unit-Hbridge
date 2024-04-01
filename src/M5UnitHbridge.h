#ifndef __UNIT_HBRIDGE_H
#define __UNIT_HBRIDGE_H

#include "Arduino.h"
#include "Wire.h"

#define HBRIDGE_I2C_ADDR               0x20
#define HBRIDGE_CONFIG_REG             0x00
#define HBRIDGE_MOTOR_ADC_8BIT_REG     0x10
#define HBRIDGE_MOTOR_ADC_12BIT_REG    0x20
#define HBRIDGE_MOTOR_CURRENT_REG      0x30
#define HBRIDGE_JUMP_TO_BOOTLOADER_REG 0xFD
#define HBRIDGE_FW_VERSION_REG         0xFE
#define HBRIDGE_I2C_ADDRESS_REG        0xFF


typedef enum {
    HBRIDGE_STOP = 0,
    HBRIDGE_FORWARD,
    HBRIDGE_BACKWARD
} hbridge_direction_t;

typedef enum { _8bit = 0, _12bit } hbridge_anolog_read_mode_t;

class M5UnitHbridge {
   private:
    uint8_t _addr;
    TwoWire* _wire;
    uint8_t _scl;
    uint8_t _sda;
    uint8_t _speed;
    void writeBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);
    void readBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);

   public:
    /**
     * @brief
     *
     * @param wire
     * @param addr device i2c addr
     * @param sda sda pin number
     * @param scl scl pin number
     * @param speed i2c speed
     * @return bool true=success / false=fail
     */
    bool begin(TwoWire* wire = &Wire, uint8_t addr = HBRIDGE_I2C_ADDR,
               uint8_t sda = 21, uint8_t scl = 22, uint32_t speed = 100000L);

    /**
     * @brief Get the Driver Direction object
     *
     * @return uint8_t 0=HBRIDGE_STOP, 1=HBRIDGE_FORWARD, 2=HBRIDGE_BACKWARD
     */
    uint8_t getDriverDirection(void);

    /**
     * @brief Get the Driver Speed8 Bits object
     *
     * @return uint8_t 8bit speed
     */
    uint8_t getDriverSpeed8Bits(void);

    /**
     * @brief Get the Driver Speed16 Bits object
     *
     * @return uint16_t 16bit speed
     */
    uint16_t getDriverSpeed16Bits(void);

    /**
     * @brief Set the Driver Direction object
     *
     * @param dir 0=HBRIDGE_STOP, 1=HBRIDGE_FORWARD, 2=HBRIDGE_BACKWARD
     */
    void setDriverDirection(hbridge_direction_t dir);

    /**
     * @brief Set the Driver Speed8 Bits object
     *
     * @param speed 8bit speed
     */
    void setDriverSpeed8Bits(uint8_t speed);

    /**
     * @brief Set the Driver Speed16 Bits object
     *
     * @param speed 16bit speed
     */
    void setDriverSpeed16Bits(uint16_t speed);

    /**
     * @brief Get the Driver PWM Freq object
     *
     * @return uint16_t PWM freq
     */
    uint16_t getDriverPWMFreq(void);

    /**
     * @brief Set the Driver PWM Freq object
     *
     * @param freq PWM freq
     */
    void setDriverPWMFreq(uint16_t freq);

    /**
     * @brief Get the Motor Current object, only 1.1 version Hbridge support
     *
     * @return float (A)
     */
    float getMotorCurrent(void);

    /**
     * @brief Get the Analog Input object
     *
     * @param bit 8bit / 12bit
     * @return uint16_t read input voltage raw ADC
     */
    uint16_t getAnalogInput(hbridge_anolog_read_mode_t bit);

    /**
     * @brief internal stm32 jump to bootloader for firmware update
     *
     */
    void jumpBootloader(void);

    /**
     * @brief Get the Firmware Version object
     *
     * @return uint8_t fw version number
     */
    uint8_t getFirmwareVersion(void);

    /**
     * @brief get device i2c addr, read only
     *
     * @return uint8_t
     */
    uint8_t getI2CAddress(void);
};

#endif
