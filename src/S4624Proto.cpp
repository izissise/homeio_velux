#include "S4624Proto.hpp"

constexpr Signal L = Signal::LONG;
constexpr Signal S = Signal::SHORT;
constexpr Signal STOP = Signal::STOP;

static constexpr Signal stop[] =      {L,S,S,L,L,S,S,L,S,L,L,S,L,S,S,L,S,L,S,L,S,L,S,STOP};
static constexpr Signal oneDown[] =   {L,S,S,L,L,S,S,L,L,S,S,L,S,L,L,S,S,L,S,L,S,L,S,STOP};
static constexpr Signal oneUp[] =     {L,S,S,L,L,S,S,L,L,S,S,L,S,L,S,L,S,L,S,L,L,S,S,STOP};
static constexpr Signal twoDown[] =   {L,S,S,L,L,S,S,L,L,S,S,L,S,L,S,L,L,S,S,L,S,L,S,STOP};
static constexpr Signal twoUp[] =     {L,S,S,L,L,S,S,L,L,S,S,L,S,L,S,L,S,L,L,S,S,L,S,STOP};
static constexpr Signal threeDown[] = {L,S,S,L,L,S,S,L,L,S,S,L,L,S,S,L,S,L,S,L,S,L,S,STOP};
static constexpr Signal threeUp[] =   {L,S,S,L,L,S,S,L,L,S,S,L,S,L,S,L,S,L,S,L,S,L,L,STOP};

static uint16_t actionBuffer[32] = {0};

void actionToBuffer(const Signal* actions) {
  int j = 0;
  int timing = 100;
  actionBuffer[j] = timing;
  j += 1;
  for (int i = 0; actions[i] != STOP; ++i) {
    uint16_t value = (actions[i] == L) ? nbTickLong : nbTickShort;
    timing += value;
    actionBuffer[j] = timing;
    j += 1;
  }
  actionBuffer[j] = 0;
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
