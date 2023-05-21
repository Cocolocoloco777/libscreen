/**
  * @brief It implements the screen interface
  *
  * @file libscreen.c
  * @author Pablo Fernández y Pablo Pérez
  * @version 2.0
  * @date 20-04-2023
  * @copyright GNU Public License
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libscreen.h"

#define MAX_AREAS 10  /*!< The maximum number of areas */

#define BLANK "                                                                                                                                                                                                        "
#define TERMINAL_RESET "\x1b[0m"  /*!< Code to reset the color */

/* Private functions */

/**
  * @brief It prints the libscreen information at the beggining
  * @author Pablo Fernández y Pablo Pérez
  */
void print_authors_start();

/**
  * @brief It prints the libscreen information at the end
  * @author Pablo Fernández y Pablo Pérez
  */
void print_authors_end();

/**
 * @brief Screen structure
 *
 * This struct stores all the information of an screen.
 */
typedef struct {
  Area *area[MAX_AREAS];    /*!< An array of areas */
  int n_areas;              /*!< The number of areas in the array */
  int rows;                 /*!< Number of rows in the screen */
  int columns;              /*!< Number of columns in the screen */
  Color area_foreground;    /*!< Foreground color of the areas */
  Color area_background;    /*!< Background color of the areas */
  Color screen_background;  /*!< Background color of the screen */
                        
} Screen; 

/**
 * @brief Area structure
 *
 * This struct stores all the information of an area.
 */
struct _Area {
  char **string_array;  /*!< An array of strings */
  int *string_len;      /*!< It stores the length of the strings */
  int x;                /*!< x coordinate of the position in the screen */
  int y;                /*!< y coordinate of the position in the screen */
  int width;            /*!< The width of the area */
  int height;           /*!< The height of the area */
  int cursor;           /*!< Position of the cursor */
  int screen_id;        /*!< The position of the area in the screen */            
};

Screen screen;

void print_authors_start(){

  printf("\nLibrería gráfica libscreen con soporte para caracteres UTF-8\n");
  printf("Autores: Pablo Fernández Izquierdo y Pablo Pérez Hernández     Universidad Autónoma de Madrid\n");
  printf("Iniciando libscreen...\n\n\n");
}

void print_authors_end(){

  printf("\nGracias por usar libscreen\n\n");
  printf("Librería gráfica libscreen con soporte para caracteres UTF-8\n");
  printf("Autores: Pablo Fernández Izquierdo y Pablo Pérez Hernández     Universidad Autónoma de Madrid\n\n\n");
}

void screen_init(int rows, int columns, Color area_foreground, Color area_background, Color screen_background){

  /* Error control */
  if ( rows <= 0 || columns <= 0){
    fprintf(stderr, "SCREEN ERROR Error in screen init: invalid parameters\n");
    return;
  }

  /* Initialises screen parameters */
  screen.columns = columns;
  screen.rows = rows;
  screen.n_areas = 0;

  /* Initialises screen colors */
  screen.area_background = area_background;
  screen.area_foreground = area_foreground;
  screen.screen_background = screen_background;

  print_authors_start();
}

void screen_destroy(){
  print_authors_end();
}

Area *screen_area_init(int x, int y, int width, int height){
  Area *area = NULL;
  int i, j;  

  /* Error control */
  if (x < 0 || y < 0 || width <= 0 || height <= 0){
    fprintf(stderr, "SCREEN ERROR Error in screen area init: invalid parameters\n");
    return NULL;
  }

  /* Check if there is space available */
  if (screen.n_areas == MAX_AREAS){
    fprintf(stderr, "SCREEN ERROR Error in screen area init: maximum number of areas reached\n");
    return NULL;
  }

  if (x + width > screen.columns || y + height > screen.rows){
    fprintf(stderr, "SCREEN ERROR Error in screen area init: area too large\n");
    return NULL;
  }

  /* Allocates memory */
  area = malloc(sizeof(Area));
  if (area == NULL){
    fprintf(stderr, "SCREEN ERROR Error in screen area init: unable to allocate memory\n");
    return NULL;
  }

  /* Allocates memory for the array of lengths */
  area->string_len = (int*) malloc(height*sizeof(int));
  if (area->string_len == NULL){
    fprintf(stderr, "SCREEN ERROR Error in screen area init: unable to allocate memory\n");
    free(area);
    return NULL;
  }
  
  /* Initializes the array of lengths */
  for (i = 0; i < height; i++){
    area->string_len[i] = width + 1;
  }

  /* Allocates memory for the string array */
  area->string_array = (char**) malloc(height*sizeof(char*));
  if (area->string_array == NULL){
    fprintf(stderr, "SCREEN ERROR Error in screen area init: unable to allocate memory\n");
    free(area);
    free(area->string_len);
    return NULL;
  }

  /* Allocates memory for the strings */
  for (i = 0; i < height; i++){

    area->string_array[i] = malloc((width + 1)*sizeof(char));

    /* Error control */
    if (area->string_array[i] == NULL){
      for (j = 0; j < i; j++){
        free(area->string_array[j]);
      }

      free(area);
      free(area->string_len);
      free(area->string_array);
    }

    /* Initialises the strings */
    area->string_array[i][0] = '\0';
  }

  /* Initialises parameters */
  area->x = x;
  area->y = y;
  area->width = width;
  area->height = height;
  area->cursor = 0;

  /* Adds the area to the screen */
  screen.area[screen.n_areas] = area;
  area->screen_id = screen.n_areas;
  screen.n_areas++;

  return area;
}

void screen_area_destroy(Area *area){
  int i;

  /* Error control */
  if (area == NULL){
    fprintf(stderr, "SCREEN ERROR Error in screen destroy: invalid parameters\n");
    return;
  }

  /* Deletes from the screen */
  if (screen.area[area->screen_id] == area){
    screen.area[area->screen_id] = screen.area[screen.n_areas - 1];
    screen.area[area->screen_id]->screen_id = area->screen_id;
    screen.n_areas--;
  } else {
    fprintf(stderr, "SCREEN ERROR error in screen area destroy: area not in screen\n");
    return;
  }
  

  /* Frees the strings */
  for (i = 0; i < area->height; i++){
    if (area->string_array[i] != NULL){
      free(area->string_array[i]);
    }
  }

  /* Frees the string array and its length */
  free(area->string_array);
  free(area->string_len);

  /* Frees the area */
  free(area);
}

void screen_set_area_foreground_color(Color color){
  screen.area_foreground = color;
}

void screen_set_area_background_color(Color color){
  screen.area_background = color;
}

void screen_set_screen_background_color(Color color){
  screen.screen_background = color;
}

int screen_multibyte_strlen(char *str){
  int i, count = 0, color_code = -1;

  /* Error control */
  if (str == NULL){
    return -1;
  }

  /* Iterates the string */
  for (i = 0; str[i] != '\0'; i++){

    /* Color code case (it doesnt count it)*/
    if (str[i] == '\x1B' && str[i + 1] == '['){
      color_code = i;
      continue;
    } else if (color_code != -1){
      if (str[i] == 'm'){
        color_code = -1;
      }
      continue;
    }

    if ((str[i] & 0x80) == 0){ /* One byte case */
      count ++;
    } else if ((str[i] & 0xE0) == 0xC0){ /* Two byte case */
      count ++;
      i++;
    } else if ((str[i] & 0xF0) == 0xE0){ /* Three byte case */
      count ++;
      i+= 2;
    } else { /* Four byte case */
      count ++;
      i+= 3;
    }
  }
  return count;
}

int screen_multibyte_move(char *str, int x){
  int i, count = 0, color_code = -1;

  /* Error control */
  if (str == NULL || x < 0){
    return -1;
  }

  /* Iterates the string */
  for (i = 0; str[i] != '\0' && count < x; i++){

    /* Color code case */
    if (str[i] == '\x1B'){
      color_code = i;
      continue;
    } else if (color_code != -1){
      if (str[i] == 'm'){
        color_code = -1;
      }
      continue;
    }

    if ((str[i] & 0x80) == 0){ /* One byte case */
      count ++;
    } else if ((str[i] & 0xE0) == 0xC0){ /* Two byte case */
      count ++;
      i++;
    } else if ((str[i] & 0xF0) == 0xE0){ /* Three byte case */
      count ++;
      i+= 2;
    } else { /* Four byte case */
      count ++;
      i+= 3;
    }
  }

  /* Color codes at the end */
  while (str[i] == '\x1B'){
    while (str[i] != '\0' && str[i] != 'm'){
      i++;
    }
    i++;    
  }

  return i;
}

int screen_sprint_background_color_code(char *str, Color color){

  /* Error control */
  if (str == NULL){
    return -1;
  }

  /* Sprints the color code */
  return sprintf(str, "\x1B[48;2;%d;%d;%dm", screen.area_background.r, screen.area_background.g, screen.area_background.b);
}

int screen_sprint_foreground_color_code(char *str, Color color){

  /* Error control */
  if (str == NULL){
    return -1;
  }

  /* Sprints the color code */
  return sprintf(str, "\x1B[38;2;%d;%d;%dm", screen.area_background.r, screen.area_background.g, screen.area_background.b);
}

void screen_paint(){
  int i, j, i_area, n_char;

  /* Scan each row and column */
  for (i = 0; i < screen.rows; i++){
    for (j = 0; j < screen.columns; j++){
      for (i_area = 0; i_area < screen.n_areas; ++i_area){
        
        /* Check if there is an area starting at this column */
        if (screen.area[i_area]->x == j && i >= screen.area[i_area]->y && i < screen.area[i_area]->y + screen.area[i_area]->height){

          /* Changes terminal color to area color */
          printf("\x1B[48;2;%d;%d;%dm", screen.area_background.r, screen.area_background.g, screen.area_background.b);
          printf("\x1B[38;2;%d;%d;%dm", screen.area_foreground.r, screen.area_foreground.g, screen.area_foreground.b);

          /* Gets the length (in characters) of the string to paint*/
          n_char = screen_multibyte_strlen(screen.area[i_area]->string_array[i - screen.area[i_area]->y]);

          /* Paints the string and fills the whats left with spaces */
          printf("%.*s%.*s" TERMINAL_RESET, (int) strlen(screen.area[i_area]->string_array[i - screen.area[i_area]->y]), screen.area[i_area]->string_array[i - screen.area[i_area]->y],screen.area[i_area]->width - n_char, BLANK);

          /* Jumps to the end of the area line */
          j = j + screen.area[i_area]->width - 1;
          break;
        }
      }
      
      /* No areas */
      if (i_area == screen.n_areas){
        printf("\x1B[48;2;%d;%d;%dm" " ", screen.screen_background.r, screen.screen_background.g, screen.screen_background.b);
      }
    }

    /* Resets the color of the terminal */
    printf(TERMINAL_RESET "\n");
  }
}

void screen_area_puts(Area *area, char *str){
  int n_char, i, n_lines, j, move;
  char *text, *realloc_string;

  /* Error control */
  if (area == NULL || str == NULL){
    fprintf(stderr, "SCREEN ERROR Error in screen area puts: invalid area or string\n");
    return;
  }
    
  /* Gets the len in characters */
  n_char = screen_multibyte_strlen(str);

  /* Calculates the number of lines to be written */
  n_lines = (n_char / area->width) + ((n_char % area->width) != 0);

  /* Writes the string in the area string/s*/
  for (i = 0; i < n_lines; i++){

    /* If the area is full of strings moves all area strings up one */
    if (area->cursor == area->height){
      text = area->string_array[0];
      for (j = 0; j < area->height - 1; j++){
        area->string_array[j] = area->string_array[j + 1];
      }
      area->string_array[area->height - 1] = text;
      area->cursor--;
    }

    /* Gets the size (in bytes) of the string*/
    move = screen_multibyte_move(str, area->width);

    /* It reallocs the string if it doesnt fit */
    if (move >= area->string_len[area->cursor]){
      realloc_string = realloc(area->string_array[area->cursor], move + 1);
      if (realloc_string == NULL){
        fprintf(stderr, "SCREEN ERROR Error in screen area puts: unable to allocate memory\n");
        return;
      }
      area->string_array[area->cursor] = realloc_string;
      area->string_len[area->cursor] = move + 1;
    }
    
    /* Writes the string */
    sprintf(area->string_array[area->cursor], "%.*s", move, str);

    /* Moves cursor to next line */
    str = str + move;
    area->cursor++;
  }
}

void screen_area_clear(Area *area){
  int i;

  /* Error control */
  if (area == NULL){
    fprintf(stderr, "SCREEN ERROR Error in screen area clear: invalid area\n");
    return;
  }
  
  /* Resets the strings */
  for (i = 0; i < area->height; i++){
    area->string_array[i][0] = '\0';
  }

  /* Resets the cursor */
  area->cursor = 0;
}

void screen_area_set_cursor(Area *area, int new_cursor){

  /* Error control */
  if (area == NULL){
    fprintf(stderr, "SCREEN ERROR Error in screen area set cursor: invalid area\n");
    return;
  }

  if (new_cursor < 0 || new_cursor >= area->height){
    fprintf(stderr, "SCREEN ERROR Error in screen area clear: invalid cursor\n");
    return;
  }

  area->cursor = new_cursor;
}
