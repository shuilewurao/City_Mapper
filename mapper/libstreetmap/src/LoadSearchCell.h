/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LoadSearchCell.h
 * Author: chenxu23
 *
 * Created on March 10, 2017, 12:45 AM
 */
#pragma once
#ifndef LOADSEARCHCELL_H
#define LOADSEARCHCELL_H

#include "m1.h"
#include "SearchCell.h"
#include "intersectionInfo.h"
#include "StreetsDatabaseAPI.h"

#include <algorithm>

extern std::vector<intersectionInfo> intersectionInformation;
extern std::vector<StreetSegmentInfo> streetSegmentInfo;
extern std::vector<std::vector<unsigned>> intersectionStreetSeg;
extern std::vector<double> TravelTime;
extern std::vector<SearchCell> intersectionCells;

void load_search_cell(void);

#endif /* LOADSEARCHCELL_H */

