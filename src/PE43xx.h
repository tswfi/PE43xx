#ifndef PE43xx_h
#define PE43xx_h

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

// Define types of sensors.
#define PE4302 02
// untested as I don't own these
#define PE4306 06
#define PE4312 12

class PE43xx
{
  public:
    PE43xx(uint8_t pin_le, uint8_t pin_clock, uint8_t pin_data, uint8_t type);

    void begin();

    // set attenuatator level. Range from 0 to 31
    // returns false if level was out of range or setting failed
    bool setLevel(float level);

    // get the current attenuator level
    float getLevel();

    // maximum level of attenuation. This varies between the chips
    // usually 31db or 31.5db
    float getMax();

    // min step. Varies between chips. Usually 1db or 0.5db
    float getStep();

  private:
    // type of the chip
    uint8_t _type;
    // pins used
    uint8_t _pin_le, _pin_clock, _pin_data;

    // current attenuator level value
    float _level;

    // step and max value
    float _step;
    float _max;

    // write the level out to the chip
    void _writeLevel();
};

#endif