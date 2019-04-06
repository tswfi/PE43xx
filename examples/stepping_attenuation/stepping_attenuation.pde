// stepping_attenuation
// by Tatu Wikman <https://github.com/tswfi>

// Simple example that steps attenuation by 0.5db every 500ms

#include <ArduinoPE43xx.h>

// Initialize our attenuator, pins are LE, DATA, CLOCK, Type. Currently only PE4302 is tested
ArduinoPE43xx attenuator(A4,A5,A6,PE4302);

// for calculations in the demo
float level = 0;

// just for the demo time keeping
unsigned long previousMillis = 0;
unsigned long interval = 2500;

void setup() {
    // initialize the attenuator, sets level to 0
    attenuator.begin();
}

void loop() {
    // drop the level every interval and start over
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // different attenuator have different steps (usually either 1db or 0.5db)
        // get the step from libabry (for now only supports PE4302 but this might change in the future)
        level = level + attenuator.getStep();

        // different chips have different max values (usually 31db or 31.5db)
        // get the max from the library (for now only supports PE4302 but this might change in the future)
        if(level > attenuator.getMax() ) {
            level = 0;
        }

        // set the level
        attenuator.setLevel(level);
    }

}