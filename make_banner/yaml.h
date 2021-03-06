/********************************************************

YAML Interpeter

Author: 3DSGuy � 2013

This file is part of make_banner.

make_banner is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

make_banner is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with make_banner.  If not, see <http://www.gnu.org/licenses/>.

********************************************************/


/** Key Search Progress States **/
#define SEEK_NEWLINE 0
#define SEEK_KEY_START 1
#define STORE_KEY 2
#define SEEK_COLON 3

/** Key Value Retreaval Progress States **/
#define SEEK_VALUE_START 0
#define STORE_VALUE_REG 1 //A Regular Key string value
#define STORE_VALUE_QUO 2 //A String in quotes

/** Key Search Return Values **/
#define FOUND 0
#define NOT_FOUND 1

/** Boolean Return Values **/
#define FALSE 0
#define TRUE 1

/** get_* Return Values **/
#define KEY_FOUND 0
#define KEY_ERROR 2
#define VALUE_ERROR 3


/** PROTOTYPES **/

//Searches for a key in a given YAML file, until an error
int key_search(char *key, FILE *file);

//Searches for the value of a key
int get_value(unsigned char *output, int max_size, char *key, FILE *file);

//Returns a boolean value
int get_boolean(char *key, FILE *file);
 
