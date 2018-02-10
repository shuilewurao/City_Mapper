/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   segmentInfo.h
 * Author: chenxu23
 *
 * Created on February 17, 2017, 7:50 PM
 */
#pragma once
#ifndef SEGMENTINFO_H
#define SEGMENTINFO_H

#include <string>
#include <vector>
#include "StreetsDatabaseAPI.h"
#include "graphics.h"

struct segmentInfo {
    
    unsigned index;
    StreetSegmentInfo information;
    std::vector<t_point> curvePointPosition;
    std::vector<float> curvePointAngle;
    std::vector<float> curveLength;
    double segmentLength;
};

#endif /* SEGMENTINFO_H */

