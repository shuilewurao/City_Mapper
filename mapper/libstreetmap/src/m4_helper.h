/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m4_helper.h
 * Author: chenxu23
 *
 * Created on April 1, 2017, 6:07 PM
 */
#pragma once
#ifndef M4_HELPER_H
#define M4_HELPER_H

#include "m1.h"
#include "m3.h"
#include "m4.h"
#include "StreetsDatabaseAPI.h"
#include "packageInfo.h"
#include "SearchCell.h"
#include "segmentInfo.h"
#include "intersectionInfo.h"

#include <thread>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>
#include <chrono>
#include <mutex>

#define TIME_LIMIT 15

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*-- Pre-load and Greedy functions                                          --*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void load_deliveries(const std::vector<DeliveryInfo>& deliveries, const float turn_penalty);
std::vector<closest_info> multi_destination_Dijkstra(const unsigned intersect_id_start, std::vector<unsigned> destinations, const float turn_penalty, int inThread);
void load_cost(std::vector<unsigned> sources, std::vector<unsigned> destinations, const float turn_penalty, int inThread);
unsigned closest_intersection(unsigned intersection_index);
unsigned closest_intersection2(unsigned intersection_index, const float turn_penalty);
bool check_legality(unsigned intersectionIndex);
void visit_intersection(unsigned intersectionId);
bool check_done_delivery();
unsigned closest_depot(unsigned intersection_index, std::vector<unsigned> depots, const float turn_penalty);

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*-- 2opt functions                                                         --*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


std::vector<unsigned> two_opt_swap(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
bool check_path_legality(std::vector<unsigned> proposed_path);
bool optimality(std::vector<unsigned> old_path_intersection, std::vector<unsigned> new_path_intersection, const float turn_penalty, const unsigned& i, const unsigned& j);
float cost_from_intersection(std::vector<unsigned> path, const float turn_penalty);

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*-- functions that cover 48 - 1 = 47 cases that 2-opt doesn't cover        --*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//void threaded_forty_eight_sub_swap(std::vector<unsigned> curr_path_intersection, const float turn_penalty, const unsigned& i, const unsigned& j);
//void forty_eight_sub_swap(unsigned thread_case, std::vector<unsigned> curr_path_intersection, const float turn_penalty, const unsigned& i, const unsigned& j);

std::vector<unsigned> ABC(std::vector<unsigned> curr_path_intersection);
std::vector<unsigned> AB_C(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
std::vector<unsigned> ABC_(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
std::vector<unsigned> AB_C_(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
std::vector<unsigned> A_BC(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
std::vector<unsigned> A_BC_(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
std::vector<unsigned> A_B_C(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
std::vector<unsigned> A_B_C_(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//std::vector<unsigned> sub_ABC(std::vector<unsigned> curr_path_intersection, const float turn_penalty);
std::vector<unsigned> sub_ACB(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
std::vector<unsigned> sub_BAC(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
std::vector<unsigned> sub_BCA(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
std::vector<unsigned> sub_CAB(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
std::vector<unsigned> sub_CBA(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



//B includes i and j, C includes k
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
std::vector<unsigned> ABDC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> ACBD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> ACDB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> ADBC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> ADCB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);

std::vector<unsigned> BACD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> BADC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> BCAD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> BCDA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> BDAC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> BDCA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);

std::vector<unsigned> CABD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> CADB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> CBAD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> CBDA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> CDAB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> CDBA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);

std::vector<unsigned> DABC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> DACB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> DBAC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> DBCA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> DCAB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
std::vector<unsigned> DCBA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

std::vector<unsigned> ABCED(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ABDCE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ABDEC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ABECD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ABEDC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);

std::vector<unsigned> ACBDE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ACBED(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ACDBE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ACDEB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ACEBD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ACEDB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);

std::vector<unsigned> ADBCE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ADBEC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ADCBE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ADCEB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ADEBC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> ADECB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);

std::vector<unsigned> AEBCD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> AEBDC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> AECBD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> AECDB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> AEDBC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
std::vector<unsigned> AEDCB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);

//std::vector<unsigned> BACDE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BACED(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BADCE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BADEC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BAECD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BAEDC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//
//std::vector<unsigned> BCABDE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BCABED(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BCADBE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BCADEB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BCAEBD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BCAEDB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//
//std::vector<unsigned> BDACE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BDAEC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BDCAE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BDCEA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BDEAC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);
//std::vector<unsigned> BDECA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l);




void two_opt_swapABC(std::vector<unsigned> path, const float turn_penalty, float best_cost);
void two_opt_swapA_BC(std::vector<unsigned> path, const float turn_penalty, float best_cost);
void two_opt_swapAB_C(std::vector<unsigned> path, const float turn_penalty, float best_cost);
void three_opt_swapABCD(std::vector<unsigned> path, const float turn_penalty, float best_cost);
void four_opt_swap1(std::vector<unsigned> path, const float turn_penalty, float best_cost);
void four_opt_swap2(std::vector<unsigned> path, const float turn_penalty, float best_cost);

std::vector<unsigned> check_global_best(std::vector<unsigned>& current_best_path, float current_best_cost);

std::vector<unsigned> find_path_between_intersections1(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty);
std::vector<unsigned> find_path_between_intersections2(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty);
std::vector<unsigned> find_path_between_intersections3(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty);
std::vector<unsigned> find_path_between_intersections4(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty);
std::vector<unsigned> find_path_between_intersections5(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty);
std::vector<unsigned> find_path_between_intersections6(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty);

float cost_from_intersection1(std::vector<unsigned> path, const float turn_penalty);
float cost_from_intersection2(std::vector<unsigned> path, const float turn_penalty);
float cost_from_intersection3(std::vector<unsigned> path, const float turn_penalty);
float cost_from_intersection4(std::vector<unsigned> path, const float turn_penalty);
float cost_from_intersection5(std::vector<unsigned> path, const float turn_penalty);
float cost_from_intersection6(std::vector<unsigned> path, const float turn_penalty);



#endif /* M4_HELPER_H */
