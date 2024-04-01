/**
 * @file demo.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief
 * @version 0.1
 * @date 2024-04-01
 *
 *
 * @Hardwares: M5Core + Unit Hbridge v1.1
 * @Platform Version: Arduino M5Stack Board Manager v2.1.1
 * @Dependent Library:
 * M5UnitHbridge: https://github.com/m5stack/M5Unit-Hbridge
 */

#include "M5UnitHbridge.h"

M5UnitHbridge driver;
uint8_t fw_version = 0;

void setup() {
    Serial.begin(115200);

    while (!driver.begin(&Wire, HBRIDGE_I2C_ADDR, 21, 22, 100000L)) {
        Serial.println("Hbridge not found");
        delay(1000);
    }

    fw_version = driver.getFirmwareVersion();

    Serial.printf("Hbridge Firmware Version: %d\r\n", fw_version);
}

void get_current_voltage() {
    // getMotorCurrent() function only support in Hbridge V1.1 version
    if (fw_version >= 2) {
        Serial.printf("%.2fA\r\n", driver.getMotorCurrent());
    }
    Serial.printf("%.2fV\r\n",
                  driver.getAnalogInput(_12bit) / 4095.0f * 3.3f / 0.09f);
}

void loop() {
    driver.setDriverDirection(HBRIDGE_FORWARD);
    driver.setDriverSpeed8Bits(127);
    for (int i = 100; i > 0; i--) {
        get_current_voltage();
    }

    driver.setDriverDirection(HBRIDGE_STOP);
    driver.setDriverSpeed8Bits(0);
    for (int i = 100; i > 0; i--) {
        get_current_voltage();
    }

    driver.setDriverDirection(HBRIDGE_BACKWARD);
    driver.setDriverSpeed8Bits(127);

    for (int i = 100; i > 0; i--) {
        get_current_voltage();
    }

    driver.setDriverDirection(HBRIDGE_STOP);
    driver.setDriverSpeed8Bits(0);
    for (int i = 100; i > 0; i--) {
        get_current_voltage();
    }
}
