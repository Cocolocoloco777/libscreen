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
#define LIB_SIZE 8

#define BLANK "                                                                                                    "


void print_authors_start(){

  printf("\nLibrería gráfica libscreen con soporte para caracteres UTF-8\n");
  printf("Autores: Pablo Fernández Izquierdo y Pablo Pérez Hernández     Universidad Autónoma de Madrid\n");
  printf("Licencia: GNU General Public License v3.0\n\n\n");
  printf("Iniciando libscreen...\n\n\n");
}

void print_authors_end(){

  printf("\nGracias por usar libscreen\n\n");
  printf("Librería gráfica libscreen con soporte para caracteres UTF-8\n");
  printf("Autores: Pablo Fernández Izquierdo y Pablo Pérez Hernández     Universidad Autónoma de Madrid\n");
  printf("Licencia: GNU General Public License v3.0\n\n");
}

void area_destroy(Area *area);

typedef struct {
  Area *area[MAX_AREAS];
  int n_areas;
  int rows;
  int columns;
                        
} Screen; 

struct _Area {
  char **character_array;
  char *free_character;
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

  print_authors_start();
}

void screen_destroy(){
  print_authors_end();
  return;
}

void screen_area_destroy(Area *area){

  if (area == NULL){
    return;
  }

  /* Modificar */

  free(area->free_character);
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

  characters = (char*) malloc(LIB_SIZE * height *(width + 1)*sizeof(char));
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
    area->character_array[i] = characters + LIB_SIZE * (width + 1) * i;
    area->character_array[i][0] = '\0';
  }

  area->x = x;
  area->y = y;
  area->width = width;
  area->height = height;
  area->cursor = 0;
  area->free_character = characters;

  screen.area[screen.n_areas] = area;
  screen.n_areas++;

  return area;
}

int screen_multibyte_strlen(char *str){
  int i, count = 0, color_scape_sequence = -1;
  for (i = 0; str[i] != '\0'; i++){
    if (str[i] == '\x1B' && str[i + 1] == '['){
      color_scape_sequence = i;
      continue;
    } else if (color_scape_sequence != -1){
      if (str[i] == 'm'){
        color_scape_sequence = -1;
      }
      continue;
    }

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

int screen_multibyte_move(char *str, int x){
  int i, count = 0, color_scape_sequence = -1;

  for (i = 0; str[i] != '\0' && count < x; i++){
    if (str[i] == '\x1B' && str[i + 1] == '['){
      color_scape_sequence = i;
      continue;
    } else if (color_scape_sequence != -1){
      if (str[i] == 'm'){
        color_scape_sequence = -1;
      }
      continue;
    }
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

  return i;
}

void screen_paint(){
  int i, j, i_area, n_char;

  for (i = 0; i < screen.rows; i++){
    for (j = 0; j < screen.columns; j++){
      for (i_area = 0; i_area < screen.n_areas; ++i_area){
        /* Comprobar que hay un area*/
        if (screen.area[i_area]->x == j && i >= screen.area[i_area]->y && i < screen.area[i_area]->y + screen.area[i_area]->height){
          printf(FOREGROUND(0,0,0) BACKGROUND(253,253,252));
          n_char = screen_multibyte_strlen(screen.area[i_area]->character_array[i - screen.area[i_area]->y]);
          printf(FOREGROUND(0,0,0) "%.*s%.*s" RESET, (int) strlen(screen.area[i_area]->character_array[i - screen.area[i_area]->y]), screen.area[i_area]->character_array[i - screen.area[i_area]->y],screen.area[i_area]->width - n_char, BLANK);
          j = j + screen.area[i_area]->width - 1;
          break;
        }
      }
      /* Caso no area      azul : 28,152,243*/
      if (i_area == screen.n_areas){
        printf(BACKGROUND(250,164,189));
        printf(" ");
        printf(RESET);
      }
    }

    printf("\n");
  }
}

void screen_area_puts(Area *area, char *str){
  int n_char, i, n_lines, j, move;
  char *text;

  if (area == NULL || str == NULL)
    return;

  n_char = screen_multibyte_strlen(str);

  n_lines = (n_char / area->width) + ((n_char % area->width) != 0);

  for (i = 0; i < n_lines; i++){
    if (area->cursor == area->height){
      text = area->character_array[0];
      for (j = 0; j < area->height - 1; j++){
        area->character_array[j] = area->character_array[j + 1];
      }
      area->character_array[area->height - 1] = text;
      area->cursor--;
    }
    move = screen_multibyte_move(str, area->width);
    snprintf(area->character_array[area->cursor], move + 1, "%s", str);
    str = str + move;
    area->cursor++;
  }
}

void screen_area_clear(Area *area){
  int i;

  if (area == NULL)
    return;

  for (i = 0; i < area->height; i++){
    area->character_array[i][0] = '\0';
  }

  area->cursor = 0;
}

void screen_area_reset_cursor(Area *area){
  area->cursor = 0;
}
