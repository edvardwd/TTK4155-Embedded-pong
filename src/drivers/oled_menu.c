#include "drivers/oled.h"
#include "drivers/oled_menu.h"
#include "fonts.h"

void add_menu_elem_w_sub(menu_t *menu, const char *title, menu_t *parent){
    menu->title = title;
    menu->action = NULL;
    menu->parent = parent;
    menu->allowed_elems = MAX_SUBMENUS;

    for (uint8_t i = 0; i < MAX_SUBMENUS; i++){
        menu->sub_menus[i] = NULL; //Make sure all submenu elem pointers are defined
    }
}

void add_menu_elem_w_func(menu_t *menu, const char *title, menu_func_t action, menu_t *parent){
    menu->title = title;
    menu->action = action; //Func ptr
    menu->parent = parent;
    menu->allowed_elems = 0;
    for (uint8_t i = 0; i < MAX_SUBMENUS; i++){
        menu->sub_menus[i] = NULL; //Make sure all submenu elem pointers are defined
    }
}



void init_menu(){
    
}