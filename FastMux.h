#pragma once
#include "config.h"

constexpr uint16_t MUX_CMOS_DELAY_US = 10;

class FastMux
{
 public:
  FastMux(byte a0, byte a1, byte a2) :_a0(a0), _a1(a1), _a2(a2)
  {
    _numBits = 3;
  }

  FastMux(byte a0, byte a1, byte a2, byte a3) : _a0(a0), _a1(a1), _a2(a2), _a3(a3)
  {
    _numBits = 4;
  }
  
  
  /// clear all address bits to start with a known previous state
  void clear()
  {
    digitalWriteFast(_a0, 0);
    digitalWriteFast(_a1, 0);
    digitalWriteFast(_a2, 0);
    if(_numBits>3) digitalWriteFast(_a3, 0);
    _previousAddr = 0;
    delayMicroseconds(MUX_CMOS_DELAY_US);
  }
  
  /// Optimized addr selection, cached previous addr will allow to write an addr bit only if it is changed
  void select(byte addr)
  {
    byte a = addr;
    byte b = _previousAddr;
    bool dirtyFlag = false;
    for (byte n=0; n< _numBits; n++)
    {
      byte val = a & 0x01 ;
      if ( val != (b & 0x01) ) {
        digitalWriteFast(_addr[n], val);
        dirtyFlag = true;
      }
      a >>= 1; // shift right
      b >>= 1; // shift right
    }
    if(dirtyFlag) {
      _previousAddr = addr;
      delayMicroseconds(MUX_CMOS_DELAY_US);
    }
  }
  
  byte Count()  const { return 1 << (_numBits); }  
 
  void begin()
  {
    pinMode(_a0, OUTPUT);
    pinMode(_a1, OUTPUT);
    pinMode(_a2, OUTPUT);
    if(_numBits > 3) pinMode(_a3, OUTPUT);
    clear();
  }

private:

  byte _a0, _a1, _a2, _a3;
  byte _numBits;
  byte _previousAddr; 
  const byte _addr[4] = {_a0, _a1, _a2, _a3};
};
