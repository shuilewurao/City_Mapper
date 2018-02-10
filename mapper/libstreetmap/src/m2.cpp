/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m2.h"
#include "m2_helper.h"

// Background color for all maps
const t_color INITIAL_COLOR = t_color(234, 234, 234);
// Callbacks for event-driven window handling.

void draw_map(void) {
    //find_map_coordinates();
    std::cout << "About to start graphics.\n";

    t_bound_box INITIAL_COORD = find_map_coordinates();
    init_graphics("map", INITIAL_COLOR);
    set_drawing_buffer(OFF_SCREEN);
    set_visible_world(INITIAL_COORD);

    set_keypress_input(true);
    
    create_button("Window", "Feature Name", act_print_feature_name);
    create_button("Feature Name", "Intersection", act_print_intersection_name);
    create_button("Intersection", "POI", act_print_POI_name);
    create_button("POI", "Find", act_on_Find_button);
    create_button("Find", "Search Path", act_on_Path_button);
    create_button("Search Path", "Click Path", act_on_Click_Path_button);
    create_button("Click Path", "Search POI", act_on_Search_Path_to_POI_button);
    create_button("Search POI", "Click POI", act_on_Click_Path_to_POI_button);
    create_button("Click POI","Help", act_on_help_button);
    
    event_loop(act_on_button, NULL, act_on_key_press, drawscreen);
    copy_off_screen_buffer_to_screen();
    clear_map();
    
    close_graphics();
}

