#include "S4624Proto.hpp"

constexpr Signal L = Signal::LONG;
constexpr Signal S = Signal::SHORT;

constexpr std::size_t cmdSize = 23;

static constexpr Signal stop[cmdSize] =      {L,S,S,L,L,S,S,L,S,L,L,S,L,S,S,L,S,L,S,L,S,L,S};
static constexpr Signal oneDown[cmdSize] =   {L,S,S,L,L,S,S,L,L,S,S,L,S,L,L,S,S,L,S,L,S,L,S};
static constexpr Signal oneUp[cmdSize] =     {L,S,S,L,L,S,S,L,L,S,S,L,S,L,S,L,S,L,S,L,L,S,S};
static constexpr Signal twoDown[cmdSize] =   {L,S,S,L,L,S,S,L,L,S,S,L,S,L,S,L,L,S,S,L,S,L,S};
static constexpr Signal twoUp[cmdSize] =     {L,S,S,L,L,S,S,L,L,S,S,L,S,L,S,L,S,L,L,S,S,L,S};
static constexpr Signal threeDown[cmdSize] = {L,S,S,L,L,S,S,L,L,S,S,L,L,S,S,L,S,L,S,L,S,L,S};
static constexpr Signal threeUp[cmdSize] =   {L,S,S,L,L,S,S,L,L,S,S,L,S,L,S,L,S,L,S,L,S,L,L};

static uint16_t actionBuffer[32] = {0};

void actionToBuffer(const Signal* actions) {
  unsigned int i = 0;
  int timing = 100;
  actionBuffer[0] = timing;
  for (i = 0; i < cmdSize; ++i) {
    uint16_t value = (actions[i] == L) ? nbTickLong : nbTickShort;
    timing += value;
    actionBuffer[i + 1] = timing;
  }
  actionBuffer[i + 1] = 0;
}

const uint16_t* s4624Proto(Rotor r, Way w) {
  switch (r) {
    case Rotor::M1:
      actionToBuffer((w == Way::UP) ? oneUp : oneDown);
    break;
    case Rotor::M2:
      actionToBuffer((w == Way::UP) ? twoUp : twoDown);
    break;
    case Rotor::M3:
      actionToBuffer((w == Way::UP) ? threeUp : threeDown);
    break;
    default:
      actionToBuffer(stop);
    break;
  }

  return actionBuffer;
}
