#ifndef _S4625PROTO_HPP_
#define _S4625PROTO_HPP_

#include <cstdint>

enum class Rotor {
  M1,
  M2,
  M3
};

enum class Way {
  UP,
  DOWN,
  STOP
};

constexpr uint16_t tickInus = 2 * 10; // 50KHz -> 20us (2 ?) , all value divided by 40

uint16_t* s4624Proto(Rotor r, Way w);

#endif
