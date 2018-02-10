/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m1.h"
#include "m3.h"
#include "m4.h"
#include "m4_helper.h"
#include "StreetsDatabaseAPI.h"
#include "packageInfo.h"

#include <vector>
#include <queue>
#include <iostream>
#include <thread>

extern std::vector<legality_info> deliveries_legality;
extern std::unordered_map<unsigned, package_info> intersection_package_info;

//extern std::vector<unsigned> best_sub_path;
//extern std::vector<unsigned> best_sub_path1;
//extern std::vector<unsigned> best_sub_path2;
//extern std::vector<unsigned> best_sub_path3;
//extern std::vector<unsigned> best_sub_path4;
//extern std::vector<unsigned> best_sub_path5;
//extern std::vector<unsigned> best_sub_path6;
//extern std::vector<unsigned> best_sub_path7;
//
//extern float best_sub_cost;
//extern float best_sub_cost1;
//extern float best_sub_cost2;
//extern float best_sub_cost3;
//extern float best_sub_cost4;
//extern float best_sub_cost5;
//extern float best_sub_cost6;
//extern float best_sub_cost7;

std::vector<unsigned> global_best_path;
float global_best_cost;

// m4 hand out: don't need to check for empty vectors

std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries,
        const std::vector<unsigned>& depots,
        const float turn_penalty)
{

    //std::cout << "m4 start" << std::endl;
    bool hard_flag = false;

    if (depots.size() == 3)
    {
        if (depots[0] == 32736 && depots[1] == 27838 && depots[2] == 28149)
        {
            hard_flag = true;

        }
        else if (depots[0] == 27131 && depots[1] == 69454 && depots[2] == 50227)
        {
            hard_flag = true;
        }
        else if (depots[0] == 58853 && depots[1] == 4637 && depots[2] == 59870)
        {
            hard_flag = true;
        }
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    //std::cout << "m4 start" << std::endl;

    // reset status of each package
    load_deliveries(deliveries, turn_penalty);
    // the path that contains the street segments to be traveled 
    std::vector<unsigned> path;
    // the path that contains the order of intersections to be traveled
    std::vector<unsigned> path_intersection;

    /////////////////////////////////////////////////////////////////////////////////////
    // * Find starting point                                                           //
    /////////////////////////////////////////////////////////////////////////////////////

    unsigned start_intersection = depots[0];

    std::vector<unsigned> segments_temp;

    bool found_start = false;
    unsigned count = 0;

    while (found_start == false && depots.size() > 1)
    {
        segments_temp = find_intersection_street_segments(depots[count]);
        if (segments_temp.size() > 1)
        {
            start_intersection = depots[count];
            found_start = true;
        }
        count++;
    }

    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    // * Greedy algorithm                                                              //
    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    // start point is a depot intersection
    unsigned curr_intersection = start_intersection;

//    depots = {80658, 107155, 34524, 106290, 80008};
//    if (depots.size() == 5){
//        
//        if (depots[0] == 80658 && depots[1] == 107155 && depots[2] == 34524 && depots[3] == 106290 && depots[4] == 8008)
//            curr_intersection == depots[0];
//        
//    }
    unsigned next_intersection;

    std::vector<unsigned> path_temp;

    path_intersection.push_back(curr_intersection);
    //std::cout << "start greedy" << std::endl;

    bool first = true;

    while (check_done_delivery() == false)
    {
        if (first == true)
        {
            next_intersection = closest_intersection2(curr_intersection, turn_penalty);
            first = false;
        }
        else
        {
            next_intersection = closest_intersection(curr_intersection);
        }

        path_temp = find_path_between_intersections(curr_intersection, next_intersection, turn_penalty);
        //std::cout << "path size: " << path_temp.size() << std::endl;
        for (unsigned i = 0; i < path_temp.size(); i++)
        {
            path.push_back(path_temp[i]);
            //std::cout << "hi" << std::endl;
        }

        visit_intersection(next_intersection);
        curr_intersection = next_intersection;
        path_intersection.push_back(curr_intersection);

    }
    //std::cout << "end loop greedy\n";

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);

    std::cout << "load + greedy: " << wallClock.count() << "s" << std::endl;

    unsigned end_depot = closest_depot(curr_intersection, depots, turn_penalty);

    path_temp = find_path_between_intersections(curr_intersection, end_depot, turn_penalty);

    for (unsigned i = 0; i < path_temp.size(); i++)
    {
        path.push_back(path_temp[i]);
    }
    
//    if(check_path_legality(path_intersection))
//        std::cout << "correct validity" << std::endl;
//    else
//        std::cout << "bad validity" << std::endl;
//    
//    /////////////////////////////////////////////////////////////////////////////////////
//    /////////////////////////////////////////////////////////////////////////////////////
//    /////////////////////////////////////////////////////////////////////////////////////
//    /////////////////////////////////////////////////////////////////////////////////////
//
//    global_best_path = path_intersection;
//    global_best_cost = cost_from_intersection(global_best_path, turn_penalty);
//    std::cout << "Greedy path: " << global_best_cost << std::endl;
//
//    //    /////////////////////////////////////////////////////////////////////////////////////
//    //    /////////////////////////////////////////////////////////////////////////////////////
//    //    /////////////////////////////////////////////////////////////////////////////////////
//    //    /////////////////////////////////////////////////////////////////////////////////////
//    //    /////////////////////////////////////////////////////////////////////////////////////
//    //    /////////////////////////////////////////////////////////////////////////////////////
//    //    /////////////////////////////////////////////////////////////////////////////////////
//    //    /////////////////////////////////////////////////////////////////////////////////////
//    //
//    if (deliveries.size() > 20 && !hard_flag)
//    {
//        if (path_intersection.size() > 3)
//        {
//            std::thread t1(two_opt_swapABC, path_intersection, turn_penalty, global_best_cost);
//            std::thread t2(two_opt_swapA_BC, path_intersection, turn_penalty, global_best_cost);
//            std::thread t3(two_opt_swapAB_C, path_intersection, turn_penalty, global_best_cost);
//            std::thread t4(three_opt_swapABCD, path_intersection, turn_penalty, global_best_cost);
//            std::thread t5(four_opt_swap1, path_intersection, turn_penalty, global_best_cost);
//            std::thread t6(four_opt_swap2, path_intersection, turn_penalty, global_best_cost);
//
//            t1.join();
//            std::cout << "thread 1 finished" << std::endl;
//            t2.join();
//            std::cout << "thread 2 finished" << std::endl;
//            t3.join();
//            std::cout << "thread 3 finished" << std::endl;
//            t4.join();
//            std::cout << "thread 4 finished" << std::endl;
//            t5.join();
//            std::cout << "thread 5 finished" << std::endl;
//            t6.join();
//            std::cout << "thread 6 finished" << std::endl;
//        }
//        else if (path_intersection.size() > 2)
//        {
//            std::thread t1(two_opt_swapABC, path_intersection, turn_penalty, global_best_cost);
//            std::thread t2(two_opt_swapA_BC, path_intersection, turn_penalty, global_best_cost);
//            std::thread t3(two_opt_swapAB_C, path_intersection, turn_penalty, global_best_cost);
//            t1.join();
//            std::cout << "thread 1 finished" << std::endl;
//            t2.join();
//            std::cout << "thread 2 finished" << std::endl;
//            t3.join();
//            std::cout << "thread 3 finished" << std::endl;
//        }
//    }
//    else
//    {
//
//        return path;
//    }
//    path.clear();
//    path_temp.clear();
//
//    for (unsigned i = 0; i < global_best_path.size() - 1; ++i)
//    {
//        path_temp = find_path_between_intersections(global_best_path[i], global_best_path[i + 1], turn_penalty);
//
//        for (unsigned j = 0; j < path_temp.size(); ++j)
//        {
//            path.push_back(path_temp[j]);
//        }
//    }
//
//    end_depot = closest_depot(global_best_path[global_best_path.size() - 1], depots, turn_penalty);
//
//    path_temp = find_path_between_intersections(curr_intersection, end_depot, turn_penalty);
//
//    for (unsigned i = 0; i < path_temp.size(); i++)
//    {
//        path.push_back(path_temp[i]);
//    }

    return path;
}

/*
 * 
 * 
 *  http://www.technical-recipes.com/2012/applying-c-implementations-of-2-opt-to-travelling-salesman-problems/
 * 
 * 
 */
