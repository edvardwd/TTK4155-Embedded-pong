#ifndef OLED_MENU_H
#define OLED_MENU_H
#include "stdint.h"

#define MAX_SUBMENUS 8 //1 SUBMENU PER PAGE

typedef void (*menu_func_t)(void);

typedef struct menu_t
{
    const char *title; //Title for Menu element
    menu_func_t action; //Func to be called. If NULL --> Do nuthing
    struct menu_t *parent;
    struct menu_t *sub_menus[MAX_SUBMENUS];
    uint8_t allowed_elems; 
} menu_t;

void add_menu_elem_w_sub(menu_t *menu, const char *title, menu_t *parent);
void add_menu_elem_w_func(menu_t *menu, const char *title, menu_func_t action, menu_t *parent);
void init_menu(); 
    
