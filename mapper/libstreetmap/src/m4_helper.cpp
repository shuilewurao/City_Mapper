/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "m4_helper.h"

struct compare
{

    bool operator()(const SearchCell & l, const SearchCell& r)
    {
        return (l.F > r.F);
    }
};

struct myLeast
{

    bool operator()(const closest_info & l, const closest_info& r)
    {
        return (l.cost < r.cost);
    }
};

extern std::vector<StreetSegmentInfo> streetSegmentInfo;
extern std::vector<intersectionInfo> intersectionInformation;
extern std::vector<SearchCell> intersectionCells;
extern std::vector<SearchCell> intersectionCells1;
extern std::vector<SearchCell> intersectionCells2;
extern std::vector<SearchCell> intersectionCells3;
extern std::vector<SearchCell> intersectionCells4;
extern std::vector<SearchCell> intersectionCells5;
extern std::vector<SearchCell> intersectionCells6;
extern std::vector<SearchCell> intersectionCells7;
extern std::vector<SearchCell> intersectionCells8;
extern std::vector<SearchCell> intersectionCells9;
extern std::vector<SearchCell> intersectionCells10;
extern std::vector<SearchCell> intersectionCells11;
extern std::vector<SearchCell> intersectionCells12;
extern std::vector<SearchCell> intersectionCells13;
extern std::vector<SearchCell> intersectionCells14;
extern std::vector<SearchCell> intersectionCells15;
extern std::vector<SearchCell> intersectionCells16;
extern std::vector<double> TravelTime;

// Global containers to be used throughout milestone 4
std::vector<legality_info> deliveries_legality;
std::unordered_map<unsigned, package_info> intersection_package_info;

extern std::vector<unsigned> global_best_path;
extern float global_best_cost;

std::mutex wrLock;
std::mutex globalLock;
//
//float best_primary_cost;
//float best_primary_cost1;
//float best_primary_cost2;
//float best_primary_cost3;
//float best_primary_cost4;
//float best_primary_cost5;
//
//std::vector<unsigned> best_sub_path;
//std::vector<unsigned> best_sub_path1;
//std::vector<unsigned> best_sub_path2;
//std::vector<unsigned> best_sub_path3;
//std::vector<unsigned> best_sub_path4;
//std::vector<unsigned> best_sub_path5;
//std::vector<unsigned> best_sub_path6;
//std::vector<unsigned> best_sub_path7;
//
//float best_sub_cost;
//float best_sub_cost1;
//float best_sub_cost2;
//float best_sub_cost3;
//float best_sub_cost4;
//float best_sub_cost5;
//float best_sub_cost6;
//float best_sub_cost7;

void load_deliveries(const std::vector<DeliveryInfo>& deliveries, const float turn_penalty)
{

    //std::cout << "load start" << std::endl;

    deliveries_legality.clear();
    intersection_package_info.clear();
    std::vector<unsigned> all_intersections;

    auto startTime = std::chrono::high_resolution_clock::now();

    for (unsigned i = 0; i < deliveries.size(); ++i)
    {
        // load legality
        legality_info tempLegalInfo;
        // this package has not been dropped off
        tempLegalInfo.dropoff = false;

        // this package has not been picked up
        tempLegalInfo.pickup = false;

        deliveries_legality.push_back(tempLegalInfo);


        //        all_intersections.push_back(deliveries[i].dropOff);
        //        all_intersections.push_back(deliveries[i].pickUp);

        /*====================================================================*/
        /*====================================================================*/
        /*====================================================================*/
        // load each package into an unordered_map, store all intersections into
        // a vector

        bool pickup_insert = false;
        bool dropoff_insert = false;

        unsigned pickup_index = deliveries[i].pickUp;
        unsigned dropoff_index = deliveries[i].dropOff;

        auto iter_pickup = intersection_package_info.find(pickup_index);
        if (iter_pickup == intersection_package_info.end())
        {
            pickup_insert = true;
            std::vector<unsigned> pickup;
            pickup.push_back(i);
            package_info tempInfo;
            tempInfo.pickup = pickup;

            intersection_package_info.insert(std::make_pair(pickup_index, tempInfo));
            all_intersections.push_back(pickup_index);
        }

        auto iter_dropoff = intersection_package_info.find(dropoff_index);
        if (iter_dropoff == intersection_package_info.end())
        {
            dropoff_insert = true;
            std::vector<unsigned> dropoff;
            dropoff.push_back(i);
            package_info tempInfo;
            tempInfo.dropoff = dropoff;

            intersection_package_info.insert(std::make_pair(dropoff_index, tempInfo));
            all_intersections.push_back(dropoff_index);
        }

        if (pickup_insert == false)
        {
            iter_pickup -> second.pickup.push_back(i);
        }
        if (dropoff_insert == false)
        {
            iter_dropoff -> second.dropoff.push_back(i);
        }
    }
    //
    //    std::sort(all_intersections.begin(), all_intersections.end());
    //    all_intersections.resize(std::distance(all_intersections.begin(), std::unique(all_intersections.begin(), all_intersections.end())));

    //    std::cout << "correct size: " << intersection_package_info.size() << std::endl;
    //    std::cout << "loaded size: " << all_intersections.size() << std::endl;

    unsigned intersection_size = all_intersections.size();
    /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    if (intersection_size >= 150)
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    {


        int division = intersection_size / 15;
        division--;

        std::vector<unsigned> temp;

        temp.insert(temp.end(), all_intersections.begin() + 0 * division, all_intersections.begin() + 1 * division);
        //std::cout << "leggo first thread!!" << std::endl;
        std::thread t1(load_cost, temp, all_intersections, turn_penalty, 1);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 1 * division, all_intersections.begin() + 2 * division);
        //std::cout << "leggo second thread!!" << std::endl;
        std::thread t2(load_cost, temp, all_intersections, turn_penalty, 2);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 2 * division, all_intersections.begin() + 3 * division);
        //std::cout << "leggo third thread!!" << std::endl;
        std::thread t3(load_cost, temp, all_intersections, turn_penalty, 3);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 3 * division, all_intersections.begin() + 4 * division);
        std::thread t4(load_cost, temp, all_intersections, turn_penalty, 4);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 4 * division, all_intersections.begin() + 5 * division);
        std::thread t5(load_cost, temp, all_intersections, turn_penalty, 5);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 5 * division, all_intersections.begin() + 6 * division);
        std::thread t6(load_cost, temp, all_intersections, turn_penalty, 6);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 6 * division, all_intersections.begin() + 7 * division);
        std::thread t7(load_cost, temp, all_intersections, turn_penalty, 7);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 7 * division, all_intersections.begin() + 8 * division);
        std::thread t8(load_cost, temp, all_intersections, turn_penalty, 8);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 8 * division, all_intersections.begin() + 9 * division);
        std::thread t9(load_cost, temp, all_intersections, turn_penalty, 9);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 9 * division, all_intersections.begin() + 10 * division);
        std::thread t10(load_cost, temp, all_intersections, turn_penalty, 10);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 10 * division, all_intersections.begin() + 11 * division);
        std::thread t11(load_cost, temp, all_intersections, turn_penalty, 11);
        temp.clear();
        temp.insert(temp.end(), all_intersections.begin() + 11 * division, all_intersections.begin() + 12 * division);
        std::thread t12(load_cost, temp, all_intersections, turn_penalty, 12);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 12 * division, all_intersections.begin() + 13 * division);
        std::thread t13(load_cost, temp, all_intersections, turn_penalty, 13);
        temp.clear();
        temp.insert(temp.end(), all_intersections.begin() + 13 * division, all_intersections.begin() + 14 * division);
        std::thread t14(load_cost, temp, all_intersections, turn_penalty, 14);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 14 * division, all_intersections.begin() + 15 * division);
        std::thread t15(load_cost, temp, all_intersections, turn_penalty, 15);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 15 * division, all_intersections.end());
        std::thread t16(load_cost, temp, all_intersections, turn_penalty, 16);
        temp.clear();

        t1.join();
        //std::cout << "1 thread finished !!" << std::endl;
        t2.join();
        //std::cout << "2 thread finished !!" << std::endl;
        t3.join();
        //std::cout << "3 thread finished !!" << std::endl;
        t4.join();
        //std::cout << "4 thread finished !!" << std::endl;
        t5.join();
        //std::cout << "5 thread finished !!" << std::endl;
        t6.join();
        //std::cout << "6 thread finished !!" << std::endl;
        t7.join();
        t8.join();
        t9.join();
        t10.join();
        t11.join();
        t12.join();
        t13.join();
        t14.join();
        t15.join();
        t16.join();
    }
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    else if (intersection_size >= 90)
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    {

        int division = intersection_size / 9;
        division--;

        std::vector<unsigned> temp;

        temp.insert(temp.end(), all_intersections.begin() + 0 * division, all_intersections.begin() + 1 * division);
        //std::cout << "leggo first thread!!" << std::endl;
        std::thread t1(load_cost, temp, all_intersections, turn_penalty, 1);
        temp.clear();
        //
        //
        temp.insert(temp.end(), all_intersections.begin() + 1 * division, all_intersections.begin() + 2 * division);
        //std::cout << "leggo second thread!!" << std::endl;
        std::thread t2(load_cost, temp, all_intersections, turn_penalty, 2);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 2 * division, all_intersections.begin() + 3 * division);
        //std::cout << "leggo third thread!!" << std::endl;
        std::thread t3(load_cost, temp, all_intersections, turn_penalty, 3);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 3 * division, all_intersections.begin() + 4 * division);
        std::thread t4(load_cost, temp, all_intersections, turn_penalty, 4);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 4 * division, all_intersections.begin() + 5 * division);
        std::thread t5(load_cost, temp, all_intersections, turn_penalty, 5);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 5 * division, all_intersections.begin() + 6 * division);
        std::thread t6(load_cost, temp, all_intersections, turn_penalty, 6);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 6 * division, all_intersections.begin() + 7 * division);
        std::thread t7(load_cost, temp, all_intersections, turn_penalty, 7);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 7 * division, all_intersections.begin() + 8 * division);
        std::thread t8(load_cost, temp, all_intersections, turn_penalty, 8);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 8 * division, all_intersections.begin() + 9 * division);
        std::thread t9(load_cost, temp, all_intersections, turn_penalty, 9);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 9 * division, all_intersections.end());
        std::thread t10(load_cost, temp, all_intersections, turn_penalty, 10);
        temp.clear();

        t1.join();
        //std::cout << "1 thread finished !!" << std::endl;
        t2.join();
        //std::cout << "2 thread finished !!" << std::endl;
        t3.join();
        //std::cout << "3 thread finished !!" << std::endl;
        t4.join();
        //std::cout << "4 thread finished !!" << std::endl;
        t5.join();
        //std::cout << "5 thread finished !!" << std::endl;
        t6.join();
        //std::cout << "6 thread finished !!" << std::endl;
        t7.join();
        t8.join();
        t9.join();
        t10.join();
    }
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    else if (intersection_size >= 50)
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    {
        int division = intersection_size / 5;
        division--;

        std::vector<unsigned> temp;

        temp.insert(temp.end(), all_intersections.begin() + 0 * division, all_intersections.begin() + 1 * division);
        //std::cout << "leggo first thread!!" << std::endl;
        std::thread t1(load_cost, temp, all_intersections, turn_penalty, 1);
        temp.clear();
        //
        //
        temp.insert(temp.end(), all_intersections.begin() + 1 * division, all_intersections.begin() + 2 * division);
        //std::cout << "leggo second thread!!" << std::endl;
        std::thread t2(load_cost, temp, all_intersections, turn_penalty, 2);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 2 * division, all_intersections.begin() + 3 * division);
        //std::cout << "leggo third thread!!" << std::endl;
        std::thread t3(load_cost, temp, all_intersections, turn_penalty, 3);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 3 * division, all_intersections.begin() + 4 * division);
        std::thread t4(load_cost, temp, all_intersections, turn_penalty, 4);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 4 * division, all_intersections.begin() + 5 * division);
        std::thread t5(load_cost, temp, all_intersections, turn_penalty, 5);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 5 * division, all_intersections.end());
        std::thread t6(load_cost, temp, all_intersections, turn_penalty, 6);
        temp.clear();

        t1.join();
        //std::cout << "1 thread finished !!" << std::endl;
        t2.join();
        //std::cout << "2 thread finished !!" << std::endl;
        t3.join();
        //std::cout << "3 thread finished !!" << std::endl;
        t4.join();
        //std::cout << "4 thread finished !!" << std::endl;
        t5.join();
        //std::cout << "5 thread finished !!" << std::endl;
        t6.join();
        //std::cout << "6 thread finished !!" << std::endl;

    }
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    else if (intersection_size >= 20)
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
        /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    {
        int division = intersection_size / 2;
        division--;

        std::vector<unsigned> temp;

        temp.insert(temp.end(), all_intersections.begin() + 0 * division, all_intersections.begin() + 1 * division);
        //std::cout << "leggo first thread!!" << std::endl;
        std::thread t1(load_cost, temp, all_intersections, turn_penalty, 1);
        temp.clear();
        //
        //
        temp.insert(temp.end(), all_intersections.begin() + 1 * division, all_intersections.begin() + 2 * division);
        //std::cout << "leggo second thread!!" << std::endl;
        std::thread t2(load_cost, temp, all_intersections, turn_penalty, 2);
        temp.clear();

        temp.insert(temp.end(), all_intersections.begin() + 2 * division, all_intersections.end());
        //std::cout << "leggo third thread!!" << std::endl;
        std::thread t3(load_cost, temp, all_intersections, turn_penalty, 3);
        temp.clear();

        t1.join();
        //std::cout << "1 thread finished !!" << std::endl;
        t2.join();
        //std::cout << "2 thread finished !!" << std::endl;
        t3.join();

    }
    else
    {
        for (unsigned i = 0; i < all_intersections.size(); ++i)
        {
            intersection_package_info[all_intersections[i]].cost_delivery = multi_destination_Dijkstra(all_intersections[i], all_intersections, turn_penalty, -1);

        }
    }

    /*########################################################################*/
    /*##                        Debug output                                ##*/
    /*########################################################################*/

    //
    //    std::cout << "legal: " << deliveries_legality.size() << std::endl;
    //
    //    for (unsigned i = 0; i < deliveries_legality.size(); ++i)
    //    {
    //        std::cout << "===== package: " << i << " =====" << std::endl;
    //        std::cout << "drop off status: " << deliveries_legality[i].dropoff << std::endl;
    //        std::cout << "pick up status: " << deliveries_legality[i].pickup << std::endl;
    //        std::cout << std::endl;
    //    }
    //
    //
    //        std::cout << "package info: " << intersection_package_info.size() << std::endl;
    //    
    //        for (auto iter = intersection_package_info.begin(); iter != intersection_package_info.end(); ++iter)
    //        {
    //            std::cout << "intersection: " << iter->first << std::endl;
    //            for (unsigned i = 0; i < iter->second.dropoff.size(); ++i)
    //            {
    //                std::cout << "package to be dropped off: " << iter->second.dropoff[i] << std::endl;
    //            }
    //            for (unsigned i = 0; i < iter->second.pickup.size(); ++i)
    //            {
    //                std::cout << "package to be picked up: " << iter->second.pickup[i] << std::endl;
    //            }
    //    
    //            std::cout << std::endl;
    //        }
    //std::cout << "load end" << std::endl;

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
    std::cout << "load time: " << wallClock.count() << "s" << std::endl;

    //    std::cout << "depot size: " << intersection_package_info[0].cost_depot.size() << std::endl;
    //    std::cout << "delivery size: " << intersection_package_info[0].cost_delivery.size() << std::endl;
    //
    //    for (auto iter = intersection_package_info.begin(); iter != intersection_package_info.end(); ++iter)
    //    {
    //        std::cout << std::endl;
    //        std::cout << "===== delivery cost: =====" << std::endl;
    //        for (unsigned i = 0; i < iter->second.cost_delivery.size(); ++i)
    //        {
    //            std::cout << "intersection number: " << iter->second.cost_delivery[i].intersection_index << std::endl;
    //            std::cout << iter->second.cost_delivery[i].cost << std::endl;
    //
    //        }
    //
    //        std::cout << "===== depot cost: =====" << std::endl;
    //        for (unsigned i = 0; i < iter->second.cost_depot.size(); ++i)
    //        {
    //            std::cout << "intersection number: " << iter->second.cost_depot[i].intersection_index << std::endl;
    //            std::cout << iter->second.cost_depot[i].cost << std::endl;
    //        }
    //        std::cout << std::endl;
    //    }

}

void load_cost(std::vector<unsigned> sources, std::vector<unsigned> destinations, const float turn_penalty, int inThread)
{

    for (unsigned i = 0; i < sources.size(); ++i)
    {

        intersection_package_info[sources[i]].cost_delivery = multi_destination_Dijkstra(sources[i], destinations, turn_penalty, inThread);

    }
}

// don't need to check for empty destination vector

std::vector<closest_info> multi_destination_Dijkstra(const unsigned intersect_id_start, std::vector<unsigned> destinations, const float turn_penalty, int inThread)
{

    //std::cout << "dijk start" << std::endl;

    std::vector<closest_info> closest_intersection;
    std::vector<unsigned> path;
    std::vector<unsigned> destination_queue;

    unsigned destination_counter = 0;

    destination_queue = destinations;

    // Load all the intersections into the desired vector
    for (unsigned i = 0; i < destinations.size(); i++)
    {
        closest_info tempInfo;
        tempInfo.intersection_index = destinations[i];
        closest_intersection.push_back(tempInfo);
    }

    // reset all heuristics
    if (inThread == 1)
    {
        for (unsigned i = 0; i < intersectionCells1.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells1[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells1[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells1[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells1[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells1[thisConnection].F)
                    {
                        intersectionCells1[thisConnection].F = F;
                        intersectionCells1[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells1[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells1[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {
                        path.push_back(intersectionCells1[navigator].parent_street_segment);
                        navigator = intersectionCells1[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 2)
    {
        for (unsigned i = 0; i < intersectionCells2.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells2[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells2[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells2[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells2[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells2[thisConnection].F)
                    {
                        intersectionCells2[thisConnection].F = F;
                        intersectionCells2[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells2[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells2[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells2[navigator].parent_street_segment);
                        navigator = intersectionCells2[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 3)
    {
        for (unsigned i = 0; i < intersectionCells3.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells3[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells3[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells3[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells3[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells3[thisConnection].F)
                    {
                        intersectionCells3[thisConnection].F = F;
                        intersectionCells3[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells3[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells3[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells3[navigator].parent_street_segment);
                        navigator = intersectionCells3[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 4)
    {
        for (unsigned i = 0; i < intersectionCells4.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells4[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells4[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells4[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells4[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells4[thisConnection].F)
                    {
                        intersectionCells4[thisConnection].F = F;
                        intersectionCells4[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells4[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells4[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells4[navigator].parent_street_segment);
                        navigator = intersectionCells4[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 5)
    {
        for (unsigned i = 0; i < intersectionCells5.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells5[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells5[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells5[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells5[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells5[thisConnection].F)
                    {
                        intersectionCells5[thisConnection].F = F;
                        intersectionCells5[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells5[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells5[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells5[navigator].parent_street_segment);
                        navigator = intersectionCells5[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 6)
    {
        for (unsigned i = 0; i < intersectionCells6.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells6[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells6[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells6[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells6[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells6[thisConnection].F)
                    {
                        intersectionCells6[thisConnection].F = F;
                        intersectionCells6[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells6[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells6[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells6[navigator].parent_street_segment);
                        navigator = intersectionCells6[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 7)
    {
        for (unsigned i = 0; i < intersectionCells7.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells7[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells7[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells7[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells7[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells7[thisConnection].F)
                    {
                        intersectionCells7[thisConnection].F = F;
                        intersectionCells7[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells7[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells7[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells7[navigator].parent_street_segment);
                        navigator = intersectionCells7[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 8)
    {
        for (unsigned i = 0; i < intersectionCells8.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells8[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells8[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells8[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells8[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells8[thisConnection].F)
                    {
                        intersectionCells8[thisConnection].F = F;
                        intersectionCells8[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells8[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells8[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells8[navigator].parent_street_segment);
                        navigator = intersectionCells8[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 9)
    {
        for (unsigned i = 0; i < intersectionCells9.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells9[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells9[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells9[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells9[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells9[thisConnection].F)
                    {
                        intersectionCells9[thisConnection].F = F;
                        intersectionCells9[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells9[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells9[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells9[navigator].parent_street_segment);
                        navigator = intersectionCells9[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 10)
    {
        for (unsigned i = 0; i < intersectionCells10.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells10[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells10[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells10[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells10[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells10[thisConnection].F)
                    {
                        intersectionCells10[thisConnection].F = F;
                        intersectionCells10[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells10[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells10[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells10[navigator].parent_street_segment);
                        navigator = intersectionCells10[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 11)
    {
        for (unsigned i = 0; i < intersectionCells11.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells11[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells11[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells11[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells11[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells11[thisConnection].F)
                    {
                        intersectionCells11[thisConnection].F = F;
                        intersectionCells11[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells11[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells11[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells11[navigator].parent_street_segment);
                        navigator = intersectionCells11[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 12)
    {
        for (unsigned i = 0; i < intersectionCells12.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells12[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells12[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells12[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells12[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells12[thisConnection].F)
                    {
                        intersectionCells12[thisConnection].F = F;
                        intersectionCells12[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells12[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells12[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells12[navigator].parent_street_segment);
                        navigator = intersectionCells12[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 13)
    {
        for (unsigned i = 0; i < intersectionCells13.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells13[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells13[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells13[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells13[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells13[thisConnection].F)
                    {
                        intersectionCells13[thisConnection].F = F;
                        intersectionCells13[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells13[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells13[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells13[navigator].parent_street_segment);
                        navigator = intersectionCells13[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 14)
    {
        for (unsigned i = 0; i < intersectionCells14.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells14[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells14[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells14[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells14[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells14[thisConnection].F)
                    {
                        intersectionCells14[thisConnection].F = F;
                        intersectionCells14[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells14[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells14[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells14[navigator].parent_street_segment);
                        navigator = intersectionCells14[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 15)
    {
        for (unsigned i = 0; i < intersectionCells15.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells15[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells15[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells15[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells15[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells15[thisConnection].F)
                    {
                        intersectionCells15[thisConnection].F = F;
                        intersectionCells15[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells15[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells15[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells15[navigator].parent_street_segment);
                        navigator = intersectionCells15[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else if (inThread == 16)
    {
        for (unsigned i = 0; i < intersectionCells16.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells16[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells16[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells16[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells16[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells16[thisConnection].F)
                    {
                        intersectionCells16[thisConnection].F = F;
                        intersectionCells16[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells16[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells16[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells16[navigator].parent_street_segment);
                        navigator = intersectionCells16[navigator].parent_intersection;
                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }
    }
    else
    {
        for (unsigned i = 0; i < intersectionCells.size(); i++)
        {
            //std::lock_guard<std::mutex> guard(wrLock);
            intersectionCells[i].F = std::numeric_limits<float>::infinity();
        }
        intersectionCells[intersect_id_start].F = 0;

        std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

        queue.push(intersectionCells[intersect_id_start]);

        // continue checking until every intersection is checked
        //    std::cout << "===== multiDijkstra start =====" << std::endl;
        while (!queue.empty())
        {

            SearchCell currentCell = queue.top();

            // pop the top of the queue, intersection that is visited
            queue.pop();

            if (currentCell.F <= intersectionCells[currentCell.id].F)
            {
                for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
                {
                    unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                    double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                    if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                    {

                        F += turn_penalty;

                    }

                    // a better path is found
                    if (F < intersectionCells[thisConnection].F)
                    {
                        intersectionCells[thisConnection].F = F;
                        intersectionCells[thisConnection].parent_intersection = currentCell.id;
                        intersectionCells[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                        queue.push(intersectionCells[thisConnection]);
                    }
                }
            }

            // path is found
            for (unsigned i = 0; i < destinations.size(); ++i)
            {
                path.clear();

                if (currentCell.id == destinations[i])
                {
                    destination_counter++;

                    unsigned navigator = destinations[i];
                    while (navigator != intersect_id_start)
                    {

                        path.push_back(intersectionCells[navigator].parent_street_segment);
                        navigator = intersectionCells[navigator].parent_intersection;

                    }

                    std::reverse(path.begin(), path.end());

                    //                std::cout << "path size: " << path.size() << std::endl;

                    float cost = compute_path_travel_time(path, turn_penalty);

                    for (unsigned j = 0; j < closest_intersection.size(); ++j)
                    {
                        if (destinations[i] == closest_intersection[j].intersection_index)
                        {

                            closest_intersection[j].cost = cost;
                            closest_intersection[j].segment_path = path;

                        }
                    }

                    if (destination_counter == destinations.size())

                        break;
                }

            }
        }

    }

    //    std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;
    //
    //    queue.push(intersectionCells[intersect_id_start]);
    //
    //    // continue checking until every intersection is checked
    //    //    std::cout << "===== multiDijkstra start =====" << std::endl;
    //    while (!queue.empty())
    //    {
    //
    //        SearchCell currentCell = queue.top();
    //
    //        // pop the top of the queue, intersection that is visited
    //        queue.pop();
    //
    //        if (currentCell.F <= intersectionCells[currentCell.id].F)
    //        {
    //            for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
    //            {
    //                unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
    //                double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
    //                if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
    //                {
    //
    //                    F += turn_penalty;
    //
    //                }
    //
    //                // a better path is found
    //                if (F < intersectionCells[thisConnection].F)
    //                {
    //                    intersectionCells[thisConnection].F = F;
    //                    intersectionCells[thisConnection].parent_intersection = currentCell.id;
    //                    intersectionCells[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
    //                    queue.push(intersectionCells[thisConnection]);
    //                }
    //            }
    //        }
    //
    //        // path is found
    //        for (unsigned i = 0; i < destinations.size(); ++i)
    //        {
    //            bool flag = true;
    //
    //            if (currentCell.id == destinations[i])
    //            {
    //                destination_counter++;
    //
    //                unsigned navigator = destinations[i];
    //                while (navigator != intersect_id_start)
    //                {
    //
    //                    if (navigator >= intersectionCells.size())
    //                        flag = false;
    //
    //                    path.push_back(intersectionCells[navigator].parent_street_segment);
    //                    navigator = intersectionCells[navigator].parent_intersection;
    //                }
    //
    //                std::reverse(path.begin(), path.end());
    //
    //                //                std::cout << "path size: " << path.size() << std::endl;
    //
    //                float cost = compute_path_travel_time(path, turn_penalty);
    //
    //                for (unsigned j = 0; j < closest_intersection.size(); ++j)
    //                {
    //                    if (destinations[i] == closest_intersection[j].intersection_index)
    //                    {
    //                        
    //                        closest_intersection[j].cost = cost;
    //
    //                    }
    //                }
    //
    //                if (destination_counter == destinations.size())
    //
    //                    break;
    //            }
    //
    //        }
    //    }
    //    std::cout << "===== multiDijkstra end =====" << std::endl;
    // sort the vector from closest to furthest
    std::sort(closest_intersection.begin(), closest_intersection.end(), myLeast());

    //        for (unsigned i = 0; i < closest_intersection.size(); ++i)
    //        {
    //            std::cout << "Intersection ID: " << closest_intersection[i].intersection_index << std::endl;
    //            std::cout << "cost: " << closest_intersection[i].cost << std::endl;
    //            std::cout << std::endl;
    //    
    //        }
    return closest_intersection;
}

//find closest legal intersection

unsigned closest_intersection(unsigned intersection_index)
{
    //std::cout << "closest intersection start" << std::endl;

    std::vector<closest_info> ranked_intersection;

    unsigned intersectionIndex;

    ranked_intersection = intersection_package_info[intersection_index].cost_delivery;

    for (unsigned i = 0; i < ranked_intersection.size(); i++)
    {
        intersectionIndex = ranked_intersection[i].intersection_index;
        if ((check_legality(intersectionIndex) == true) && (ranked_intersection[i].cost != 0))
        {
            //std::cout<<intersectionIndex<<std::endl;
            return intersectionIndex;
        }
    }
    //std::cout << "closest intersection end" << std::endl;
}

/*############################################################################*/
/*##                              GOOD CODE                                 ##*/

/*############################################################################*/

unsigned closest_intersection2(unsigned intersection_index, const float turn_penalty)
{

    std::vector<closest_info> ranked_intersection;
    std::vector<unsigned> all_intersection;
    std::unordered_map<unsigned, package_info> intersection_package_info_temp = intersection_package_info;
    unsigned intersectionIndex;

    intersection_package_info_temp.erase(intersection_index);
    for (auto iter = intersection_package_info_temp.begin(); iter != intersection_package_info_temp.end(); iter++)
    {
        intersectionIndex = iter->first;
        if (check_legality(intersectionIndex) == true)
        {
            all_intersection.push_back(intersectionIndex);
        }
    }

    ranked_intersection = multi_destination_Dijkstra(intersection_index, all_intersection, turn_penalty, 10);

    for (unsigned i = 0; i < ranked_intersection.size(); i++)
    {
        intersectionIndex = ranked_intersection[i].intersection_index;
        if (check_legality(intersectionIndex) == true)
        {
            //std::cout << intersectionIndex << std::endl;
            return intersectionIndex;
        }
    }

}

/*############################################################################*/
/*############################################################################*/
/*############################################################################*/

//check if the intersection is legal to be visited

bool check_legality(unsigned intersectionIndex)
{

    //std::cout << "legal start" << std::endl;

    std::vector<unsigned> pickup_package;
    std::vector<unsigned> dropoff_package;
    unsigned packageIndex;

    pickup_package = intersection_package_info[intersectionIndex].pickup;
    dropoff_package = intersection_package_info[intersectionIndex].dropoff;

    for (unsigned i = 0; i < pickup_package.size(); i++)
    {
        packageIndex = pickup_package[i];
        if (deliveries_legality[packageIndex].pickup == false)
            return true;
    }
    for (unsigned i = 0; i < dropoff_package.size(); i++)
    {
        packageIndex = dropoff_package[i];
        if ((deliveries_legality[packageIndex].dropoff == false)&&(deliveries_legality[packageIndex].pickup == true))
            return true;
    }

    //std::cout << "legal end" << std::endl;

    return false;

}

void visit_intersection(unsigned intersectionId)
{
    //std::cout << "visit start" << std::endl;

    std::vector<unsigned> pickup_package;
    std::vector<unsigned> dropoff_package;
    unsigned packageIndex;

    pickup_package = intersection_package_info[intersectionId].pickup;
    dropoff_package = intersection_package_info[intersectionId].dropoff;

    for (unsigned i = 0; i < pickup_package.size(); i++)
    {
        packageIndex = pickup_package[i];
        if (deliveries_legality[packageIndex].pickup == false)
        {
            deliveries_legality[packageIndex].pickup = true;
        }
    }
    for (unsigned i = 0; i < dropoff_package.size(); i++)
    {
        packageIndex = dropoff_package[i];
        if ((deliveries_legality[packageIndex].dropoff == false) && (deliveries_legality[packageIndex].pickup == true))
        {
            deliveries_legality[packageIndex].dropoff = true;
        }
    }
    //std::cout << "visit end" << std::endl;

}

bool check_done_delivery()
{
    //std::cout << "check done  start" << std::endl;
    for (unsigned i = 0; i < deliveries_legality.size(); i++)
    {
        if (deliveries_legality[i].pickup == false)
            return false;
        if (deliveries_legality[i].dropoff == false)
            return false;
    }
    //std::cout << "check done end" << std::endl;
    return true;

}

unsigned closest_depot(unsigned intersection_index, std::vector<unsigned> depots, const float turn_penalty)
{
    //std::cout << "closest depot depot" << std::endl;
    std::vector<closest_info> ranked_depots;
    ranked_depots = multi_destination_Dijkstra(intersection_index, depots, turn_penalty, 10);

    unsigned return_depot = ranked_depots[0].intersection_index;
    std::vector<unsigned> segments_temp;
    bool found_depot = false;
    unsigned count = 0;

    while (found_depot == false && ranked_depots.size() > 1)
    {
        segments_temp = find_intersection_street_segments(ranked_depots[count].intersection_index);
        if (segments_temp.size() > 1)
        {
            return_depot = ranked_depots[count].intersection_index;
            found_depot = true;
        }
        count++;
    }
    //std::cout << "closest depot end" << std::endl;

    return return_depot;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

void two_opt_swapABC(std::vector<unsigned> path, const float turn_penalty, float best_cost)
{
    //    std::cout << "thread 1 start" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    bool timeOut = false;
    float current_cost = best_cost;
    std::vector<unsigned> current_path = path;
    std::vector<unsigned> best_path = path;

    for (unsigned i = 2; i < path.size() - 1 && !timeOut; i++)
    {
        for (unsigned j = i + 1; j < path.size() && !timeOut; ++j)
        {

            //            std::cout << "hi thread 1" << std::endl;

            current_path = two_opt_swap(best_path, i, j);

            std::vector<unsigned> swapped_path_1 = sub_ACB(current_path, i, j);
            std::vector<unsigned> swapped_path_2 = sub_BAC(current_path, i, j);
            std::vector<unsigned> swapped_path_3 = sub_BCA(current_path, i, j);
            //            std::vector<unsigned> swapped_path_4 = sub_CAB(current_path, i, j);
            //            std::vector<unsigned> swapped_path_5 = sub_CBA(current_path, i, j);

            float cost = cost_from_intersection1(current_path, turn_penalty);
            float cost1 = cost_from_intersection1(swapped_path_1, turn_penalty);
            float cost2 = cost_from_intersection1(swapped_path_2, turn_penalty);
            float cost3 = cost_from_intersection1(swapped_path_3, turn_penalty);
            //            float cost4 = cost_from_intersection1(swapped_path_4, turn_penalty);
            //            float cost5 = cost_from_intersection1(swapped_path_5, turn_penalty);

            //            std::cout << best_cost << std::endl;
            //            std::cout << cost << std::endl;
            //            std::cout << cost1 << std::endl;
            //            std::cout << cost2 << std::endl;
            //            std::cout << cost3 << std::endl;
            //            std::cout << cost4 << std::endl;
            //            std::cout << cost5 << std::endl;

            if (cost < current_cost && check_path_legality(current_path))
            {
                best_path = current_path;
                current_cost = cost;
            }

            if (cost1 < current_cost && check_path_legality(swapped_path_1))
            {
                best_path = swapped_path_1;
                current_cost = cost1;
            }

            if (cost2 < current_cost && check_path_legality(swapped_path_2))
            {
                best_path = swapped_path_2;
                current_cost = cost2;
            }
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
            //            std::cout << wallClock.count() << " === 1" << std::endl;
            if (wallClock.count() > 0.9 * TIME_LIMIT)
            {
                timeOut = true;
                break;
            }
            if (cost3 < current_cost && check_path_legality(swapped_path_3))
            {
                best_path = swapped_path_3;
                current_cost = cost3;
            }
            //
            //            if (cost4 < current_cost && check_path_legality(swapped_path_4))
            //            {
            //                best_path = swapped_path_4;
            //                current_cost = cost4;
            //            }
            //
            //            if (cost5 < current_cost && check_path_legality(swapped_path_5))
            //            {
            //                best_path = swapped_path_5;
            //                current_cost = cost5;
            //            }


            current_path = check_global_best(best_path, current_cost);
            current_cost = global_best_cost;
            currentTime = std::chrono::high_resolution_clock::now();
            wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
            //            std::cout << wallClock.count() << " === 1" << std::endl;
            if (wallClock.count() > 0.9 * TIME_LIMIT)
            {
                timeOut = true;
                break;
            }

        }
    }
    //    std::cout << "thread 1 end" << std::endl;
}

void two_opt_swapA_BC(std::vector<unsigned> path, const float turn_penalty, float best_cost)
{
    //    std::cout << "thread 2 start" << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();
    bool timeOut = false;

    float current_cost = best_cost;

    std::vector<unsigned> best_path = path;

    std::vector<unsigned> current_path = path;

    for (unsigned i = 2; i < path.size() - 1 && !timeOut; i++)
    {
        for (unsigned j = i + 1; j < path.size() && !timeOut; ++j)
        {
            //            std::cout << "in thread 2" << std::endl;

            current_path = A_BC(current_path, i, j);

            std::vector<unsigned> swapped_path_1 = sub_ACB(current_path, i, j);
            std::vector<unsigned> swapped_path_2 = sub_BAC(current_path, i, j);
            std::vector<unsigned> swapped_path_3 = sub_BCA(current_path, i, j);
            //            std::vector<unsigned> swapped_path_4 = sub_CAB(current_path, i, j);
            //            std::vector<unsigned> swapped_path_5 = sub_CBA(current_path, i, j);

            float cost = cost_from_intersection2(current_path, turn_penalty);
            float cost1 = cost_from_intersection2(swapped_path_1, turn_penalty);
            float cost2 = cost_from_intersection2(swapped_path_2, turn_penalty);
            float cost3 = cost_from_intersection2(swapped_path_3, turn_penalty);
            //            float cost4 = cost_from_intersection2(swapped_path_4, turn_penalty);
            //            float cost5 = cost_from_intersection2(swapped_path_5, turn_penalty);

            if (cost < current_cost && check_path_legality(current_path))
            {
                best_path = current_path;
                current_cost = cost;
            }

            if (cost1 < current_cost && check_path_legality(swapped_path_1))
            {
                best_path = swapped_path_1;
                current_cost = cost1;
            }

            if (cost2 < current_cost && check_path_legality(swapped_path_2))
            {
                best_path = swapped_path_2;
                current_cost = cost2;
            }
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
            //            std::cout << wallClock.count() << " === 1" << std::endl;
            if (wallClock.count() > 0.9 * TIME_LIMIT)
            {
                timeOut = true;
                break;
            }
            if (cost3 < current_cost && check_path_legality(swapped_path_3))
            {
                best_path = swapped_path_3;
                current_cost = cost3;
            }
            //
            //            if (cost4 < current_cost && check_path_legality(swapped_path_4))
            //            {
            //                best_path = swapped_path_4;
            //                current_cost = cost4;
            //            }
            //
            //            if (cost5 < current_cost && check_path_legality(swapped_path_5))
            //            {
            //                best_path = swapped_path_5;
            //                current_cost = cost5;
            //            }

            current_path = check_global_best(best_path, current_cost);
            current_cost = global_best_cost;
            currentTime = std::chrono::high_resolution_clock::now();
            wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
            //            std::cout << wallClock.count() << " === 2" << std::endl;
            if (wallClock.count() > 0.9 * TIME_LIMIT)
                timeOut = true;

        }
    }
    //    std::cout << "thread 2 end" << std::endl;
}

void two_opt_swapAB_C(std::vector<unsigned> path, const float turn_penalty, float best_cost)
{
    //    std::cout << "thread 3 start" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    bool timeOut = false;
    float current_cost = best_cost;
    std::vector<unsigned> best_path = path;

    std::vector<unsigned> current_path = path;

    for (unsigned i = 2; i < path.size() - 1 && !timeOut; i++)
    {
        for (unsigned j = i + 1; j < path.size() && !timeOut; ++j)
        {
            //            std::cout << "in thread 3" << std::endl;

            current_path = AB_C(current_path, i, j);

            std::vector<unsigned> swapped_path_1 = sub_ACB(current_path, i, j);
            std::vector<unsigned> swapped_path_2 = sub_BAC(current_path, i, j);
            std::vector<unsigned> swapped_path_3 = sub_BCA(current_path, i, j);
            //            std::vector<unsigned> swapped_path_4 = sub_CAB(current_path, i, j);
            //            std::vector<unsigned> swapped_path_5 = sub_CBA(current_path, i, j);

            float cost = cost_from_intersection3(current_path, turn_penalty);
            float cost1 = cost_from_intersection3(swapped_path_1, turn_penalty);
            float cost2 = cost_from_intersection3(swapped_path_2, turn_penalty);
            float cost3 = cost_from_intersection3(swapped_path_3, turn_penalty);
            //            float cost4 = cost_from_intersection3(swapped_path_4, turn_penalty);
            //            float cost5 = cost_from_intersection3(swapped_path_5, turn_penalty);

            if (cost < current_cost && check_path_legality(current_path))
            {
                best_path = current_path;
                current_cost = cost;
            }

            if (cost1 < current_cost && check_path_legality(swapped_path_1))
            {
                best_path = swapped_path_1;
                current_cost = cost1;
            }

            if (cost2 < current_cost && check_path_legality(swapped_path_2))
            {
                best_path = swapped_path_2;
                current_cost = cost2;
            }
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
            //            std::cout << wallClock.count() << " === 1" << std::endl;
            if (wallClock.count() > 0.9 * TIME_LIMIT)
            {
                timeOut = true;
                break;
            }
            if (cost3 < current_cost && check_path_legality(swapped_path_3))
            {
                best_path = swapped_path_3;
                current_cost = cost3;
            }
            //
            //            if (cost4 < current_cost && check_path_legality(swapped_path_4))
            //            {
            //                best_path = swapped_path_4;
            //                current_cost = cost4;
            //            }
            //
            //
            //            if (cost5 < current_cost && check_path_legality(swapped_path_5))
            //            {
            //                best_path = swapped_path_5;
            //                current_cost = cost5;
            //            }

            current_path = check_global_best(best_path, current_cost);
            current_cost = global_best_cost;

            currentTime = std::chrono::high_resolution_clock::now();
            wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
            //            std::cout << wallClock.count() << " === 3" << std::endl;
            if (wallClock.count() > 0.9 * TIME_LIMIT)
                timeOut = true;

        }
    }
    //    std::cout << "thread 3 end" << std::endl;
}

void three_opt_swapABCD(std::vector<unsigned> path, const float turn_penalty, float best_cost)
{
    //    std::cout << "thread 4 start" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    bool timeOut = false;
    float current_cost = best_cost;
    std::vector<unsigned> best_path = path;

    std::vector<unsigned> current_path = path;

    for (unsigned i = 2; i < path.size() - 2 && !timeOut; ++i)
    {
        for (unsigned j = i + 1; j < path.size() - 1 && !timeOut; ++j)
        {
            for (unsigned k = j + 1; k < path.size() && !timeOut; ++k)
            {
                //                std::cout << "in thread 4" << std::endl;

                //current_path = ABDC(current_path, i, j, k);
                std::vector<unsigned> swapped_path_1 = ACBD(current_path, i, j, k);
                std::vector<unsigned> swapped_path_2 = ACDB(current_path, i, j, k);
                std::vector<unsigned> swapped_path_3 = ADBC(current_path, i, j, k);
                std::vector<unsigned> swapped_path_4 = ADCB(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_5 = BACD(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_6 = BADC(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_7 = BCAD(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_8 = BCDA(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_9 = BDAC(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_10 = BDCA(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_11 = CABD(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_12 = CADB(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_13 = CBAD(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_14 = CBDA(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_15 = CDAB(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_16 = CDBA(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_17 = DABC(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_18 = DACB(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_19 = DBAC(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_20 = DBCA(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_21 = DCAB(current_path, i, j, k);
                //                std::vector<unsigned> swapped_path_22 = DCBA(current_path, i, j, k);

                //float cost = cost_from_intersection(current_path, turn_penalty);
                float cost1 = cost_from_intersection4(swapped_path_1, turn_penalty);
                float cost2 = cost_from_intersection4(swapped_path_2, turn_penalty);
                float cost3 = cost_from_intersection4(swapped_path_3, turn_penalty);
                float cost4 = cost_from_intersection4(swapped_path_4, turn_penalty);
                //                float cost5 = cost_from_intersection4(swapped_path_5, turn_penalty);
                //                float cost6 = cost_from_intersection4(swapped_path_6, turn_penalty);
                //                float cost7 = cost_from_intersection4(swapped_path_7, turn_penalty);
                //                float cost8 = cost_from_intersection4(swapped_path_8, turn_penalty);
                //                float cost9 = cost_from_intersection4(swapped_path_9, turn_penalty);
                //                float cost10 = cost_from_intersection4(swapped_path_10, turn_penalty);
                //                float cost11 = cost_from_intersection4(swapped_path_11, turn_penalty);
                //                float cost12 = cost_from_intersection4(swapped_path_12, turn_penalty);
                //                float cost13 = cost_from_intersection4(swapped_path_13, turn_penalty);
                //                float cost14 = cost_from_intersection4(swapped_path_14, turn_penalty);
                //                float cost15 = cost_from_intersection4(swapped_path_15, turn_penalty);
                //                float cost16 = cost_from_intersection(swapped_path_16, turn_penalty);
                //                float cost17 = cost_from_intersection(swapped_path_17, turn_penalty);
                //                float cost18 = cost_from_intersection(swapped_path_18, turn_penalty);
                //                float cost19 = cost_from_intersection(swapped_path_19, turn_penalty);
                //                float cost20 = cost_from_intersection(swapped_path_20, turn_penalty);
                //                float cost21 = cost_from_intersection(swapped_path_21, turn_penalty);
                //                float cost22 = cost_from_intersection(swapped_path_22, turn_penalty);
                //                if (cost < best_cost && check_path_legality(current_path))
                //                {
                //                    best_path = current_path;
                //                    best_cost = cost;
                //                }

                if (cost1 < current_cost && check_path_legality(swapped_path_1))
                {
                    best_path = swapped_path_1;
                    current_cost = cost1;
                }

                if (cost2 < current_cost && check_path_legality(swapped_path_2))
                {
                    best_path = swapped_path_2;
                    current_cost = cost2;
                }

                if (cost3 < current_cost && check_path_legality(swapped_path_3))
                {
                    best_path = swapped_path_3;
                    current_cost = cost3;
                }
                auto currentTime = std::chrono::high_resolution_clock::now();
                auto wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
                //            std::cout << wallClock.count() << " === 1" << std::endl;
                if (wallClock.count() > 0.9 * TIME_LIMIT)
                {
                    timeOut = true;
                    break;
                }
                if (cost4 < current_cost && check_path_legality(swapped_path_4))
                {
                    best_path = swapped_path_4;
                    current_cost = cost4;
                }
                //
                //                if (cost5 < current_cost && check_path_legality(swapped_path_5))
                //                {
                //                    best_path = swapped_path_5;
                //                    current_cost = cost5;
                //                }
                //                if (cost6 < current_cost && check_path_legality(swapped_path_6))
                //                {
                //                    best_path = swapped_path_6;
                //                    current_cost = cost6;
                //                }
                //                currentTime = std::chrono::high_resolution_clock::now();
                //                wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
                //                //            std::cout << wallClock.count() << " === 1" << std::endl;
                //                if (wallClock.count() > 0.9 * TIME_LIMIT)
                //                {
                //                    timeOut = true;
                //                    break;
                //                }
                //                if (cost7 < current_cost && check_path_legality(swapped_path_7))
                //                {
                //                    best_path = swapped_path_7;
                //                    current_cost = cost7;
                //                }
                //
                //                if (cost8 < current_cost && check_path_legality(swapped_path_8))
                //                {
                //                    best_path = swapped_path_8;
                //                    current_cost = cost8;
                //                }
                //
                //                if (cost9 < best_cost && check_path_legality(swapped_path_9))
                //                {
                //                    best_path = swapped_path_9;
                //                    best_cost = cost9;
                //                }
                //
                //                if (cost10 < best_cost && check_path_legality(swapped_path_10))
                //                {
                //                    best_path = swapped_path_10;
                //                    best_cost = cost10;
                //                }
                //                if (cost11 < best_cost && check_path_legality(swapped_path_11))
                //                {
                //                    best_path = swapped_path_11;
                //                    best_cost = cost11;
                //                }
                //
                //                if (cost12 < best_cost && check_path_legality(swapped_path_12))
                //                {
                //                    best_path = swapped_path_12;
                //                    best_cost = cost12;
                //                }
                //
                //                if (cost13 < best_cost && check_path_legality(swapped_path_13))
                //                {
                //                    best_path = swapped_path_13;
                //                    best_cost = cost13;
                //                }
                //
                //                if (cost14 < best_cost && check_path_legality(swapped_path_14))
                //                {
                //                    best_path = swapped_path_14;
                //                    best_cost = cost14;
                //                }
                //
                //                if (cost15 < best_cost && check_path_legality(swapped_path_15))
                //                {
                //                    best_path = swapped_path_15;
                //                    best_cost = cost15;
                //                }
                //                if (cost16 < best_cost)
                //                {
                //                    best_path = swapped_path_16;
                //                    best_cost = cost16;
                //                }
                //
                //                if (cost17 < best_cost)
                //                {
                //                    best_path = swapped_path_17;
                //                    best_cost = cost17;
                //                }
                //
                //                if (cost18 < best_cost)
                //                {
                //                    best_path = swapped_path_18;
                //                    best_cost = cost18;
                //                }
                //
                //                if (cost19 < best_cost)
                //                {
                //                    best_path = swapped_path_19;
                //                    best_cost = cost19;
                //                }
                //
                //                if (cost20 < best_cost)
                //                {
                //                    best_path = swapped_path_20;
                //                    best_cost = cost20;
                //                }
                //                if (cost21 < best_cost)
                //                {
                //                    best_path = swapped_path_21;
                //                    best_cost = cost21;
                //                }
                //                if (cost22 < best_cost)
                //                {
                //                    best_path = swapped_path_22;
                //                    best_cost = cost22;
                //                }

                current_path = check_global_best(best_path, current_cost);
                current_cost = global_best_cost;
                currentTime = std::chrono::high_resolution_clock::now();
                wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
                //                std::cout << wallClock.count() << " === 4" << std::endl;
                if (wallClock.count() > 0.9 * TIME_LIMIT)
                    timeOut = true;

            }
        }
    }
    //    std::cout << "thread 4 end" << std::endl;
}

void four_opt_swap1(std::vector<unsigned> path, const float turn_penalty, float best_cost)
{
    //    std::cout << "thread 5 start" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    bool timeOut = false;
    float current_cost = best_cost;
    std::vector<unsigned> best_path = path;

    std::vector<unsigned> current_path = path;

    for (unsigned i = 2; i < path.size() - 3 && !timeOut; ++i)
    {
        for (unsigned j = i + 1; j < path.size() - 2 && !timeOut; ++j)
        {
            for (unsigned k = j + 1; k < path.size() - 1 && !timeOut; ++k)
            {
                for (unsigned l = k + 1; l < path.size() && !timeOut; ++l)
                {
                    //                    std::cout << "in thread 5" << std::endl;

                    //current_path = ABCED(current_path, i, j, k, l);

                    std::vector<unsigned> swapped_path_1 = ABDCE(current_path, i, j, k, l);
                    std::vector<unsigned> swapped_path_2 = ABDEC(current_path, i, j, k, l);
                    std::vector<unsigned> swapped_path_3 = ABECD(current_path, i, j, k, l);
                    std::vector<unsigned> swapped_path_4 = ABEDC(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_5 = ACBDE(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_6 = ACBED(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_7 = ACDBE(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_8 = ACDEB(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_9 = ACEBD(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_10 = ACEDB(current_path, i, j, k, l);

                    //float cost = cost_from_intersection(current_path, turn_penalty);
                    float cost1 = cost_from_intersection5(swapped_path_1, turn_penalty);
                    float cost2 = cost_from_intersection5(swapped_path_2, turn_penalty);
                    float cost3 = cost_from_intersection5(swapped_path_3, turn_penalty);
                    float cost4 = cost_from_intersection5(swapped_path_4, turn_penalty);
                    //                    float cost5 = cost_from_intersection5(swapped_path_5, turn_penalty);
                    //                    float cost6 = cost_from_intersection5(swapped_path_6, turn_penalty);
                    //                    //                    float cost7 = cost_from_intersection5(swapped_path_7, turn_penalty);
                    //                    float cost8 = cost_from_intersection5(swapped_path_8, turn_penalty);
                    //                    float cost9 = cost_from_intersection5(swapped_path_9, turn_penalty);
                    //                    float cost10 = cost_from_intersection5(swapped_path_10, turn_penalty);

                    //                    if (cost < best_cost && check_path_legality(current_path))
                    //                    {
                    //                        best_path = current_path;
                    //                        best_cost = cost;
                    //                    }

                    if (cost1 < current_cost && check_path_legality(swapped_path_1))
                    {
                        best_path = swapped_path_1;
                        current_cost = cost1;
                    }

                    if (cost2 < current_cost && check_path_legality(swapped_path_2))
                    {
                        best_path = swapped_path_2;
                        current_cost = cost2;
                    }

                    if (cost3 < current_cost && check_path_legality(swapped_path_3))
                    {
                        best_path = swapped_path_3;
                        current_cost = cost3;
                    }
                    auto currentTime = std::chrono::high_resolution_clock::now();
                    auto wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
                    //            std::cout << wallClock.count() << " === 1" << std::endl;
                    if (wallClock.count() > 0.9 * TIME_LIMIT)
                    {
                        timeOut = true;
                        break;
                    }
                    if (cost4 < current_cost && check_path_legality(swapped_path_4))
                    {
                        best_path = swapped_path_4;
                        current_cost = cost4;
                    }
                    //
                    //                    if (cost5 < current_cost && check_path_legality(swapped_path_5))
                    //                    {
                    //                        best_path = swapped_path_5;
                    //                        current_cost = cost5;
                    //                    }
                    //                    if (cost6 < current_cost && check_path_legality(swapped_path_6))
                    //                    {
                    //                        best_path = swapped_path_6;
                    //                        current_cost = cost6;
                    //                    }
                    //
                    //                    if (cost7 < best_cost && check_path_legality(swapped_path_7))
                    //                    {
                    //                        best_path = swapped_path_7;
                    //                        best_cost = cost7;
                    //                    }
                    //
                    //                    if (cost8 < best_cost && check_path_legality(swapped_path_8))
                    //                    {
                    //                        best_path = swapped_path_8;
                    //                        best_cost = cost8;
                    //                    }
                    //
                    //                    if (cost9 < best_cost && check_path_legality(swapped_path_9))
                    //                    {
                    //                        best_path = swapped_path_9;
                    //                        best_cost = cost9;
                    //                    }
                    //
                    //                    if (cost10 < best_cost && check_path_legality(swapped_path_10))
                    //                    {
                    //                        best_path = swapped_path_10;
                    //                        best_cost = cost10;
                    //                    }

                    current_path = check_global_best(best_path, current_cost);
                    current_cost = global_best_cost;

                    currentTime = std::chrono::high_resolution_clock::now();
                    wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
                    //                    std::cout << wallClock.count() << " === 5" << std::endl;
                    if (wallClock.count() > 0.9 * TIME_LIMIT)
                        timeOut = true;

                }
            }
        }
    }
    //    std::cout << "thread 5 end" << std::endl;
}

void four_opt_swap2(std::vector<unsigned> path, const float turn_penalty, float best_cost)
{
    //    std::cout << "thread 6 start" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    bool timeOut = false;

    float current_cost = best_cost;

    std::vector<unsigned> best_path = path;
    std::vector<unsigned> current_path = path;

    for (unsigned i = 2; i < path.size() - 3 && !timeOut; ++i)
    {
        for (unsigned j = i + 1; j < path.size() - 2 && !timeOut; ++j)
        {
            for (unsigned k = j + 1; k < path.size() - 1 && !timeOut; ++k)
            {
                for (unsigned l = k + 1; l < path.size() && !timeOut; ++l)
                {

                    //                    std::cout << "in thread 6" << std::endl;

                    current_path = ADBCE(current_path, i, j, k, l);
                    std::vector<unsigned> swapped_path_1 = ADBEC(current_path, i, j, k, l);
                    std::vector<unsigned> swapped_path_2 = ADCBE(current_path, i, j, k, l);
                    std::vector<unsigned> swapped_path_3 = ADCEB(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_4 = ADEBC(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_5 = ADECB(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_6 = AEBCD(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_7 = AEBDC(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_8 = AECBD(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_9 = AECDB(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_10 = AEDBC(current_path, i, j, k, l);
                    //                    std::vector<unsigned> swapped_path_11 = AEDCB(current_path, i, j, k, l);

                    float cost = cost_from_intersection6(current_path, turn_penalty);
                    float cost1 = cost_from_intersection6(swapped_path_1, turn_penalty);
                    float cost2 = cost_from_intersection6(swapped_path_2, turn_penalty);
                    float cost3 = cost_from_intersection6(swapped_path_3, turn_penalty);
                    //                    float cost4 = cost_from_intersection6(swapped_path_4, turn_penalty);
                    //                    float cost5 = cost_from_intersection6(swapped_path_5, turn_penalty);
                    //                    float cost6 = cost_from_intersection6(swapped_path_6, turn_penalty);
                    //                    float cost7 = cost_from_intersection6(swapped_path_7, turn_penalty);
                    //                    float cost8 = cost_from_intersection6(swapped_path_8, turn_penalty);
                    //                    float cost9 = cost_from_intersection6(swapped_path_9, turn_penalty);
                    //                    float cost10 = cost_from_intersection6(swapped_path_10, turn_penalty);
                    //                    float cost11 = cost_from_intersection6(swapped_path_11, turn_penalty);

                    if (cost < current_cost && check_path_legality(current_path))
                    {
                        best_path = current_path;
                        current_cost = cost;
                    }

                    if (cost1 < current_cost && check_path_legality(swapped_path_1))
                    {
                        best_path = swapped_path_1;
                        current_cost = cost1;
                    }

                    if (cost2 < current_cost && check_path_legality(swapped_path_2))
                    {
                        best_path = swapped_path_2;
                        current_cost = cost2;
                    }
                    auto currentTime = std::chrono::high_resolution_clock::now();
                    auto wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
                    //            std::cout << wallClock.count() << " === 1" << std::endl;
                    if (wallClock.count() > 0.9 * TIME_LIMIT)
                    {
                        timeOut = true;
                        break;
                    }
                    if (cost3 < current_cost && check_path_legality(swapped_path_3))
                    {
                        best_path = swapped_path_3;
                        current_cost = cost3;
                    }
                    //
                    //                    if (cost4 < current_cost && check_path_legality(swapped_path_4))
                    //                    {
                    //                        best_path = swapped_path_4;
                    //                        current_cost = cost4;
                    //                    }
                    //
                    //                    if (cost5 < current_cost && check_path_legality(swapped_path_5))
                    //                    {
                    //                        best_path = swapped_path_5;
                    //                        current_cost = cost5;
                    //                    }
                    //                    currentTime = std::chrono::high_resolution_clock::now();
                    //                    wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
                    //                    //            std::cout << wallClock.count() << " === 1" << std::endl;
                    //                    if (wallClock.count() > 0.9 * TIME_LIMIT)
                    //                    {
                    //                        timeOut = true;
                    //                        break;
                    //                    }
                    //                    if (cost6 < current_cost && check_path_legality(swapped_path_6))
                    //                    {
                    //                        best_path = swapped_path_6;
                    //                        current_cost = cost6;
                    //                    }
                    //
                    //                    if (cost7 < current_cost && check_path_legality(swapped_path_7))
                    //                    {
                    //                        best_path = swapped_path_7;
                    //                        current_cost = cost7;
                    //                    }
                    //
                    //                    if (cost8 < best_cost && check_path_legality(swapped_path_8))
                    //                    {
                    //                        best_path = swapped_path_8;
                    //                        best_cost = cost8;
                    //                    }
                    //
                    //                    if (cost9 < best_cost && check_path_legality(swapped_path_9))
                    //                    {
                    //                        best_path = swapped_path_9;
                    //                        best_cost = cost9;
                    //                    }
                    //
                    //                    if (cost10 < best_cost && check_path_legality(swapped_path_10))
                    //                    {
                    //                        best_path = swapped_path_10;
                    //                        best_cost = cost10;
                    //                    }
                    //                    if (cost11 < best_cost && check_path_legality(swapped_path_11))
                    //                    {
                    //                        best_path = swapped_path_11;
                    //                        best_cost = cost11;
                    //                    }

                    current_path = check_global_best(best_path, current_cost);
                    current_cost = global_best_cost;

                    currentTime = std::chrono::high_resolution_clock::now();
                    wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
                    //                    std::cout << wallClock.count() << " === 6" << std::endl;
                    if (wallClock.count() > 0.9 * TIME_LIMIT)
                        timeOut = true;

                }
            }
        }
    }
    //    std::cout << "thread 6 end" << std::endl;
}

std::vector<unsigned> check_global_best(std::vector<unsigned>& current_best_path, float current_best_cost)
{
    std::lock_guard<std::mutex> lock(globalLock);
    //    std::cout << "threadCom curr cost: " << current_best_cost << std::endl;
    //    std::cout << "threadCom global cost: " << global_best_cost << std::endl;
    if (current_best_cost < global_best_cost)
    {
        global_best_cost = current_best_cost;
        global_best_path = current_best_path;
        std::cout << "better cost: " << current_best_cost << std::endl;
        return current_best_path;
    }
    else
    {
        return global_best_path;
    }
}

std::vector<unsigned> two_opt_swap(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{

    //    std::cout << "2 opt function start" << std::endl;

    std::vector<unsigned> swapped_path_intersection;

    // 1. take route[0] to route[i-1] and add them in order to new_route
    for (unsigned k = 0; k < i; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    //std::cout << "first loop correct" << std::endl;

    // 2. take route[i] to route[j] and add them in reverse order to new_route
    unsigned dec = 0;
    for (unsigned k = i; k <= j; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[j - dec]);
        dec++;
    }
    //    std::cout << "second loop correct" << std::endl;

    // 3. take route[j + 1] to end and add them in order to new_route
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    //    //    std::cout << "third loop correct" << std::endl;
    //    //    std::cout << "2 opt function end" << std::endl;
    //    //    std::cout << swapped_path_intersection.size() << std::endl;

    return swapped_path_intersection;
}

//bool check_path_legality(std::vector<unsigned> proposed_path){
//    
//    proposed_path.erase(proposed_path.begin());
//    
//    bool found=true
//    
//    std::vector<unsigned> all_pickup;
//    
//    std::vector<unsigned> pick_temp;
//    std:;vector<unsigned> drop_temp;
//    
//    for(unsigned i=0;i<proposed_path.size()&&found==true;i++){
//        found=false;
//        pick_temp=intersection_package_info[proposed_path[i]].pickup;
//        for(unsigned j=0;j<pick_temp.size();j++){
//            all_pickup.push_back(pick_temp[j]);
//        }
//        drop_temp=intersection_package_info[proposed_path[i[]]]
//        
//    }
//    
//}

bool check_path_legality(std::vector<unsigned> proposed_path)
{

    bool single_legal1, single_legal2;
    bool all_legal = true;
    unsigned intersection_Id;
    std::vector<unsigned> pick;
    std::vector<unsigned> drop;
    std::vector<unsigned> picked_temp;
    std::vector<unsigned> dropped_temp;

    for (unsigned i = 1; (i < proposed_path.size()) && (all_legal == true); i++)
    {

        single_legal1 = false;
        single_legal2 = false;

        intersection_Id = proposed_path[i];
        pick = intersection_package_info[intersection_Id].pickup;
        drop = intersection_package_info[intersection_Id].dropoff;

        // if any package is picked up already then true
        for (unsigned j = 0; j <= i; j++)
        {
            picked_temp = intersection_package_info[proposed_path[j]].pickup;

            for (unsigned m = 0; m < drop.size(); m++)
            {
                for (unsigned n = 0; n < picked_temp.size(); n++)
                {
                    if (picked_temp[n] == drop[m])
                        single_legal1 = true;
                }
            }
        }
        if (drop.size() == 0)
            single_legal1 = true;

        for (unsigned j = i; j < proposed_path.size(); j++)
        {
            dropped_temp = intersection_package_info[proposed_path[j]].dropoff;

            for (unsigned m = 0; m < pick.size(); m++)
            {
                for (unsigned n = 0; n < dropped_temp.size(); n++)
                {
                    if (dropped_temp[n] == pick[m])
                        single_legal2 = true;
                }
            }
        }
        if (pick.size() == 0)
        {
            single_legal2 = true;
        }

        if (single_legal1 == false || single_legal2 == false)
            all_legal = false;
    }
    std::cout << all_legal << std::endl;
    return all_legal;
}

bool optimality(std::vector<unsigned> old_path_intersection, std::vector<unsigned> new_path_intersection, const float turn_penalty, const unsigned& i, const unsigned& j)
{

    float old_cost = 0;
    float new_cost = 0;


    if (i == 0 && j == (new_path_intersection.size() - 1))
    {
        for (unsigned k = i; k <= j - 1; ++k)
        {
            std::vector<unsigned> old_path = find_path_between_intersections(old_path_intersection[k], old_path_intersection[k + 1], turn_penalty);
            old_cost += compute_path_travel_time(old_path, turn_penalty);

            std::vector<unsigned> new_path = find_path_between_intersections(new_path_intersection[k], new_path_intersection[k + 1], turn_penalty);
            new_cost += compute_path_travel_time(new_path, turn_penalty);

        }
    }
    else if (i == 0)
    {
        for (unsigned k = i; k <= j; ++k)
        {
            std::vector<unsigned> old_path = find_path_between_intersections(old_path_intersection[k], old_path_intersection[k + 1], turn_penalty);
            old_cost += compute_path_travel_time(old_path, turn_penalty);

            std::vector<unsigned> new_path = find_path_between_intersections(new_path_intersection[k], new_path_intersection[k + 1], turn_penalty);
            new_cost += compute_path_travel_time(new_path, turn_penalty);

        }
    }
    else if (j == (new_path_intersection.size() - 1))
    {
        for (unsigned k = i - 1; k <= j - 1; ++k)
        {
            std::vector<unsigned> old_path = find_path_between_intersections(old_path_intersection[k], old_path_intersection[k + 1], turn_penalty);
            old_cost += compute_path_travel_time(old_path, turn_penalty);

            std::vector<unsigned> new_path = find_path_between_intersections(new_path_intersection[k], new_path_intersection[k + 1], turn_penalty);
            new_cost += compute_path_travel_time(new_path, turn_penalty);

        }
    }
    else
    {
        for (unsigned k = i - 1; k <= j; ++k)
        {
            std::vector<unsigned> old_path = find_path_between_intersections(old_path_intersection[k], old_path_intersection[k + 1], turn_penalty);
            old_cost += compute_path_travel_time(old_path, turn_penalty);

            std::vector<unsigned> new_path = find_path_between_intersections(new_path_intersection[k], new_path_intersection[k + 1], turn_penalty);
            new_cost += compute_path_travel_time(new_path, turn_penalty);

        }
    }

    if (new_cost < old_cost)
    {
        //std::cout << "better cost: " << new_cost << std::endl;
        //std::cout << "old cost: " << old_cost << std::endl;

        return true;
    }

    return false;
}

float cost_from_intersection(std::vector<unsigned> path, const float turn_penalty)
{

    float cost = 0;

    for (unsigned i = 0; i < path.size() - 1; ++i)
    {

        std::vector<unsigned> tempPath = find_path_between_intersections(path[i], path[i + 1], turn_penalty);
        cost += compute_path_travel_time(tempPath, turn_penalty);

    }

    return cost;
}

float cost_from_intersection1(std::vector<unsigned> path, const float turn_penalty)
{

    float cost = 0;

    for (unsigned i = 0; i < path.size() - 1; ++i)
    {

        std::vector<unsigned> tempPath = find_path_between_intersections1(path[i], path[i + 1], turn_penalty);
        cost += compute_path_travel_time(tempPath, turn_penalty);

    }

    return cost;
}

float cost_from_intersection2(std::vector<unsigned> path, const float turn_penalty)
{

    float cost = 0;

    for (unsigned i = 0; i < path.size() - 1; ++i)
    {

        std::vector<unsigned> tempPath = find_path_between_intersections2(path[i], path[i + 1], turn_penalty);
        cost += compute_path_travel_time(tempPath, turn_penalty);

    }

    return cost;
}

float cost_from_intersection3(std::vector<unsigned> path, const float turn_penalty)
{

    float cost = 0;

    for (unsigned i = 0; i < path.size() - 1; ++i)
    {

        std::vector<unsigned> tempPath = find_path_between_intersections3(path[i], path[i + 1], turn_penalty);
        cost += compute_path_travel_time(tempPath, turn_penalty);

    }

    return cost;
}

float cost_from_intersection4(std::vector<unsigned> path, const float turn_penalty)
{

    float cost = 0;

    for (unsigned i = 0; i < path.size() - 1; ++i)
    {

        std::vector<unsigned> tempPath = find_path_between_intersections4(path[i], path[i + 1], turn_penalty);
        cost += compute_path_travel_time(tempPath, turn_penalty);

    }

    return cost;
}

float cost_from_intersection5(std::vector<unsigned> path, const float turn_penalty)
{

    float cost = 0;

    for (unsigned i = 0; i < path.size() - 1; ++i)
    {

        std::vector<unsigned> tempPath = find_path_between_intersections5(path[i], path[i + 1], turn_penalty);
        cost += compute_path_travel_time(tempPath, turn_penalty);

    }

    return cost;
}

float cost_from_intersection6(std::vector<unsigned> path, const float turn_penalty)
{

    float cost = 0;

    for (unsigned i = 0; i < path.size() - 1; ++i)
    {

        std::vector<unsigned> tempPath = find_path_between_intersections6(path[i], path[i + 1], turn_penalty);
        cost += compute_path_travel_time(tempPath, turn_penalty);

    }

    return cost;
}

//void threaded_forty_eight_sub_swap(std::vector<unsigned> curr_path_intersection, const float turn_penalty, const unsigned& i, const unsigned& j)
//{
//    //    unsigned ABC = 1;
//    //    unsigned AB_C = 2;
//    //    unsigned ABC_ = 3;
//    //    unsigned AB_C_ = 4;
//    //    unsigned A_BC = 5;
//    //    unsigned A_BC_ = 6;
//    //    unsigned A_B_C = 7;
//    //    unsigned A_B_C_ = 8;
//
//    best_sub_path.clear();
//    best_sub_path1.clear();
//    best_sub_path2.clear();
//    best_sub_path3.clear();
//    best_sub_path4.clear();
//    best_sub_path5.clear();
//    best_sub_path6.clear();
//    best_sub_path7.clear();
//
//    best_sub_cost = 0;
//    best_sub_cost1 = 0;
//    best_sub_cost2 = 0;
//    best_sub_cost3 = 0;
//    best_sub_cost4 = 0;
//    best_sub_cost5 = 0;
//    best_sub_cost6 = 0;
//    best_sub_cost7 = 0;
//
//    std::thread t1(forty_eight_sub_swap, 1, curr_path_intersection, turn_penalty, i, j);
//    std::thread t2(forty_eight_sub_swap, 2, curr_path_intersection, turn_penalty, i, j);
//    std::thread t3(forty_eight_sub_swap, 3, curr_path_intersection, turn_penalty, i, j);
//    std::thread t4(forty_eight_sub_swap, 4, curr_path_intersection, turn_penalty, i, j);
//    std::thread t5(forty_eight_sub_swap, 5, curr_path_intersection, turn_penalty, i, j);
//    std::thread t6(forty_eight_sub_swap, 6, curr_path_intersection, turn_penalty, i, j);
//    std::thread t7(forty_eight_sub_swap, 7, curr_path_intersection, turn_penalty, i, j);
//    std::thread t8(forty_eight_sub_swap, 8, curr_path_intersection, turn_penalty, i, j);
//
//    t1.join();
//    std::cout << "thread 1 end" << std::endl;
//    t2.join();
//    std::cout << "thread 2 end" << std::endl;
//    t3.join();
//    std::cout << "thread 3 end" << std::endl;
//    t4.join();
//    std::cout << "thread 4 end" << std::endl;
//    t5.join();
//    std::cout << "thread 5 end" << std::endl;
//    t6.join();
//    std::cout << "thread 6 end" << std::endl;
//    t7.join();
//    std::cout << "thread 7 end" << std::endl;
//    t8.join();
//    std::cout << "thread 8 end" << std::endl;
//}

//void forty_eight_sub_swap(unsigned thread_case, std::vector<unsigned> curr_path_intersection, const float turn_penalty, const unsigned& i, const unsigned& j)
//{
//
//    if (thread_case == 1)
//    {
//        std::cout << "in thread 1" << std::endl;
//        std::vector<unsigned> primary_path;
//
//        std::vector<unsigned> secondary_path1;
//        std::vector<unsigned> secondary_path2;
//        std::vector<unsigned> secondary_path3;
//        std::vector<unsigned> secondary_path4;
//        std::vector<unsigned> secondary_path5;
//
//        primary_path = ABC(curr_path_intersection);
//
//        secondary_path1 = sub_ACB(primary_path, i, j);
//        secondary_path2 = sub_BAC(primary_path, i, j);
//        secondary_path3 = sub_BCA(primary_path, i, j);
//        secondary_path4 = sub_CAB(primary_path, i, j);
//        secondary_path5 = sub_CBA(primary_path, i, j);
//
//        float cost = cost_from_intersection(primary_path, turn_penalty);
//        float cost1 = cost_from_intersection(secondary_path1, turn_penalty);
//        float cost2 = cost_from_intersection(secondary_path2, turn_penalty);
//        float cost3 = cost_from_intersection(secondary_path3, turn_penalty);
//        float cost4 = cost_from_intersection(secondary_path4, turn_penalty);
//        float cost5 = cost_from_intersection(secondary_path5, turn_penalty);
//
//        if (cost >= cost1 && cost >= cost2 && cost >= cost3 && cost >= cost4 && cost >= cost5)
//        {
//            best_sub_path = primary_path;
//            best_sub_cost = cost;
//
//        }
//        else if (cost1 >= cost && cost1 >= cost2 && cost1 >= cost3 && cost1 >= cost4 && cost1 >= cost5)
//        {
//            best_sub_path = secondary_path1;
//            best_sub_cost = cost1;
//
//        }
//        else if (cost2 >= cost && cost2 >= cost1 && cost2 >= cost3 && cost2 >= cost4 && cost2 >= cost5)
//        {
//            best_sub_path = secondary_path2;
//            best_sub_cost = cost2;
//
//        }
//        else if (cost3 >= cost && cost3 >= cost1 && cost3 >= cost2 && cost3 >= cost4 && cost3 >= cost5)
//        {
//            best_sub_path = secondary_path3;
//            best_sub_cost = cost3;
//
//
//        }
//        else if (cost4 >= cost && cost4 >= cost1 && cost4 >= cost2 && cost4 >= cost3 && cost4 >= cost5)
//        {
//            best_sub_path = secondary_path4;
//            best_sub_cost = cost4;
//
//
//        }
//        else if (cost5 >= cost && cost5 >= cost1 && cost5 >= cost2 && cost5 >= cost3 && cost5 >= cost4)
//        {
//            best_sub_path = secondary_path5;
//            best_sub_cost = cost5;
//
//        }
//        else
//            best_sub_cost = -1;
//
//        std::cout << "end thread 1" << std::endl;
//    }
//    else if (thread_case == 2)
//    {
//
//        std::cout << "in thread 2" << std::endl;
//        std::vector<unsigned> primary_path;
//
//        std::vector<unsigned> secondary_path1;
//        std::vector<unsigned> secondary_path2;
//        std::vector<unsigned> secondary_path3;
//        std::vector<unsigned> secondary_path4;
//        std::vector<unsigned> secondary_path5;
//
//        primary_path = AB_C(curr_path_intersection, i, j);
//
//        secondary_path1 = sub_ACB(primary_path, i, j);
//        secondary_path2 = sub_BAC(primary_path, i, j);
//        secondary_path3 = sub_BCA(primary_path, i, j);
//        secondary_path4 = sub_CAB(primary_path, i, j);
//        secondary_path5 = sub_CBA(primary_path, i, j);
//
//
//        float cost = cost_from_intersection(primary_path, turn_penalty);
//        float cost1 = cost_from_intersection(secondary_path1, turn_penalty);
//        float cost2 = cost_from_intersection(secondary_path2, turn_penalty);
//        float cost3 = cost_from_intersection(secondary_path3, turn_penalty);
//        float cost4 = cost_from_intersection(secondary_path4, turn_penalty);
//        float cost5 = cost_from_intersection(secondary_path5, turn_penalty);
//
//        if (cost >= cost1 && cost >= cost2 && cost >= cost3 && cost >= cost4 && cost >= cost5)
//        {
//            best_sub_path1 = primary_path;
//            best_sub_cost1 = cost;
//
//
//        }
//        else if (cost1 >= cost && cost1 >= cost2 && cost1 >= cost3 && cost1 >= cost4 && cost1 >= cost5)
//        {
//            best_sub_path1 = secondary_path1;
//            best_sub_cost1 = cost1;
//
//        }
//        else if (cost2 >= cost && cost2 >= cost1 && cost2 >= cost3 && cost2 >= cost4 && cost2 >= cost5)
//        {
//            best_sub_path1 = secondary_path2;
//            best_sub_cost1 = cost2;
//
//
//        }
//        else if (cost3 >= cost && cost3 >= cost1 && cost3 >= cost2 && cost3 >= cost4 && cost3 >= cost5)
//        {
//            best_sub_path1 = secondary_path3;
//            best_sub_cost1 = cost3;
//
//
//        }
//        else if (cost4 >= cost && cost4 >= cost1 && cost4 >= cost2 && cost4 >= cost3 && cost4 >= cost5)
//        {
//            best_sub_path1 = secondary_path4;
//            best_sub_cost1 = cost4;
//
//
//        }
//        else if (cost5 >= cost && cost5 >= cost1 && cost5 >= cost2 && cost5 >= cost3 && cost5 >= cost4)
//        {
//            best_sub_path1 = secondary_path5;
//            best_sub_cost1 = cost5;
//
//        }
//        else
//            best_sub_cost1 = -1;
//
//        std::cout << "end thread 2" << std::endl;
//    }
//    else if (thread_case == 3)
//    {
//        std::cout << "in thread 3" << std::endl;
//        std::vector<unsigned> primary_path;
//
//        std::vector<unsigned> secondary_path1;
//        std::vector<unsigned> secondary_path2;
//        std::vector<unsigned> secondary_path3;
//        std::vector<unsigned> secondary_path4;
//        std::vector<unsigned> secondary_path5;
//
//        primary_path = ABC_(curr_path_intersection, i, j);
//
//        secondary_path1 = sub_ACB(primary_path, i, j);
//        secondary_path2 = sub_BAC(primary_path, i, j);
//        secondary_path3 = sub_BCA(primary_path, i, j);
//        secondary_path4 = sub_CAB(primary_path, i, j);
//        secondary_path5 = sub_CBA(primary_path, i, j);
//
//
//        float cost = cost_from_intersection(primary_path, turn_penalty);
//        float cost1 = cost_from_intersection(secondary_path1, turn_penalty);
//        float cost2 = cost_from_intersection(secondary_path2, turn_penalty);
//        float cost3 = cost_from_intersection(secondary_path3, turn_penalty);
//        float cost4 = cost_from_intersection(secondary_path4, turn_penalty);
//        float cost5 = cost_from_intersection(secondary_path5, turn_penalty);
//
//        if (cost >= cost1 && cost >= cost2 && cost >= cost3 && cost >= cost4 && cost >= cost5)
//        {
//            best_sub_path2 = primary_path;
//            best_sub_cost2 = cost;
//
//
//        }
//        else if (cost1 >= cost && cost1 >= cost2 && cost1 >= cost3 && cost1 >= cost4 && cost1 >= cost5)
//        {
//            best_sub_path2 = secondary_path1;
//            best_sub_cost2 = cost1;
//
//        }
//        else if (cost2 >= cost && cost2 >= cost1 && cost2 >= cost3 && cost2 >= cost4 && cost2 >= cost5)
//        {
//            best_sub_path2 = secondary_path2;
//            best_sub_cost2 = cost2;
//
//
//        }
//        else if (cost3 >= cost && cost3 >= cost1 && cost3 >= cost2 && cost3 >= cost4 && cost3 >= cost5)
//        {
//            best_sub_path2 = secondary_path3;
//            best_sub_cost2 = cost3;
//
//
//        }
//        else if (cost4 >= cost && cost4 >= cost1 && cost4 >= cost2 && cost4 >= cost3 && cost4 >= cost5)
//        {
//            best_sub_path2 = secondary_path4;
//            best_sub_cost2 = cost4;
//
//
//        }
//        else if (cost5 >= cost && cost5 >= cost1 && cost5 >= cost2 && cost5 >= cost3 && cost5 >= cost4)
//        {
//            best_sub_path2 = secondary_path5;
//            best_sub_cost2 = cost5;
//
//
//        }
//        else
//            best_sub_cost2 = -1;
//
//        std::cout << "end thread 3" << std::endl;
//    }
//    else if (thread_case == 4)
//    {
//        std::cout << "in thread 4" << std::endl;
//        std::vector<unsigned> primary_path;
//
//        std::vector<unsigned> secondary_path1;
//        std::vector<unsigned> secondary_path2;
//        std::vector<unsigned> secondary_path3;
//        std::vector<unsigned> secondary_path4;
//        std::vector<unsigned> secondary_path5;
//
//        primary_path = AB_C_(curr_path_intersection, i, j);
//
//        secondary_path1 = sub_ACB(primary_path, i, j);
//        secondary_path2 = sub_BAC(primary_path, i, j);
//        secondary_path3 = sub_BCA(primary_path, i, j);
//        secondary_path4 = sub_CAB(primary_path, i, j);
//        secondary_path5 = sub_CBA(primary_path, i, j);
//
//
//        float cost = cost_from_intersection(primary_path, turn_penalty);
//        float cost1 = cost_from_intersection(secondary_path1, turn_penalty);
//        float cost2 = cost_from_intersection(secondary_path2, turn_penalty);
//        float cost3 = cost_from_intersection(secondary_path3, turn_penalty);
//        float cost4 = cost_from_intersection(secondary_path4, turn_penalty);
//        float cost5 = cost_from_intersection(secondary_path5, turn_penalty);
//
//        if (cost >= cost1 && cost >= cost2 && cost >= cost3 && cost >= cost4 && cost >= cost5)
//        {
//            best_sub_path3 = primary_path;
//            best_sub_cost3 = cost;
//
//
//        }
//        else if (cost1 >= cost && cost1 >= cost2 && cost1 >= cost3 && cost1 >= cost4 && cost1 >= cost5)
//        {
//            best_sub_path3 = secondary_path1;
//            best_sub_cost3 = cost1;
//
//        }
//        else if (cost2 >= cost && cost2 >= cost1 && cost2 >= cost3 && cost2 >= cost4 && cost2 >= cost5)
//        {
//            best_sub_path3 = secondary_path2;
//            best_sub_cost3 = cost2;
//
//
//        }
//        else if (cost3 >= cost && cost3 >= cost1 && cost3 >= cost2 && cost3 >= cost4 && cost3 >= cost5)
//        {
//            best_sub_path3 = secondary_path3;
//            best_sub_cost3 = cost3;
//
//
//        }
//        else if (cost4 >= cost && cost4 >= cost1 && cost4 >= cost2 && cost4 >= cost3 && cost4 >= cost5)
//        {
//            best_sub_path3 = secondary_path4;
//            best_sub_cost3 = cost4;
//
//
//        }
//        else if (cost5 >= cost && cost5 >= cost1 && cost5 >= cost2 && cost5 >= cost3 && cost5 >= cost4)
//        {
//            best_sub_path3 = secondary_path5;
//            best_sub_cost3 = cost5;
//
//
//        }
//        else
//            best_sub_cost3 = -1;
//
//        std::cout << "end thread 4" << std::endl;
//    }
//    else if (thread_case == 5)
//    {
//        std::cout << "in thread 5" << std::endl;
//        std::vector<unsigned> primary_path;
//
//        std::vector<unsigned> secondary_path1;
//        std::vector<unsigned> secondary_path2;
//        std::vector<unsigned> secondary_path3;
//        std::vector<unsigned> secondary_path4;
//        std::vector<unsigned> secondary_path5;
//
//        primary_path = A_BC(curr_path_intersection, i, j);
//
//        secondary_path1 = sub_ACB(primary_path, i, j);
//        secondary_path2 = sub_BAC(primary_path, i, j);
//        secondary_path3 = sub_BCA(primary_path, i, j);
//        secondary_path4 = sub_CAB(primary_path, i, j);
//        secondary_path5 = sub_CBA(primary_path, i, j);
//
//
//        float cost = cost_from_intersection(primary_path, turn_penalty);
//        float cost1 = cost_from_intersection(secondary_path1, turn_penalty);
//        float cost2 = cost_from_intersection(secondary_path2, turn_penalty);
//        float cost3 = cost_from_intersection(secondary_path3, turn_penalty);
//        float cost4 = cost_from_intersection(secondary_path4, turn_penalty);
//        float cost5 = cost_from_intersection(secondary_path5, turn_penalty);
//
//        if (cost >= cost1 && cost >= cost2 && cost >= cost3 && cost >= cost4 && cost >= cost5)
//        {
//            best_sub_path4 = primary_path;
//            best_sub_cost4 = cost;
//
//
//        }
//        else if (cost1 >= cost && cost1 >= cost2 && cost1 >= cost3 && cost1 >= cost4 && cost1 >= cost5)
//        {
//            best_sub_path4 = secondary_path1;
//            best_sub_cost4 = cost1;
//
//        }
//        else if (cost2 >= cost && cost2 >= cost1 && cost2 >= cost3 && cost2 >= cost4 && cost2 >= cost5)
//        {
//            best_sub_path4 = secondary_path2;
//            best_sub_cost4 = cost2;
//
//
//        }
//        else if (cost3 >= cost && cost3 >= cost1 && cost3 >= cost2 && cost3 >= cost4 && cost3 >= cost5)
//        {
//            best_sub_path4 = secondary_path3;
//            best_sub_cost4 = cost3;
//
//
//        }
//        else if (cost4 >= cost && cost4 >= cost1 && cost4 >= cost2 && cost4 >= cost3 && cost4 >= cost5)
//        {
//            best_sub_path4 = secondary_path4;
//            best_sub_cost4 = cost4;
//
//
//        }
//        else if (cost5 >= cost && cost5 >= cost1 && cost5 >= cost2 && cost5 >= cost3 && cost5 >= cost4)
//        {
//            best_sub_path4 = secondary_path5;
//            best_sub_cost4 = cost5;
//
//
//        }
//        else
//            best_sub_cost4 = -1;
//
//        std::cout << "end thread 5" << std::endl;
//    }
//    else if (thread_case == 6)
//    {
//        std::cout << "in thread 6" << std::endl;
//        std::vector<unsigned> primary_path;
//
//        std::vector<unsigned> secondary_path1;
//        std::vector<unsigned> secondary_path2;
//        std::vector<unsigned> secondary_path3;
//        std::vector<unsigned> secondary_path4;
//        std::vector<unsigned> secondary_path5;
//
//        primary_path = A_BC_(curr_path_intersection, i, j);
//
//        secondary_path1 = sub_ACB(primary_path, i, j);
//        secondary_path2 = sub_BAC(primary_path, i, j);
//        secondary_path3 = sub_BCA(primary_path, i, j);
//        secondary_path4 = sub_CAB(primary_path, i, j);
//        secondary_path5 = sub_CBA(primary_path, i, j);
//
//
//        float cost = cost_from_intersection(primary_path, turn_penalty);
//        float cost1 = cost_from_intersection(secondary_path1, turn_penalty);
//        float cost2 = cost_from_intersection(secondary_path2, turn_penalty);
//        float cost3 = cost_from_intersection(secondary_path3, turn_penalty);
//        float cost4 = cost_from_intersection(secondary_path4, turn_penalty);
//        float cost5 = cost_from_intersection(secondary_path5, turn_penalty);
//
//        if (cost >= cost1 && cost >= cost2 && cost >= cost3 && cost >= cost4 && cost >= cost5)
//        {
//            best_sub_path5 = primary_path;
//            best_sub_cost5 = cost;
//
//
//        }
//        else if (cost1 >= cost && cost1 >= cost2 && cost1 >= cost3 && cost1 >= cost4 && cost1 >= cost5)
//        {
//            best_sub_path5 = secondary_path1;
//            best_sub_cost5 = cost1;
//
//        }
//        else if (cost2 >= cost && cost2 >= cost1 && cost2 >= cost3 && cost2 >= cost4 && cost2 >= cost5)
//        {
//            best_sub_path5 = secondary_path2;
//            best_sub_cost5 = cost2;
//
//
//        }
//        else if (cost3 >= cost && cost3 >= cost1 && cost3 >= cost2 && cost3 >= cost4 && cost3 >= cost5)
//        {
//            best_sub_path5 = secondary_path3;
//            best_sub_cost5 = cost3;
//
//
//        }
//        else if (cost4 >= cost && cost4 >= cost1 && cost4 >= cost2 && cost4 >= cost3 && cost4 >= cost5)
//        {
//            best_sub_path5 = secondary_path4;
//            best_sub_cost5 = cost4;
//
//
//        }
//        else if (cost5 >= cost && cost5 >= cost1 && cost5 >= cost2 && cost5 >= cost3 && cost5 >= cost4)
//        {
//            best_sub_path5 = secondary_path5;
//            best_sub_cost5 = cost5;
//
//
//        }
//
//        else
//            best_sub_cost5 = -1;
//
//        std::cout << "end thread 6" << std::endl;
//    }
//    else if (thread_case == 7)
//    {
//        std::cout << "in thread 7" << std::endl;
//        std::vector<unsigned> primary_path;
//
//        std::vector<unsigned> secondary_path1;
//        std::vector<unsigned> secondary_path2;
//        std::vector<unsigned> secondary_path3;
//        std::vector<unsigned> secondary_path4;
//        std::vector<unsigned> secondary_path5;
//
//        primary_path = A_B_C(curr_path_intersection, i, j);
//
//        secondary_path1 = sub_ACB(primary_path, i, j);
//        secondary_path2 = sub_BAC(primary_path, i, j);
//        secondary_path3 = sub_BCA(primary_path, i, j);
//        secondary_path4 = sub_CAB(primary_path, i, j);
//        secondary_path5 = sub_CBA(primary_path, i, j);
//
//
//        float cost = cost_from_intersection(primary_path, turn_penalty);
//        float cost1 = cost_from_intersection(secondary_path1, turn_penalty);
//        float cost2 = cost_from_intersection(secondary_path2, turn_penalty);
//        float cost3 = cost_from_intersection(secondary_path3, turn_penalty);
//        float cost4 = cost_from_intersection(secondary_path4, turn_penalty);
//        float cost5 = cost_from_intersection(secondary_path5, turn_penalty);
//
//        if (cost >= cost1 && cost >= cost2 && cost >= cost3 && cost >= cost4 && cost >= cost5)
//        {
//            best_sub_path6 = primary_path;
//            best_sub_cost6 = cost;
//
//
//        }
//        else if (cost1 >= cost && cost1 >= cost2 && cost1 >= cost3 && cost1 >= cost4 && cost1 >= cost5)
//
//        {
//            best_sub_path6 = secondary_path1;
//            best_sub_cost6 = cost1;
//
//        }
//        else if (cost2 >= cost && cost2 >= cost1 && cost2 >= cost3 && cost2 >= cost4 && cost2 >= cost5)
//        {
//            best_sub_path6 = secondary_path2;
//            best_sub_cost6 = cost2;
//
//
//        }
//        else if (cost3 >= cost && cost3 >= cost1 && cost3 >= cost2 && cost3 >= cost4 && cost3 >= cost5)
//        {
//            best_sub_path6 = secondary_path3;
//            best_sub_cost6 = cost3;
//
//
//        }
//        else if (cost4 >= cost && cost4 >= cost1 && cost4 >= cost2 && cost4 >= cost3 && cost4 >= cost5)
//        {
//            best_sub_path6 = secondary_path4;
//            best_sub_cost6 = cost4;
//
//
//        }
//        else if (cost5 >= cost && cost5 >= cost1 && cost5 >= cost2 && cost5 >= cost3 && cost5 >= cost4)
//        {
//            best_sub_path6 = secondary_path5;
//            best_sub_cost6 = cost5;
//
//
//        }
//
//        else
//            best_sub_cost6 = -1;
//
//        std::cout << "end thread 7" << std::endl;
//    }
//    else if (thread_case == 8)
//    {
//        std::cout << "in thread 8" << std::endl;
//        std::vector<unsigned> primary_path;
//
//        std::vector<unsigned> secondary_path1;
//        std::vector<unsigned> secondary_path2;
//        std::vector<unsigned> secondary_path3;
//        std::vector<unsigned> secondary_path4;
//        std::vector<unsigned> secondary_path5;
//
//        primary_path = A_B_C_(curr_path_intersection, i, j);
//
//        secondary_path1 = sub_ACB(primary_path, i, j);
//        secondary_path2 = sub_BAC(primary_path, i, j);
//        secondary_path3 = sub_BCA(primary_path, i, j);
//        secondary_path4 = sub_CAB(primary_path, i, j);
//        secondary_path5 = sub_CBA(primary_path, i, j);
//
//
//        float cost = cost_from_intersection(primary_path, turn_penalty);
//        float cost1 = cost_from_intersection(secondary_path1, turn_penalty);
//        float cost2 = cost_from_intersection(secondary_path2, turn_penalty);
//        float cost3 = cost_from_intersection(secondary_path3, turn_penalty);
//        float cost4 = cost_from_intersection(secondary_path4, turn_penalty);
//        float cost5 = cost_from_intersection(secondary_path5, turn_penalty);
//
//        if (cost >= cost1 && cost >= cost2 && cost >= cost3 && cost >= cost4 && cost >= cost5)
//        {
//            best_sub_path7 = primary_path;
//            best_sub_cost7 = cost;
//
//
//        }
//        else if (cost1 >= cost && cost1 >= cost2 && cost1 >= cost3 && cost1 >= cost4 && cost1 >= cost5)
//        {
//            best_sub_path7 = secondary_path1;
//            best_sub_cost7 = cost1;
//
//        }
//        else if (cost2 >= cost && cost2 >= cost1 && cost2 >= cost3 && cost2 >= cost4 && cost2 >= cost5)
//        {
//            best_sub_path7 = secondary_path2;
//            best_sub_cost7 = cost2;
//
//
//        }
//        else if (cost3 >= cost && cost3 >= cost1 && cost3 >= cost2 && cost3 >= cost4 && cost3 >= cost5)
//        {
//            best_sub_path7 = secondary_path3;
//            best_sub_cost7 = cost3;
//
//
//        }
//        else if (cost4 >= cost && cost4 >= cost1 && cost4 >= cost2 && cost4 >= cost3 && cost4 >= cost5)
//        {
//            best_sub_path7 = secondary_path4;
//            best_sub_cost7 = cost4;
//
//
//        }
//        else if (cost5 >= cost && cost5 >= cost1 && cost5 >= cost2 && cost5 >= cost3 && cost5 >= cost4)
//        {
//            best_sub_path7 = secondary_path5;
//            best_sub_cost7 = cost5;
//
//
//        }
//
//        else
//            best_sub_cost7 = -1;
//
//        std::cout << "end thread 8" << std::endl;
//    }
//
//}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*
    
 *               1. ABC
 *               2. AB_C
 *               3. ABC_
 *               4. AB_C_
 *               5. A_BC
 *               6. A_B_C
 *               7. A_BC_
 *               8. A_B_C_
 */

/*============================================================================*/


std::vector<unsigned> ABC(std::vector<unsigned> curr_path_intersection)
{

    //    std::vector<unsigned> swapped_path_intersection;
    //
    //    for (unsigned k = 0; k < curr_path_intersection.size(); ++k)
    //    {
    //        swapped_path_intersection.push_back(curr_path_intersection[k]);
    //    }
    //
    //    return swapped_path_intersection;

    return curr_path_intersection;
}

std::vector<unsigned> AB_C(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // A
    for (unsigned k = 0; k < i; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // AB_
    unsigned dec = 0;
    for (unsigned k = i; k <= j; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[j - dec]);
        dec++;
    }
    // AB_C
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }

    return swapped_path_intersection;
}

std::vector<unsigned> ABC_(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // AB
    for (unsigned k = 0; k <= j; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // ABC_
    unsigned dec = 0;
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[curr_path_intersection.size() - 1 - dec]);
        dec++;
    }

    return swapped_path_intersection;
}

std::vector<unsigned> AB_C_(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // A
    for (unsigned k = 0; k < i; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // AB_
    unsigned dec = 0;
    for (unsigned k = i; k <= j; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[j - dec]);
        dec++;
    }
    // AB_C_
    dec = 0;
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[curr_path_intersection.size() - 1 - dec]);
        dec++;
    }
    return swapped_path_intersection;
}

std::vector<unsigned> A_BC(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // A_
    unsigned dec = 0;
    for (unsigned k = 0; k < i; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[i - 1 - dec]);
        dec++;
    }
    // A_B
    for (unsigned k = i; k <= j; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // A_BC
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    return swapped_path_intersection;
}

std::vector<unsigned> A_BC_(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // A_
    unsigned dec = 0;
    for (unsigned k = 0; k < i; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[i - 1 - dec]);
        dec++;
    }
    // A_B
    for (unsigned k = i; k <= j; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // A_BC_
    dec = 0;
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[curr_path_intersection.size() - 1 - dec]);
        dec++;
    }
    return swapped_path_intersection;
}

std::vector<unsigned> A_B_C(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // A_
    unsigned dec = 0;
    for (unsigned k = 0; k < i; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[i - 1 - dec]);
        dec++;
    }
    // A_B_
    dec = 0;
    for (unsigned k = i; k <= j; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[j - dec]);
        dec++;
    }
    // A_B_C
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    return swapped_path_intersection;
}

std::vector<unsigned> A_B_C_(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // A_
    unsigned dec = 0;
    for (unsigned k = 0; k < i; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[i - 1 - dec]);
        dec++;
    }
    // A_B_
    dec = 0;
    for (unsigned k = i; k <= j; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[j - dec]);
        dec++;
    }
    // A_B_C_
    dec = 0;
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[curr_path_intersection.size() - 1 - dec]);
        dec++;
    }
    return swapped_path_intersection;
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*
    
 *               1. ABC
 *               2. ACB
 *               3. BAC
 *               4. BCA
 *               5. CAB
 *               6. CBA
     
 */

/*============================================================================*/

//std::vector<unsigned> sub_ABC(std::vector<unsigned> curr_path_intersection, const float turn_penalty)
//{
//    std::vector<unsigned> swapped_path_intersection;
//    // ABC
//    for (unsigned k = 0; k < curr_path_intersection.size(); ++k)
//    {
//        swapped_path_intersection.push_back(curr_path_intersection[k]);
//    }
//    return swapped_path_intersection;
//}

std::vector<unsigned> sub_ACB(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // A
    for (unsigned k = 0; k < i; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // AC
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // ACB
    for (unsigned k = i; k <= j; ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    return swapped_path_intersection;
}

std::vector<unsigned> sub_BAC(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // B
    for (unsigned k = i; k <= j; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // BA
    for (unsigned k = 0; k < i; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // BAC
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    return swapped_path_intersection;
}

std::vector<unsigned> sub_BCA(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // B
    for (unsigned k = i; k <= j; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // BC
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // BCA
    for (unsigned k = 0; k < i; ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    return swapped_path_intersection;
}

std::vector<unsigned> sub_CAB(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // C
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // CA
    for (unsigned k = 0; k < i; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);

    }
    // CAB
    for (unsigned k = i; k <= j; ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    return swapped_path_intersection;
}

std::vector<unsigned> sub_CBA(std::vector<unsigned> curr_path_intersection, const unsigned& i, const unsigned& j)
{
    std::vector<unsigned> swapped_path_intersection;
    // C
    for (unsigned k = j + 1; k < curr_path_intersection.size(); ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // CB
    for (unsigned k = i; k <= j; ++k)
    {
        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    // CBA
    for (unsigned k = 0; k < i; ++k)
    {

        swapped_path_intersection.push_back(curr_path_intersection[k]);
    }
    return swapped_path_intersection;
}

std::vector<unsigned> ABDC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;

}

std::vector<unsigned> ACBD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;

}

std::vector<unsigned> ACDB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;

}

std::vector<unsigned> ADBC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;

}

std::vector<unsigned> ADCB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;
}

std::vector<unsigned> BACD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;

    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;
}

std::vector<unsigned> BADC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{


    std::vector<unsigned> new_path;

    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;

}

std::vector<unsigned> BCAD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;

    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;

}

std::vector<unsigned> BCDA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{


    std::vector<unsigned> new_path;

    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;

}

std::vector<unsigned> BDAC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;

    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }


    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;
}

std::vector<unsigned> BDCA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;

    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }


    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;

}

std::vector<unsigned> CABD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;

    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }


    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;
}

std::vector<unsigned> CADB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //b
    for (unsigned n = i; n < j + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;
}

std::vector<unsigned> CBAD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }


    return new_path;

}

std::vector<unsigned> CBDA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //a
    for (unsigned n = 0; n < i; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;

}

std::vector<unsigned> CDAB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }


    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;

}

std::vector<unsigned> CDBA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //a
    for (unsigned n = 0; n < i; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> DABC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;


    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;

}

std::vector<unsigned> DACB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;


    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> DBAC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;


    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;

}

std::vector<unsigned> DBCA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{


    std::vector<unsigned> new_path;


    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;

}

std::vector<unsigned> DCAB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{

    std::vector<unsigned> new_path;


    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;

}

std::vector<unsigned> DCBA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k)
{


    std::vector<unsigned> new_path;


    //d
    for (unsigned n = j + 1; n < k + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //c
    for (unsigned n = k + 1; n < path_by_intersection.size(); n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    return new_path;

}

std::vector<unsigned> ABCED(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;


}

std::vector<unsigned> ABDCE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> ABDEC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }


    return new_path;
}

std::vector<unsigned> ABECD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> ABEDC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }


    return new_path;
}

std::vector<unsigned> ACBDE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> ACBED(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> ACDBE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> ACDEB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }


    return new_path;
}

std::vector<unsigned> ACEBD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> ACEDB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> ADBCE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> ADBEC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> ADCBE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }


    return new_path;
}

std::vector<unsigned> ADCEB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }


    return new_path;
}

std::vector<unsigned> ADEBC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> ADECB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> AEBCD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> AEBDC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> AECBD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }


    return new_path;
}

std::vector<unsigned> AECDB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }


    return new_path;
}

std::vector<unsigned> AEDBC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}

std::vector<unsigned> AEDCB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
{
    std::vector<unsigned> new_path;

    //a
    for (unsigned n = 0; n < i; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //e
    for (unsigned n = l + 1; n < path_by_intersection.size(); n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //d
    for (unsigned n = k + 1; n < l + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }
    //c
    for (unsigned n = j + 1; n < k + 1; n++)
    {

        new_path.push_back(path_by_intersection[n]);
    }
    //b
    for (unsigned n = i; n < j + 1; n++)
    {
        new_path.push_back(path_by_intersection[n]);
    }

    return new_path;
}
//std::vector<unsigned> BACDE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BACED(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BADCE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BADEC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BAECD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BAEDC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BCABDE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BCABED(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BCADBE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BCADEB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BCAEBD(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BCAEDB(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BDACE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BDAEC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BDCAE(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BDCEA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BDEAC(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}
//
//std::vector<unsigned> BDECA(std::vector<unsigned> path_by_intersection, unsigned i, unsigned j, unsigned k, unsigned l)
//{
//}

std::vector<unsigned> find_path_between_intersections1(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty)
{

    std::vector<unsigned> path;

    // Empty vector
    if (intersect_id_start >= intersectionCells1.size() || intersect_id_end >= intersectionCells1.size())
    {
        return path;
    }

    // reset all heuristics
    for (unsigned i = 0; i < intersectionCells1.size(); i++)
    {
        intersectionCells1[i].F = std::numeric_limits<float>::infinity();
    }

    // initial heuristic is 0
    intersectionCells1[intersect_id_start].F = 0;
    std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

    queue.push(intersectionCells1[intersect_id_start]);

    // continue checking until every intersection is checked
    while (!queue.empty())
    {
        SearchCell currentCell = queue.top();

        // pop the top of the queue, intersection that is visited
        queue.pop();

        if (currentCell.F <= intersectionCells1[currentCell.id].F)
        {
            for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
            {
                unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                {

                    F += turn_penalty;

                }

                // a better path is found
                if (F < intersectionCells1[thisConnection].F)
                {
                    intersectionCells1[thisConnection].F = F;
                    intersectionCells1[thisConnection].parent_intersection = currentCell.id;
                    intersectionCells1[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                    queue.push(intersectionCells1[thisConnection]);
                }
            }
        }

        // path is found
        if (currentCell.id == intersect_id_end)
        {
            unsigned navigator = intersect_id_end;
            while (navigator != intersect_id_start)
            {
                path.push_back(intersectionCells1[navigator].parent_street_segment);
                navigator = intersectionCells1[navigator].parent_intersection;
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

    }
    return path;
}

std::vector<unsigned> find_path_between_intersections2(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty)
{

    std::vector<unsigned> path;

    // Empty vector
    if (intersect_id_start >= intersectionCells2.size() || intersect_id_end >= intersectionCells2.size())
    {
        return path;
    }

    // reset all heuristics
    for (unsigned i = 0; i < intersectionCells2.size(); i++)
    {
        intersectionCells2[i].F = std::numeric_limits<float>::infinity();
    }

    // initial heuristic is 0
    intersectionCells2[intersect_id_start].F = 0;
    std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

    queue.push(intersectionCells2[intersect_id_start]);

    // continue checking until every intersection is checked
    while (!queue.empty())
    {
        SearchCell currentCell = queue.top();

        // pop the top of the queue, intersection that is visited
        queue.pop();

        if (currentCell.F <= intersectionCells2[currentCell.id].F)
        {
            for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
            {
                unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                {

                    F += turn_penalty;

                }

                // a better path is found
                if (F < intersectionCells2[thisConnection].F)
                {
                    intersectionCells2[thisConnection].F = F;
                    intersectionCells2[thisConnection].parent_intersection = currentCell.id;
                    intersectionCells2[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                    queue.push(intersectionCells2[thisConnection]);
                }
            }
        }

        // path is found
        if (currentCell.id == intersect_id_end)
        {
            unsigned navigator = intersect_id_end;
            while (navigator != intersect_id_start)
            {
                path.push_back(intersectionCells2[navigator].parent_street_segment);
                navigator = intersectionCells2[navigator].parent_intersection;
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

    }
    return path;
}

std::vector<unsigned> find_path_between_intersections3(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty)
{

    std::vector<unsigned> path;

    // Empty vector
    if (intersect_id_start >= intersectionCells3.size() || intersect_id_end >= intersectionCells3.size())
    {
        return path;
    }

    // reset all heuristics
    for (unsigned i = 0; i < intersectionCells3.size(); i++)
    {
        intersectionCells3[i].F = std::numeric_limits<float>::infinity();
    }

    // initial heuristic is 0
    intersectionCells3[intersect_id_start].F = 0;
    std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

    queue.push(intersectionCells3[intersect_id_start]);

    // continue checking until every intersection is checked
    while (!queue.empty())
    {
        SearchCell currentCell = queue.top();

        // pop the top of the queue, intersection that is visited
        queue.pop();

        if (currentCell.F <= intersectionCells3[currentCell.id].F)
        {
            for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
            {
                unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                {

                    F += turn_penalty;

                }

                // a better path is found
                if (F < intersectionCells3[thisConnection].F)
                {
                    intersectionCells3[thisConnection].F = F;
                    intersectionCells3[thisConnection].parent_intersection = currentCell.id;
                    intersectionCells3[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                    queue.push(intersectionCells3[thisConnection]);
                }
            }
        }

        // path is found
        if (currentCell.id == intersect_id_end)
        {
            unsigned navigator = intersect_id_end;
            while (navigator != intersect_id_start)
            {
                path.push_back(intersectionCells3[navigator].parent_street_segment);
                navigator = intersectionCells3[navigator].parent_intersection;
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

    }
    return path;
}

std::vector<unsigned> find_path_between_intersections4(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty)
{

    std::vector<unsigned> path;

    // Empty vector
    if (intersect_id_start >= intersectionCells4.size() || intersect_id_end >= intersectionCells4.size())
    {
        return path;
    }

    // reset all heuristics
    for (unsigned i = 0; i < intersectionCells4.size(); i++)
    {
        intersectionCells4[i].F = std::numeric_limits<float>::infinity();
    }

    // initial heuristic is 0
    intersectionCells4[intersect_id_start].F = 0;
    std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

    queue.push(intersectionCells4[intersect_id_start]);

    // continue checking until every intersection is checked
    while (!queue.empty())
    {
        SearchCell currentCell = queue.top();

        // pop the top of the queue, intersection that is visited
        queue.pop();

        if (currentCell.F <= intersectionCells4[currentCell.id].F)
        {
            for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
            {
                unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                {

                    F += turn_penalty;

                }

                // a better path is found
                if (F < intersectionCells4[thisConnection].F)
                {
                    intersectionCells4[thisConnection].F = F;
                    intersectionCells4[thisConnection].parent_intersection = currentCell.id;
                    intersectionCells4[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                    queue.push(intersectionCells4[thisConnection]);
                }
            }
        }

        // path is found
        if (currentCell.id == intersect_id_end)
        {
            unsigned navigator = intersect_id_end;
            while (navigator != intersect_id_start)
            {
                path.push_back(intersectionCells4[navigator].parent_street_segment);
                navigator = intersectionCells4[navigator].parent_intersection;
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

    }
    return path;
}

std::vector<unsigned> find_path_between_intersections5(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty)
{

    std::vector<unsigned> path;

    // Empty vector
    if (intersect_id_start >= intersectionCells5.size() || intersect_id_end >= intersectionCells5.size())
    {
        return path;
    }

    // reset all heuristics
    for (unsigned i = 0; i < intersectionCells5.size(); i++)
    {
        intersectionCells5[i].F = std::numeric_limits<float>::infinity();
    }

    // initial heuristic is 0
    intersectionCells5[intersect_id_start].F = 0;
    std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

    queue.push(intersectionCells5[intersect_id_start]);

    // continue checking until every intersection is checked
    while (!queue.empty())
    {
        SearchCell currentCell = queue.top();

        // pop the top of the queue, intersection that is visited
        queue.pop();

        if (currentCell.F <= intersectionCells5[currentCell.id].F)
        {
            for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
            {
                unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                {

                    F += turn_penalty;

                }

                // a better path is found
                if (F < intersectionCells5[thisConnection].F)
                {
                    intersectionCells5[thisConnection].F = F;
                    intersectionCells5[thisConnection].parent_intersection = currentCell.id;
                    intersectionCells5[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                    queue.push(intersectionCells5[thisConnection]);
                }
            }
        }

        // path is found
        if (currentCell.id == intersect_id_end)
        {
            unsigned navigator = intersect_id_end;
            while (navigator != intersect_id_start)
            {
                path.push_back(intersectionCells5[navigator].parent_street_segment);
                navigator = intersectionCells5[navigator].parent_intersection;
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

    }
    return path;
}

std::vector<unsigned> find_path_between_intersections6(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty)
{

    std::vector<unsigned> path;

    // Empty vector
    if (intersect_id_start >= intersectionCells6.size() || intersect_id_end >= intersectionCells6.size())
    {
        return path;
    }

    // reset all heuristics
    for (unsigned i = 0; i < intersectionCells6.size(); i++)
    {
        intersectionCells6[i].F = std::numeric_limits<float>::infinity();
    }

    // initial heuristic is 0
    intersectionCells6[intersect_id_start].F = 0;
    std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

    queue.push(intersectionCells6[intersect_id_start]);

    // continue checking until every intersection is checked
    while (!queue.empty())
    {
        SearchCell currentCell = queue.top();

        // pop the top of the queue, intersection that is visited
        queue.pop();

        if (currentCell.F <= intersectionCells6[currentCell.id].F)
        {
            for (unsigned i = 0; i < currentCell.directed_connections.size(); i++)
            {
                unsigned thisConnection = currentCell.directed_connections[i].intersection_id;
                double F = currentCell.F + TravelTime[currentCell.directed_connections[i].streetsegment_id];
                if (currentCell.id != intersect_id_start && streetSegmentInfo[currentCell.parent_street_segment].streetID != streetSegmentInfo[currentCell.directed_connections[i].streetsegment_id].streetID)
                {

                    F += turn_penalty;

                }

                // a better path is found
                if (F < intersectionCells6[thisConnection].F)
                {
                    intersectionCells6[thisConnection].F = F;
                    intersectionCells6[thisConnection].parent_intersection = currentCell.id;
                    intersectionCells6[thisConnection].parent_street_segment = currentCell.directed_connections[i].streetsegment_id;
                    queue.push(intersectionCells6[thisConnection]);
                }
            }
        }

        // path is found
        if (currentCell.id == intersect_id_end)
        {
            unsigned navigator = intersect_id_end;
            while (navigator != intersect_id_start)
            {
                path.push_back(intersectionCells6[navigator].parent_street_segment);
                navigator = intersectionCells6[navigator].parent_intersection;
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

    }
    return path;
}