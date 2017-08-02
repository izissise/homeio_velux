#include "S4624Proto.hpp"

// 1-up.csv:   |1||1||1| 0| 0| 0| 0||1
// 1-down.csv: |1||1||1| 0||1| 0| 0| 0

// 2-up.csv:   |1||1||1| 0| 0| 0||1| 0
// 2-down.csv: |1||1||1| 0| 0||1| 0| 0

// 3-up.csv:   |1||1||1| 0| 0| 0| 0| 0
// 3-down.csv: |1||1||1||1| 0| 0| 0| 0

// 1-mid.csv:  |1||1| 0||0| 1| 0| 0| 0
// 2-mid.csv:  |1||1| 0||0| 1| 0| 0| 0
// 3-mid.csv:  |1||1| 0||0| 1| 0| 0| 0

uint8_t s4624Proto(Rotor r, Way w) {
  uint8_t command;
  if (w == Way::STOP) {
    command = 0b11001000;
  } else {
    command = 0b11100000;
    switch (r) {
      case Rotor::M1:
        command |= (w == Way::UP) ? 0b00000001 : 0b00001000;
      break;
      case Rotor::M2:
        command |= (w == Way::UP) ? 0b00000010 : 0b00000100;
      break;
      case Rotor::M3:
        command |= (w == Way::UP) ? 0b00000000 : 0b00010000;
      break;
    }
  }
  return command;
}
