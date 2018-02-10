/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   intersectionInfo.h
 * Author: ninghuiy
 *
 * Created on January 30, 2017, 1:56 PM
 */
#pragma once
#include "StreetsDatabaseAPI.h"
#include <string>
#include <vector>

#ifndef INTERSECTIONINFO_H
#define INTERSECTIONINFO_H


struct intersectionInfo {
    
    unsigned intersection_index;
    std::string intersection_name;
    LatLon intersection_position;
    std::vector<unsigned> street_segment_index;
    std::vector<std::string> intersection_street_names;
    std::vector<StreetSegmentInfo> street_segment_info;
    
};


#endif /* INTERSECTIONINFO_H */

