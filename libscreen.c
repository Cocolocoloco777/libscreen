/**
  * @brief It implements the screen interface
  *
  * @file libscreen.c
  * @author Plablos
  * @version 2.0
  * @date 20-04-2023
  * @copyright GNU Public License
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libscreen.h"

#define MAX_AREAS 10

#define BACKGROUND(r,g,b) "\x1B[48;2;" #r ";" #g ";" #b "m"
#define RESET "\x1B[0m"

#define BLANK "                                                                                                    "

void area_destroy(Area *area);

typedef struct {
  Area *area[MAX_AREAS];
  int n_areas;
  int rows;
  int columns;
                        
} Screen; 

struct _Area {
  char **character_array;
  int x;
  int y;  
  int width;
  int height;
  int cursor;                      
};

Screen screen;

void screen_init(int rows, int columns){

  /* Error control */
  if ( rows <= 0 || columns <= 0){
    return;
  }

  screen.columns = columns;
  screen.rows = rows;
  screen.n_areas = 0;
}

void screen_destroy(){
  int i;
  for (i = 0; i < screen.n_areas; i++){
    area_destroy(screen.area[i]);
  }
}

void area_destroy(Area *area){

  if (area == NULL){
    return;
  }

  free(area->character_array[0]);
  free(area->character_array);
  free(area);
}

Area *screen_area_init(int x, int y, int width, int height){
  char *characters;
  Area *area = NULL;
  int i;  

  area = malloc(sizeof(Area));
  if (area == NULL){
    return NULL;
  }

  characters = (char*) malloc(4* height *(width + 1)*sizeof(char));
  if (characters == NULL){
    free(area);
    return NULL;
  }

  area->character_array = (char**) malloc(height*sizeof(char*));
  if (area->character_array == NULL){
    free(characters);
    return NULL;
  }

  for(i = 0; i < height; i++){
    area->character_array[i] = characters + (width + 1)*i;
    area->character_array[i][0] = '\0';
  }

  area->x = x;
  area->y = y;
  area->width = width;
  area->height = height;
  area->cursor = 0;

  screen.area[screen.n_areas] = area;
  screen.n_areas++;

  return area;
}

int wbstrlen(char *str){
  int i, count = 0;
  for (i = 0; str[i] != '\0'; i++){
    if ((str[i] & 0x80) == 0){
      count ++;
    } else if ((str[i] & 0xE0) == 0xC0){
      count ++;
      i++;
    } else if ((str[i] & 0xF0) == 0xE0){
      count ++;
      i+= 2;
    } else {
      count ++;
      i+= 3;
    }
  }
  return count;
}

char* wbstrmove(char *str, int x){
  int i, count = 0;

  for (i = 0; str[i] != '\0' && count < x; i++){
    if ((str[i] & 0x80) == 0){
      count ++;
    } else if ((str[i] & 0xE0) == 0xC0){
      count ++;
      i++;
    } else if ((str[i] & 0xF0) == 0xE0){
      count ++;
      i+= 2;
    } else {
      count ++;
      i+= 3;
    }
  }
}

void screen_paint(){
  int i, j, i_area, n_char;

  for (i = 0; i < screen.rows; i++){
    for (j = 0; j < screen.columns; j++){
      for (i_area = 0; i_area < screen.n_areas; ++i_area){
        /* Comprobar que hay un area*/
        if (screen.area[i_area]->x == j && i >= screen.area[i_area]->y && i < screen.area[i_area]->y + screen.area[i_area]->height){
          printf(BACKGROUND(253,253,252));
          n_char = wbstrlen(screen.area[i_area]->character_array[i - screen.area[i_area]->y]);
          printf("%s%.*s", screen.area[i_area]->character_array[i - screen.area[i_area]->y],screen.area[i_area]->width - n_char, BLANK);
          printf(RESET);
          j = j + screen.area[i_area]->width - 1;
          break;
        }
      }
      /* Caso no area*/
      if (i_area == screen.n_areas){
        printf(BACKGROUND(28,152,243));
        printf(" ");
        printf(RESET);
      }
    }

    printf("\n");
  }
}

void screen_area_puts(Area *area, char *str){

 
  strcpy(area->character_array[area->cursor], str);
  area->cursor++;
}

int main(){

  Area *a;

  screen_init(10, 10);

  a = screen_area_init(3,3,3,3);

  screen_area_puts(a, "👃");

  screen_paint();

  return 0;
}