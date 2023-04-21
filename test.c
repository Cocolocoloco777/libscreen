#include "libscreen.h"


int main(){

  Area *a, *b;

  screen_init(10, 20);

  a = screen_area_init(3,3,3,3);
  b = screen_area_init(7,3,3,3);

  screen_area_puts(a, "a");
  screen_area_puts(a, "o");
  screen_area_puts(b, "bbbbb");

  screen_paint();

  screen_area_clear(a);

  screen_area_puts(a, "ooa");

  screen_paint();

  screen_area_destroy(a);

  screen_destroy();

  return 0;
}
