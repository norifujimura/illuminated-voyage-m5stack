/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <M5Core2.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#include <Arduino.h>
#include "TCA9548A.h"

TCA9548A I2CMux;                  // Address can be passed into the constructor

#define REPORTING_PERIOD_MS     100

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup()
{
    Serial.begin(115200);

    Wire.begin(); // I2Cの初期化
        pinMode(21, INPUT_PULLUP); //デファルトのSDAピン21　のプルアップの指定
    pinMode(22, INPUT_PULLUP); //デファルトのSCLピン22　のプルアップの指定
  //Wire.setPins(21, 22);       // ESP32 users, use setPins(sda, scl) if customised, *before* passing Wire to the library (the line below).  
  I2CMux.begin(Wire);             // Wire instance is passed to the library

  I2CMux.closeAll();              // Set a base state which we know (also the default state on power on)
  

  I2CMux.openChannel(0);
    Serial.print("Initializing pulse oximeter..");

    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    // The default current for the IR LED is 50mA and it could be changed
    //   by uncommenting the following line. Check MAX30100_Registers.h for all the
    //   available options.
     //pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
     //MAX30100_LED_CURR_24MA  
     pox.setIRLedCurrent(MAX30100_LED_CURR_24MA);

    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);

    I2CMux.closeChannel(0);
    I2CMux.closeAll();
}

void loop()
{
    I2CMux.openChannel(0);
    // Make sure to call update as fast as possible
    pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");

        tsLastReport = millis();
    }
    I2CMux.closeChannel(0);
    I2CMux.closeAll();
}
