/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   packageInfo.h
 * Author: chenxu23
 *
 * Created on March 23, 2017, 4:59 PM
 */
#pragma once
#ifndef PACKAGEINFO_H
#define PACKAGEINFO_H

#include <vector>

struct legality_info {
    bool pickup;
    bool dropoff;

};

struct closest_info {
    unsigned intersection_index;
    float cost;
    std::vector<unsigned> segment_path;
};

struct package_info {
    std::vector<unsigned> pickup;
    std::vector<unsigned> dropoff;
    std::vector<closest_info> cost_depot;
    std::vector<closest_info> cost_delivery;
};

struct cost_info {
    unsigned path_number;
    float path_cost;
};

#endif /* PACKAGEINFO_H */


