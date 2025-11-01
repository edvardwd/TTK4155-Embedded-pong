#include "drivers/oled_menu.h"
/////////////////////////////////////////////

void add_menu_elem_w_sub(menu_t *menu, const char *title, menu_t *parent){
    menu->title = title;
    menu->action = NULL;
    menu->parent = parent;
    menu->n_elems = MAX_SUBMENUS;

    for (uint8_t i = 0; i < MAX_SUBMENUS; i++){
        menu->sub_menus[i] = NULL; //Make sure all submenu elem pointers are defined
    }
}

void add_menu_elem_w_func(menu_t *menu, const char *title, menu_func_t action, menu_t *parent){
    menu->title = title;
    menu->action = action; //Func ptr
    menu->parent = parent;
    menu->n_elems = 0;
    for (uint8_t i = 0; i < MAX_SUBMENUS; i++){
        menu->sub_menus[i] = NULL; //Make sure all submenu elem pointers are defined
    }
}

void game1_action(){
    oled_clear_disp();
    oled_line(20, 20, 40, 20);
}

void game3_action(){
    oled_clear_disp();
    oled_circle(40, 40, 10);
}


// Simplified menu - only store the essentials, reduce to 5 total menus
static menu_t menu_pool[5];
static uint8_t menu_idx = 0;

static menu_t* get_menu(void) {
    return &menu_pool[menu_idx++];
}

menu_t* init_menu(void){
    menu_idx = 0;
    
    // Only create root + 4 main items (no deep submenus)
    static menu_t root;
    static menu_t m_game1;
    static menu_t m_game2;
    static menu_t m_game3;
    static menu_t m_game4;

    // Init root
    root.title = "Menu";
    root.action = NULL;
    root.parent = NULL;
    root.n_elems = 0;
    root.curr_elem = 0;
    root.scroll_dir = NEUTRAL;
    for (uint8_t i=0; i<MAX_SUBMENUS; i++) {
        root.sub_menus[i] = NULL;
    }

    // Init 4 main menu items only
    m_game1.title     = "Draw line";   m_game1.action = &game1_action; m_game1.parent = &root; m_game1.n_elems = 0;
    m_game2.title     = "Draw circle"; m_game2.action = &game3_action; m_game2.parent = &root; m_game2.n_elems = 0;
    m_game3.title     = "Game 3";      m_game3.action = NULL;          m_game3.parent = &root; m_game3.n_elems = 0;
    m_game4.title     = "Game 4";      m_game4.action = NULL;          m_game4.parent = &root; m_game4.n_elems = 0;

    // Add to root
    root.sub_menus[root.n_elems++] = &m_game1;
    root.sub_menus[root.n_elems++] = &m_game2;
    root.sub_menus[root.n_elems++] = &m_game3;
    root.sub_menus[root.n_elems++] = &m_game4;

    return &root;
}


void navigate_menu(menu_t *m){
    // Check if a new element has been chosen
    if (!(PINE & (1 << JOYSTICK_BUTTON))){
        menu_t* selected = m->sub_menus[m->curr_elem];
        oled_clear_disp();
        // Check if action button
        if (selected->action != NULL){
            selected->action();
        } 
        else{
            m = selected;
            oled_print_menu(m);
        }
        return;
    }
    

    joystick_dir_t dir = joystick_get_dir();
    uint8_t curr_elem = m->curr_elem; // The "selected" element in th emenu
    uint8_t ptr_start_col = N_COLS - (2 * FONT_WIDTH + 1); // Start column for drawing pointer to screen

    // Draw pointer arrow in the correct place
    oled_print(curr_elem*8, ptr_start_col, "<=");
    
    
    if (dir.vertical_dir == NEUTRAL){
        m->scroll_dir = NEUTRAL;
        return;
    }
    
    if (m->scroll_dir == dir.vertical_dir) return;

    switch (dir.vertical_dir){            
        case UP:
        curr_elem = (curr_elem == 0) ? m->n_elems-1 : curr_elem - 1;
        break;
        
        case DOWN:
        curr_elem = (curr_elem + 1) % m->n_elems;
        
        default:
        break;
    }
    
    // Clear trailing pointer arrow
    for (uint8_t i = ptr_start_col; i < N_COLS; i++){
        oled_clear_column(m->curr_elem*8, i);
    }
    
    m->curr_elem = curr_elem;
    m->scroll_dir = dir.vertical_dir;
}

void oled_print_menu(menu_t *m){
    for (uint8_t i = 0; i < m->n_elems; i++){
        menu_t* child = (m->sub_menus[i]);
        oled_print(i*8, 0, child->title);
    }
}

