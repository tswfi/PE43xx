#include "ArduinoPE43xx.h"

ArduinoPE43xx::ArduinoPE43xx(uint8_t pin_le, uint8_t pin_clock, uint8_t pin_data, uint8_t type) {
    _pin_le = pin_le;
    _pin_clock = pin_clock;
    _pin_data = pin_data;
    _type = type;
}

void ArduinoPE43xx::begin() {
    // set pins
    pinMode(_pin_le, OUTPUT);
    pinMode(_pin_clock, OUTPUT);
    pinMode(_pin_data, OUTPUT);

    // set our internal max and step values
    switch (_type) {
        case PE4302:
        case PE4312:
            _max = 31.5;
            _step = 0.5;
            break;
        case PE4306:
            _max = 30;
            _step = 1;
            break;
        default:
            #warning type not certain, going with defaults, step=1 max=30
            _step = 1;
            _max = 30;
            break;
    }

    // set level to 0
    setLevel(0);
}

bool ArduinoPE43xx::setLevel(float level) {
    // bounds check
    if(level < 0 || level > getMax() ) {
        return false;
    }

    _level = level;
    _writeLevel();

    return true;
}

void ArduinoPE43xx::_writeLevel() {

    // PE4302 and PE4312 wants something like this
    //
    // C16  C8  C4  C2  C1  C0.5  =>  result
    //  0    0   0   0   0    0   =>  0db
    //  0    0   0   0   0    1   =>  0.5db
    //  0    0   1   1   0    1   =>  6.5db
    //  1    1   1   1   1    1   => 31.5db

    // for PE4306 (which has only 1db resolution)
    //
    // C16  C8  C4  C2  C1  =>  result
    //  0    0   0   0   0  =>  0db
    //  0    0   0   0   1  =>  1db
    //  0    1   0   0   1  =>  9db
    //  1    1   1   1   1  => 31db

    // so the big different is that one can set the .5 bit or not

    // get the integer part, it will be the same for both chips
    int intlevel = int(_level);
    // default to 5 bits
    int bits = 5;

    // if our step is less than 1 the chip supports the 0.5 decimal
    if(_step < 1) {
        // one more bit
        bits = 6;

        // if the decimal is not 0 shift 1 into our level
        if( (_level*10 - intlevel*10) > 0) {
            intlevel = intlevel << 1;
            bitWrite(intlevel, 0, 1);
        } else {
            intlevel = intlevel << 1;
            bitWrite(intlevel, 0, 0);
        }
    }

    // LE and CLOCK down to get the chip listen
    digitalWrite(_pin_le, LOW);
    digitalWrite(_pin_clock,LOW);

    // Write the level out MSB first
    int b;
    for (int bit = bits; bit >= 0; bit--) {
        b = ((intlevel << 1) >> bit) & 0x01;

        digitalWrite(_pin_data, b);
        digitalWrite(_pin_clock, HIGH);
        digitalWrite(_pin_clock, LOW);
    }

    // toggle LE to latch
    digitalWrite(_pin_le, HIGH); // Toggle LE high to enable latch
    digitalWrite(_pin_le, LOW);  // and then low again to hold it.
}

// getters
float ArduinoPE43xx::getLevel() {
    return _level;
}

float ArduinoPE43xx::getMax() {
    return _max;
}

float ArduinoPE43xx::getStep() {
    return _step;
}