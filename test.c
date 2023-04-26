#include "libscreen.h"
#include <string.h>

#define BACKGROUND(r,g,b) "\x1B[48;2;" #r ";" #g ";" #b "m"
#define FOREGROUND(r,g,b) "\x1B[38;2;" #r ";" #g ";" #b "m"


int main(){

  Area *b;

  screen_init(10, 20);

  b = screen_area_init(3,3,6,6);

  screen_area_puts(b, "bb"BACKGROUND(0,0,255) FOREGROUND(0,255,0)"bbb" BACKGROUND(255,255,255) "a");

  screen_paint();

  screen_area_destroy(b);

  screen_destroy();

  return 0;
}
