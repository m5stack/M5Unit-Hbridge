/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套 M5Core 示例源代码
**
* Product: Hbridge Unit
* getMotorCurrent() only support in v1.1 function
* Date: 2023/8/9
*******************************************************************************
*/

#include <M5Stack.h>
#include <M5GFX.h>
#include "UNIT_HBRIDGE.h"

M5GFX display;
M5Canvas canvas(&display);
UNIT_HBRIDGE driver;

void setup() {
    M5.begin();
    display.begin();
    canvas.setColorDepth(8);
    canvas.setFont(&fonts::efontCN_12);
    canvas.setTextSize(2);
    canvas.setTextColor(ORANGE);
    canvas.createSprite(display.width(), display.height());
    canvas.pushSprite(0, 0);
    driver.begin(&Wire, HBRIDGE_ADDR, 21, 22, 100000L);
}

void loop() {
    M5.update();
    if (M5.BtnA.wasPressed()) {
        driver.setDriverDirection(1);
        driver.setDriverSpeed8Bits(127);
    } else if (M5.BtnB.wasPressed()) {
        driver.setDriverDirection(2);
        driver.setDriverSpeed8Bits(127);
    } else if (M5.BtnC.wasPressed()) {
        driver.setDriverDirection(0);
        driver.setDriverSpeed8Bits(0);
    }

    canvas.fillSprite(0);
    canvas.setTextColor(ORANGE);
    canvas.setTextSize(2);
    canvas.drawString("HBridge Demo", 90, 0);
    // getMotorCurrent() function only support in V1.1 version
    if (driver.getFirmwareVersion == 2) {
        canvas.setCursor(10, 100);
        canvas.setTextColor(WHITE);
        canvas.setTextSize(2);
        canvas.printf("Current:");
        canvas.setTextColor(GREEN);
        canvas.setTextSize(3);
        canvas.printf("%.2fA", driver.getMotorCurrent());
        canvas.setTextSize(2);
    }
    canvas.setCursor(10, 50);
    canvas.setTextColor(WHITE);
    canvas.setTextSize(2);
    canvas.printf("Voltage:");
    canvas.setTextColor(GREEN);
    canvas.setTextSize(3);
    canvas.printf("%.2fV",
                  driver.getAnalogInput(_12bit) / 4095.0f * 3.3f / 0.09f);
    canvas.setTextSize(2);
    canvas.setTextColor(ORANGE);
    canvas.drawString("Forward", 30, 190);
    canvas.drawString("(50%)", 30, 210);
    canvas.drawString("Reverse", 130, 190);
    canvas.drawString("(50%)", 130, 210);
    canvas.drawString("Stop", 230, 190);
    canvas.pushSprite(0, 0);
}