/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "m1.h"
#include "m3.h"
#include "LoadSearchCell.h"
#include "POIInfo.h"
#include "nearestPOI.h"

#include <vector>
#include <unordered_map>
#include <utility>
#include <limits>
#include <queue>
#include <assert.h>
#include <list>
#include <float.h>
#include <time.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

extern std::vector<StreetSegmentInfo> streetSegmentInfo;
extern std::vector<intersectionInfo> intersectionInformation;
extern std::vector<SearchCell> intersectionCells;
extern std::vector<double> TravelTime;
extern std::unordered_map<std::string, std::vector<unsigned>> poiFromName;
extern std::vector<POIInfo> m1_POI;

unsigned DESIRED_POI_NUMBER = 0;

struct compare
{

    bool operator()(const SearchCell & l, const SearchCell& r)
    {
        return l.F > r.F;
    }
};

struct myLeast
{

    bool operator()(const nearestPOI & l, const nearestPOI& r)
    {
        return l.delta_distance < r.delta_distance;
    }
};

double compute_path_travel_time(const std::vector<unsigned>& path,
        const double turn_penalty)
{

    float travelTime = 0;
    unsigned numTurns = 0;

    // return empty vector 
    if (path.empty())
    {

        return travelTime;

    }
    else
    {
        // intial travel time
        travelTime += (TravelTime[path[0]]);
        if (path.size() > 1)
        {
            for (unsigned i = 1; i < path.size(); ++i)
            {

                travelTime += (TravelTime[path[i]]);

                if (streetSegmentInfo[path[i]].streetID != streetSegmentInfo[path[i - 1]].streetID)
                {

                    numTurns++;

                }
            }
        }
        return (travelTime + numTurns * turn_penalty);
    }
}

////////////////////////////////////////////////////////////////////////////////
// * Find fastest path using dijkstra, algorithm credit:                      //
// * https://en.wikipedia.org/wiki/Dijkstra's_algorithm                       //
////////////////////////////////////////////////////////////////////////////////

std::vector<unsigned> find_path_between_intersections(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty)
{

    std::vector<unsigned> path;

    // Empty vector
    if (intersect_id_start >= intersectionCells.size() || intersect_id_end >= intersectionCells.size())
    {
        return path;
    }

    // reset all heuristics
    for (unsigned i = 0; i < intersectionCells.size(); i++)
    {
        intersectionCells[i].F = std::numeric_limits<float>::infinity();
    }

    // initial heuristic is 0
    intersectionCells[intersect_id_start].F = 0;
    std::priority_queue<SearchCell, std::vector<SearchCell>, compare> queue;

    queue.push(intersectionCells[intersect_id_start]);

    // continue checking until every intersection is checked
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
        if (currentCell.id == intersect_id_end)
        {
            unsigned navigator = intersect_id_end;
            while (navigator != intersect_id_start)
            {
                path.push_back(intersectionCells[navigator].parent_street_segment);
                navigator = intersectionCells[navigator].parent_intersection;
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

    }
    return path;
}

std::vector<unsigned> find_path_to_point_of_interest(const unsigned intersect_id_start,
        const std::string point_of_interest_name,
        const double turn_penalty)
{
    std::vector<unsigned> current_path;
    std::vector<unsigned> path;
    std::vector<unsigned> poiList;
    std::vector<nearestPOI> targetList;
    double minimum_time = std::numeric_limits<float>::infinity();
    double compute_time = 0;
    unsigned number_of_poi;

    // retrieve the list of POIs based on given name
    poiList = poiFromName[point_of_interest_name];
    targetList.resize(poiList.size());

    if (poiList.empty())
        return path;

    // load the target list with distances
    for (unsigned i = 0; i < poiList.size(); ++i)
    {
        targetList[i].poi_id = poiList[i];
        targetList[i].delta_distance = find_distance_between_two_points(intersectionInformation[intersect_id_start].intersection_position, m1_POI[poiList[i]].LatLon_position);
    }

    // sort the list from closest to furthest
    std::sort(targetList.begin(), targetList.end(), myLeast());

    if (targetList.size() > 10)
        number_of_poi = 10;
    else
        number_of_poi = targetList.size();

    // use Dijkstra to find the closest intersection by comparing the first 10 (or less) POI
    for (unsigned i = 0; i < number_of_poi; ++i)
    {
        unsigned target = find_closest_intersection(m1_POI[targetList[i].poi_id].LatLon_position);
        current_path = find_path_between_intersections(intersect_id_start, target, turn_penalty);

        if (!current_path.empty())
        {
            compute_time = compute_path_travel_time(current_path, turn_penalty);

            if (compute_time < minimum_time)
            {
                minimum_time = compute_time;
                path = current_path;
                DESIRED_POI_NUMBER = targetList[i].poi_id;
            }
        }
    }

    return path;
}