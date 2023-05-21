# libscreen

libscreen is a terminal-based graphic library capable of showing text organized in different areas over a colored background. It is written in C and it supports UTF-8 encoding (Unicode) as well as terminal color codes.


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

### string management functions   

+ `screen_multibyte_strlen`

    Counts the number of characters in a string (it could not match with the byte length of the string), it ignore color codes.

+ `screen_multibyte_move`

    Return the number of bytes if you move `x` characters, color codes does not count as characters.

### makefile

+ `build`

    It compiles and generates the static library.

+ `build_test`

    It compiles the test and the library.

+ `run_test`

    It compiles the test and the library and runs it.

+ `clean`

    It remove the executables, the static library and the compiled objects.

## Authors

Pablo Fernández Izquierdo and Pablo Pérez Hernández
Universidad Autónoma de Madrid (UAM)

## License

libscreen is Free Software: You can use, study, share, and improve it at will. Specifically you can redistribute and/or modify it under the terms of the GNU General Public License, version 3, as published by the Free Software Foundation.