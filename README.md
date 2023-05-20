# libscreen

libscreen is a terminal-based graphic library capable of showing text organized in different areas over a colored background. It is written in C.


## Interacting with the library

libscreen works using two data structures: screens and areas. Screens show the background and hold inside of them the areas. Only on screen at the same time is supported. Areas are where text is written. 

### libscreen functions

+ `screen_init`

	Initializes the data structure for a screen, storing the numbers of rows and columns to display. This function only needs to be called once at the beggining.

+ `screen_area_init`

    Initializes the data structure for an area, such as its position in the screen (x and y coordinates) and size (number of rows and columns).

+ `screen_area_puts`

    Stores in an area the text to show. If the string to write overflows the area width it puts the remaining onto the next line. If the area is full if shift the lines up so more text can be added.

+ `screen_paint`

    Paints in the terminal the screen with its background, the areas and the text they may have.

+ `screen_area_set_cursor`

    Changes the line where text is being stored in an area.

+ `screen_area_clear`

    Removes text from area.
    
+ `screen_area_destroy`

    Deletes the area from the screen and frees dynamic memory used for areas. This function should be called at the end of the program for each area created.

    
## Authors

Pablo Fernández Izquierdo and Pablo Pérez Hernández
Universidad Autónoma de Madrid (UAM)

## License

libscreen is Free Software: You can use, study, share, and improve it at will. Specifically you can redistribute and/or modify it under the terms of the GNU General Public License, version 3, as published by the Free Software Foundation.