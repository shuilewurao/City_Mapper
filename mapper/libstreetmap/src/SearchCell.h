/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SearchMap.h
 * Author: chenxu23
 *
 * Created on March 7, 2017, 9:00 PM
 */
#pragma once
#ifndef SEARCHCELL_H
#define SEARCHCELL_H

#include "StreetsDatabaseAPI.h"
#include <math.h>
#include <vector>

struct cellInfo {
    
    float travelTime;

    unsigned intersection_id; //connected intersection

    unsigned streetsegment_id; //connected segment

};

class SearchCell {
public:

    // intersection id
    unsigned id;

    // cost of each segment connected to the intersection with
    std::vector<cellInfo> directed_connections;

    float G = 0;
    float H = 0;
    float F = std::numeric_limits<float>::infinity();

    unsigned parent_street_segment;
    unsigned parent_intersection;
    bool visited = false;

};

#endif /* SEARCHMAP_H */

