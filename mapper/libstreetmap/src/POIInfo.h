/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   poiInfo.h
 * Author: chenxu23
 *
 * Created on February 21, 2017, 5:38 AM
 */
#include <string>
#include "StreetsDatabaseAPI.h"
#include "graphics.h"
#ifndef POIINFO_H
#define POIINFO_H

struct POIInfo {
    std::string POItype;
    std::string POIname;
    t_point XY_position;
    LatLon LatLon_position;
    OSMID POIOSMID;
};


#endif /* POIINFO_H */

