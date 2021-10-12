void onBeatDetected()
{
    Serial.println("Beat!");
    notify();
}

void setupHB(){
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
}
