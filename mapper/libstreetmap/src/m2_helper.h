/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m2_helper.h
 * Author: chenxu23
 *
 * Created on February 16, 2017, 6:29 PM
 */
#pragma once
#include <string>
#include <vector>
#include <math.h>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "m1.h"
#include "m3.h"
#include "DataStructures.h"
#include "intersectionInfo.h"
#include "segmentInfo.h"
#include "featureInfo.h"
#include "POIInfo.h"
#include "constants.h"
#include "SearchCell.h"

#include "StreetsDatabaseAPI.h"
#include "Surface.h"
#include "easygl/easygl_constants.h"
#include "fontcache.h"
#include "graphics.h"
#include "graphics_automark.h"
#include "graphics_state.h"
#include "graphics_types.h"
#ifndef M2_HELPER_H
#define M2_HELPER_H

extern std::vector<intersectionInfo> intersectionInformation;
extern std::vector<StreetSegmentInfo> streetSegmentInfo;


t_bound_box find_map_coordinates(void);
t_point LatLon_to_xy(LatLon latlon);
LatLon xy_to_LatLon(t_point xy);
float find_polygon_area(std::vector<float> x, std::vector<float>, unsigned numPoint);
bool if_in_visible_world(t_bound_box boundary, t_point location);
float find_distance(t_point xy1, t_point xy2);
// point 1 = first segment last curve point, point 2 = middle intersection, point 3 = second segment first curve point
std::string direction_between_two_segments(t_point point1, t_point point2, t_point point3);
std::vector<std::string>  direction_of_path(std::vector<unsigned> path);

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

void load_data(void);

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

void draw_streets(std::vector<segmentInfo> streets, t_color color, float width);
void print_street_name(std::vector<segmentInfo> segmentInformation);
void draw_point_of_interest(std::vector<POIInfo> poiInformation, char* path);
void print_point_of_interest_name(std::vector<POIInfo> poiInformation);
void draw_feature(std::vector<featureInfo> featureInformation, t_color color);
void print_feature_name(std::vector<featureInfo> featureInformation);

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

void act_on_button(float x, float y, t_event_buttonPressed buttonPressed);
void act_on_intersection(void (*drawscreen_ptr) (void));
void act_print_feature_name(void (*drawscreen_ptr) (void));
void act_print_intersection_name(void (*drawscreen_ptr) (void));
void act_print_POI_name(void (*drawscreen_ptr) (void));

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

void act_on_help_button(void (*drawscreen_ptr) (void));
void act_on_Find_button(void (*drawscreen_ptr) (void));
void act_on_Path_button(void (*drawscreen_ptr) (void));
void act_on_Click_Path_button(void (*drawscreen_ptr) (void));
void act_on_Search_Path_to_POI_button(void (*drawscreen_ptr) (void));
void act_on_Click_Path_to_POI_button(void (*drawscreen_ptr) (void));
void act_on_click(float x, float y, t_event_buttonPressed buttonPressed);
void act_on_click_to_POI(float x, float y, t_event_buttonPressed buttonPressed);
void act_on_key_press(char c, int keysym);
void draw_search_box(void);
void drawscreen_for_button(void);
void parser(void);
void parser_path(void);
void draw_path(unsigned intersection1, unsigned intersection2);
void draw_path_to_poi(unsigned intersection_index, string POI_name);
void draw_error(string error_message);
void draw_instruction(vector<unsigned> path);
void highlight_street(void);
unsigned levenshtein_distance(string s1, string s2);
string fuzzy_search(string inputName);
string fuzzy_search_POI(string inputName);

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
void draw_scale(void);
float get_zoom_level(void);
void act_on_button_press(float x, float y, t_event_buttonPressed event);
void draw_zoom_level_0(void);
void draw_zoom_level_1(void);
void draw_zoom_level_2(void);
void draw_zoom_level_3(void);
void draw_zoom_level_4(void);
void draw_zoom_level_5(void);
void drawscreen(void);

void clear_map(void);

#endif /* M2_HELPER_H */
