/**
  * @brief It defines the screen interface
  *
  * @file libscreen.h
  * @author Profesores PPROG
  * @version 2.0
  * @date 07-02-2021
  * @copyright GNU Public License
  */

#ifndef __LIBSCREEN_H_
#define __LIBSCREEN_H_

#define BACKGROUND(r,g,b) "\x1B[48;2;" #r ";" #g ";" #b "m"
#define FOREGROUND(r,g,b) "\x1B[38;2;" #r ";" #g ";" #b "m"
/*#define RESET FOREGROUND(0,0,0) BACKGROUND(253,253,252)*/

/**
 * @brief Color structure
 *
 * This struct stores all the information of an color.
 */
typedef struct {
  int r, g, b;    /*!< Color represented in RGB format */
                        
} Color;

/**
  * @brief Screen area for displaying information
  * @author Pablo Fernández y Pablo Pérez
  */
typedef struct _Area Area;

/**
  * @brief It creates a new screen
  * @author Pablo Fernández y Pablo Pérez
  *
  * This function should be called at the beginning of the program, 
  *  so the complete screen is allocated before starting defining areas.
  * @param rows the number of rows that will have the full screen
  * @param columns the number of columns that will have the full screen
  * @param area_foreground foreground color of the areas
  * @param area_background background color of the areas
  * @param screen_background background color of the screen
  */
void screen_init(int rows, int columns, Color area_foreground, Color area_background, Color screen_background);

/**
  * @brief It destroys a new screen area
  * @author Pablo Fernández y Pablo Pérez
  *
  * As it frees the screen, it must be called at the end of the program, 
  *  once the areas created have been freed.
  */
void screen_destroy();

/**
  * @brief It paints in the terminal the actual screen composition 
  * @author Pablo Fernández y Pablo Pérez
  *
  * This function should be called when some updates 
  *  in the screen want to be shown. 
  */
void screen_paint();

/**
  * @brief It creates a new area inside a screen
  * @author Pablo Fernández y Pablo Pérez
  *
  * screen_area_init allocates memory for a new area
  *  and initializes its members.
  * @param x the x-coordinate of the up-left corner of the area
  * @param y the x-coordinate of the up-left corner of the area
  * @param width the width of the area
  * @param height the height of the area
  * @return a new area, initialized
  */
Area *screen_area_init(int x, int y, int width, int height);

/**
  * @brief It destroys a new screen area
  * @author Pablo Fernández y Pablo Pérez
  *
  * This function should be called once the area is not needed anymore, 
  *  before ending the programm.
  * @param area the area to be freed
  */
void screen_area_destroy(Area *area);

/**
  * @brief It sets the foreground color of the areas
  * @author Pablo Fernández y Pablo Pérez
  *
  * @param color the color to set
  */
void screen_set_area_foreground_color(Color color);

/**
  * @brief It sets the background color of the areas
  * @author Pablo Fernández y Pablo Pérez
  *
  * @param color the color to set
  */
void screen_set_area_background_color(Color color);

/**
  * @brief It sets the background color of the screen
  * @author Pablo Fernández y Pablo Pérez
  *
  * @param color the color to set
  */
void screen_set_screen_background_color(Color color);
/**
  * @brief It cleares an area, eraising all its content
  * @author Pablo Fernández y Pablo Pérez
  *
  * This function should be called for earaising all the information in an area, 
  *  before introducing a new state of it.
  * @param area the area to be cleared
  */
void screen_area_clear(Area *area);

/**
  * @brief It sets the cursor of an area
  * @author Pablo Fernández y Pablo Pérez
  *
  * This function reset the cursor to the up-left corner of the area.
  * @param area the involved area
  * @param new_cursor an integer indicating the new cursor
  */
void screen_area_set_cursor(Area *area, int new_cursor);

/**
  * @brief It introduces some information inside an area
  * @author Pablo Fernández y Pablo Pérez
  *
  * This function sets the string that will be shown in an area. 
  *  Each string introduced will be a line in the specified area.
  * @param area the area to be modified
  * @param str a string that contains the information to be included in a particular area
  */
void screen_area_puts(Area *area, char *str);

/**
  * @brief It counts the length of the string (characters). Compatible with UTF-8. It ignores color scape codes (\033[ ... m)
  * @author Pablo Fernández y Pablo Pérez
  *
  * @param str the string to count
  * @return the length of the string
  */
int screen_multibyte_strlen(char *str);

/**
  * @brief It counts the size (bytes) of moving x positions in the array. Compatible with UTF-8. It ignores color scape codes (\033[ ... m)
  * @author Pablo Fernández y Pablo Pérez
  *
  * @param str the string to count
  * @return the size (bytes) of moving x positions in the array
  */
int screen_multibyte_move(char *str, int x);

/**
  * @brief It prints a background color code into a string 
  * @author Pablo Fernández y Pablo Pérez
  *
  * @param color the color to print
  * @return the bytes/characters printed or -1 if an error ocurred
  */
int screen_sprint_background_color_code(char *str, Color color);

/**
  * @brief It prints a foreground color code into a string 
  * @author Pablo Fernández y Pablo Pérez
  *
  * @param color the color to print
  * @return the bytes/characters printed or -1 if an error ocurred
  */
int screen_sprint_foreground_color_code(char *str, Color color);

#endif
