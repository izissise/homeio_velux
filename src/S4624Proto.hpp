#ifndef _S4625PROTO_HPP_
#define _S4625PROTO_HPP_

#include <cstdint>

enum class Rotor : uint8_t {
  M1,
  M2,
  M3
};

enum class Way : uint8_t {
  UP,
  DOWN,
  STOP
};

enum class Signal : bool {
  SHORT, //SHORT 0.44ms 220 ticks
  LONG //LONG  1.25ms 630 ticks
};

constexpr uint16_t tickDivider = 10;
constexpr uint16_t tickInus = 2 * tickDivider; // 500KHz -> 2us, all value divided by 10
constexpr uint16_t nbTickLong = (630 / tickDivider) - 11; // 5 Code time run
constexpr uint16_t nbTickShort = (220 / tickDivider) - 4; // 4 Code time run

const uint16_t* s4624Proto(Rotor r, Way w);

#endif
