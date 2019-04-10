# PE43xx Library

Simple library for PE43xx Digital Step Attenuator family of chips. Should work with all PE43xx family but I don't have those for testing. Currently only tested with PE4302.

https://www.psemi.com/pdf/datasheets/pe4302ds.pdf
https://www.psemi.com/pdf/datasheets/pe4312ds.pdf

https://www.psemi.com/pdf/datasheets/pe4306ds.pdf

# Usage

```c++
<include PE43xx.h>

// Initialize our attenuator, pins are LE, DATA, CLOCK, Type. Currently only PE4302 is tested
PE43xx attenuator(A4,A5,A6,PE4302)

// just for the demo time keeping
unsigned long previousMillis = 0;
unsigned long interval = 500;

void setup() {
    // initialize the attenuator, sets level to 0
    attenuator.begin();
}

void loop() {
    // drop the level every 500ms and then start over
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {

        // different attenuator have different steps (usually either 1db or 0.5db)
        // get the step from libabry (for now only supports PE4302 but this might change in the future)
        level = level + attenuator.getStep();

        // different chips have different max values (usually 31db or 31.5db)
        // get the max from the library (for now only supports PE4302 but this might change in the future)
        if(level > attenuator.getMax() ) {
            level = 0;
        }

        // set the level
        attenuator.setLevel(level)
    }

}
```