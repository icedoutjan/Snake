#ifndef snake_h
#define sanke_h
#include "Arduino.h"
class snake{
  public:
  int len = 3;
  int direction = 3; // Anfangsrichtung: 0 = links, 1 = rechts, 2 = oben, 3 = unten
  int body_x[32];
  int body_y[32];
  void move();
  bool alife();
  void updateDirection();
};
#endif
