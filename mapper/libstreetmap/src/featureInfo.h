/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   featureInfo.h
 * Author: chenxu23
 *
 * Created on February 18, 2017, 2:31 PM
 */
#pragma once
#include "StreetsDatabaseAPI.h"
#include "graphics_types.h"
#include <string>
#include <vector>


#ifndef FEATUREINFO_H
#define FEATUREINFO_H

struct featureInfo {
    unsigned index;
    std::string featureName;
    FeatureType featureType;
    TypedOSMID featureOSMID;
    unsigned featurePointCount;
    std::vector<t_point> featurePoint;
    float area;
    bool polygon = false;
};


#endif /* FEATUREINFO_H */