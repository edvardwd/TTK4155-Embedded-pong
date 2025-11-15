#include "drivers/oled_menu.h"
/////////////////////////////////////////////

void oled_menu_line_action(){
    oled_clear_disp();
    oled_line(20, 20, 40, 20);
    _delay_ms(500);
}

void oled_menu_circle_action(){
    oled_clear_disp();
    oled_circle(40, 40, 10);
    _delay_ms(500);
}



menu_t* oled_menu_init(){
    static uint8_t menu_idx = 0;
    
    // Only create root + 3 main items (no deep submenus)
    static menu_t root;
    static menu_t m_draw_line;
    static menu_t m_draw_circle;
    static menu_t m_play_game;


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

    // Init 3 main menu items only
    m_play_game.title     = "Play game";    m_play_game.action     = &game_play;    m_play_game.parent = &root; m_play_game.n_elems = 0;
    m_draw_line.title     = "Draw line";    m_draw_line.action     = &oled_menu_line_action; m_draw_line.parent = &root; m_draw_line.n_elems = 0;
    m_draw_circle.title   = "Draw circle";  m_draw_circle.action   = &oled_menu_circle_action; m_draw_circle.parent = &root; m_draw_circle.n_elems = 0;
   
    // Add to root
    root.sub_menus[root.n_elems++] = &m_play_game;
    root.sub_menus[root.n_elems++] = &m_draw_line;
    root.sub_menus[root.n_elems++] = &m_draw_circle;

    return &root;
}


void oled_menu_navigate(menu_t *m){
    // Check if a new element has been chosen
    if (joystick_get_button_pressed()){
        menu_t* selected = m->sub_menus[m->curr_elem];
        oled_clear_disp();
        // Check if action button
        if (selected->action != NULL){
            selected->action();
        } 
        else{
            m = selected;
        }
        oled_menu_print(m);
        return;
    }
    

    joystick_dir_t dir = joystick_get_dir();
    uint8_t curr_elem = m->curr_elem; // The "selected" element in the menu
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

void oled_menu_print(menu_t *m){
    oled_clear_disp();
    for (uint8_t i = 0; i < m->n_elems; i++){
        menu_t* child = (m->sub_menus[i]);
        oled_print(i*8, 0, child->title);
    }
}

