#include "m1.h"
#include "m3.h"
#include "StreetsDatabaseAPI.h"
#include "intersectionInfo.h"
#include "featureInfo.h"
#include "DataStructures.h"
#include "POIInfo.h"

#include <vector>
#include <unordered_map>
#include <cmath>
#include <math.h>
#include <algorithm>
#include <string>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <iostream>
#include <boost/foreach.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef std::pair<point, unsigned> point_info;

///////////////////////////////////////////////////////////
// *Container with index same as intersection index*     //
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
std::vector<intersectionInfo> intersectionInformation;
std::vector<std::vector<unsigned>> allStreetIntersections;
std::vector<std::vector<unsigned>> intersectionStreetSeg;
std::vector<SearchCell> intersectionCells;
std::vector<SearchCell> intersectionCells1;
std::vector<SearchCell> intersectionCells2;
std::vector<SearchCell> intersectionCells3;
std::vector<SearchCell> intersectionCells4;
std::vector<SearchCell> intersectionCells5;
std::vector<SearchCell> intersectionCells6;
std::vector<SearchCell> intersectionCells7;
std::vector<SearchCell> intersectionCells8;
std::vector<SearchCell> intersectionCells9;
std::vector<SearchCell> intersectionCells10;
std::vector<SearchCell> intersectionCells11;
std::vector<SearchCell> intersectionCells12;
std::vector<SearchCell> intersectionCells13;
std::vector<SearchCell> intersectionCells14;
std::vector<SearchCell> intersectionCells15;
std::vector<SearchCell> intersectionCells16;

///////////////////////////////////////////////////////////
// *Container with index same as street index*           //
std::vector<std::vector<unsigned>> ssSegments;
std::vector<std::vector<LatLon>> ssCurvePoint;

///////////////////////////////////////////////////////////
// *Container with street name for index*                //
///////////////////////////////////////////////////////////
std::unordered_map<std::string, std::vector<unsigned>> streetIdsFromName;
std::unordered_map<std::string, std::vector<unsigned>> intersectionIdsFromName;
std::vector<StreetSegmentInfo> streetSegmentInfo;

///////////////////////////////////////////////////////////
// *Container with poi name for index                    //
///////////////////////////////////////////////////////////
std::unordered_map<std::string, std::vector<unsigned>> poiFromName;
std::vector<POIInfo> m1_POI;

///////////////////////////////////////////////////////////
// *rTree variables used for the nearest point problem*  //
///////////////////////////////////////////////////////////
bgi::rtree< point_info, bgi::quadratic<16> > rtreePOI;
bgi::rtree< point_info, bgi::quadratic<16> > rtreeIntersection;

////////////////////////////////////////////////////////////////////////////////////
// *Vectors relating to lengths of a street/segment and the travel time as a      //
// *result of calculation of such information*                                    //
////////////////////////////////////////////////////////////////////////////////////
std::vector<double> ssLength;
std::vector<double> streetLength;
std::vector<double> TravelTime;

/*============================================================================*/
/*============================================================================*/

/*============================================================================*/
bool load_map(std::string map_path)
{

    bool load = loadStreetsDatabaseBIN(map_path);
    if (!load)
        return false;

    unsigned number_of_intersections = getNumberOfIntersections();
    unsigned number_of_streets = getNumberOfStreets();
    unsigned number_of_street_segments = getNumberOfStreetSegments();
    unsigned number_of_features = getNumberOfFeatures();
    unsigned number_of_poi = getNumberOfPointsOfInterest();

    intersectionInformation.resize(number_of_intersections);
    allStreetIntersections.resize(number_of_intersections);

    /////////////////////////////////////////////////////////////////////////////////////
    // *The following loops reallocate/load information regarding each intersection    //
    // *by its index into the corresponding vector*/                                   //
    /////////////////////////////////////////////////////////////////////////////////////

    for (unsigned i = 0; i < number_of_intersections; ++i)
    {

        /*rTree for intersections*/
        point p;
        p.set<0>(getIntersectionPosition(i).lat());
        p.set<1>(getIntersectionPosition(i).lon());
        rtreeIntersection.insert(std::make_pair(p, i));

        std::vector<unsigned> tempSegmentIndex;
        /////////////////////////////////////////////////////////////////////////////////
        // *load intersection index into intersectionInformation struct which contains //
        // *all the information relating one intersection*/                            //
        /////////////////////////////////////////////////////////////////////////////////

        intersectionInformation[i].intersection_index = i;
        intersectionInformation[i].intersection_position = getIntersectionPosition(i);
        /////////////////////////////////////////////////////////////////////////////////
        // *load information of each segment into the intersection it belongs to       //
        // *information such as segment index, segment information struct, and street  //
        // *name is loaded here*/                                                      //
        /////////////////////////////////////////////////////////////////////////////////

        for (unsigned j = 0; j < getIntersectionStreetSegmentCount(i); ++j)
        {
            auto const ssID = getIntersectionStreetSegment(i, j);
            auto const ssInformation = getStreetSegmentInfo(ssID);
            intersectionInformation[i].street_segment_index.push_back(ssID);
            intersectionInformation[i].street_segment_info.push_back(ssInformation);
            intersectionInformation[i].intersection_street_names.push_back(getStreetName(intersectionInformation[i].street_segment_info[j].streetID));
            allStreetIntersections[static_cast<unsigned> (intersectionInformation[i].street_segment_info[j].streetID)].push_back(i);
            tempSegmentIndex.push_back(ssID);
        }
        intersectionStreetSeg.push_back(tempSegmentIndex);
    }

    ssSegments.resize(number_of_streets);

    ssCurvePoint.resize(number_of_street_segments);
    ////////////////////////////////////////////////////////////////////////////////////
    // *Information regarding each segment but disregarding the intersection it belongs/
    // *to is loaded into the containers here*/                                       //
    ////////////////////////////////////////////////////////////////////////////////////   

    for (unsigned i = 0; i < ssCurvePoint.size(); ++i)
    {

        auto const tempSegmentInfo = getStreetSegmentInfo(i);
        /*A vector of segment index containing the corresponding segment information
         * struct is loaded again*/
        streetSegmentInfo.push_back(tempSegmentInfo);

        /*Number of curve points belonging into one segment is loaded into a 
         * vector ordered by segment index is loaded here*/
        for (unsigned j = 0; j < streetSegmentInfo[i].curvePointCount; ++j)
        {
            ssCurvePoint[i].push_back(getStreetSegmentCurvePoint(i, j));
        }

        ssSegments[streetSegmentInfo[i].streetID].push_back(i);
        StreetSegmentInfo segInfo = streetSegmentInfo[i];
        double segLength = 0;

        //start and end coordinates of the street segment
        LatLon start = getIntersectionPosition(segInfo.from);
        LatLon end = getIntersectionPosition(segInfo.to);

        //3 cases to calculate the street segment length
        //1st case, there are no curves, the street is a straight line
        if (segInfo.curvePointCount == 0)
            segLength = find_distance_between_two_points(start, end);
            //2nd case, there is exactly 1 curve point
        else if (segInfo.curvePointCount == 1)
        {
            LatLon curvePoint = ssCurvePoint[i][0];
            segLength = (find_distance_between_two_points(start, curvePoint) + find_distance_between_two_points(curvePoint, end));
        }//3rd case, there are more than 1 curve points
        else
        {
            //calculate the total distance between curve point 1 and curve point N
            //sum of the distance between two different curve points x y
            for (unsigned j = 0; j < segInfo.curvePointCount - 1; j++)
            {
                segLength += find_distance_between_two_points(ssCurvePoint[i][j], ssCurvePoint[i][j + 1]);
            }
            //add the distance between start and curve point1, curve pointN and end to the total distance
            segLength += (find_distance_between_two_points(start, ssCurvePoint[i][0]) + find_distance_between_two_points(ssCurvePoint[i][segInfo.curvePointCount - 1], end));
        }
        /*Load the end length result into its vector*/
        ssLength.push_back(segLength);
        TravelTime.push_back((ssLength[i]) / (streetSegmentInfo[i].speedLimit) * 3.6);
    }

    /*Information regarding each street is loaded here*/
    for (unsigned i = 0; i < number_of_streets; ++i)
    {

        std::vector<unsigned> street_index;
        std::string streetName = getStreetName(i);

        auto find_key = streetIdsFromName.find(streetName);


        if (find_key == streetIdsFromName.end())
        {
            street_index.push_back(i);
            streetIdsFromName.insert(std::make_pair(streetName, street_index));
        }
        else
        {
            /*If name already exists, combine both vectors into one*//*Load street index into it's corresponding name*/
            find_key->second.push_back(i);
        }
        std::vector<unsigned> intersectionID = allStreetIntersections[static_cast<unsigned> (i)];

        std::unordered_map<std::string, std::vector<unsigned>>::iterator iter = intersectionIdsFromName.find(streetName);
        if (iter == intersectionIdsFromName.end())
            intersectionIdsFromName.insert(std::make_pair(streetName, intersectionID));
            /*If name already exists, combine both vectors into one*//*Load street index into it's corresponding name*/
        else
        {
            intersectionIdsFromName[streetName].insert(intersectionIdsFromName[streetName].end(), intersectionID.begin(), intersectionID.end());
        }

        double length = 0;

        //find the total distance for all segments on one street
        for (unsigned j = 0; j < ssSegments[i].size(); ++j)
        {

            length += ssLength[ssSegments[i][j]];
        }
        /*Total length of a street*/
        streetLength.push_back(length);

    }

    /*rTree for POI*/
    for (unsigned i = 0; i < number_of_poi; ++i)
    {
        point p;

        p.set<0>(getPointOfInterestPosition(i).lat());
        p.set<1>(getPointOfInterestPosition(i).lon());

        rtreePOI.insert(std::make_pair(p, i));

        POIInfo tempPOI;
        tempPOI.POItype = getPointOfInterestType(i);
        tempPOI.POIname = getPointOfInterestName(i);
        tempPOI.LatLon_position = getPointOfInterestPosition(i);
        tempPOI.POIOSMID = getPointOfInterestOSMNodeID(i);

        m1_POI.push_back(tempPOI);

        auto iter = poiFromName.find(tempPOI.POIname);
        if (iter == poiFromName.end())
        {
            std::vector<unsigned> poiIndex;
            poiIndex.push_back(i);

            poiFromName.insert(std::make_pair(tempPOI.POIname, poiIndex));
        }
        else
        { // name is found in current map
            // push the ID into vector corresponding to its street name index
            iter -> second.push_back(i);
        }
    }

    for (unsigned i = 0; i < number_of_intersections; i++)
    {
        SearchCell currentIntersection;

        currentIntersection.id = i;
        currentIntersection.F = DBL_MAX;
        currentIntersection.G = 0;
        currentIntersection.H = 0;
        currentIntersection.parent_intersection = 0;
        currentIntersection.parent_street_segment = 0;
        currentIntersection.visited = false;

        for (unsigned j = 0; j < intersectionInformation[i].street_segment_index.size(); j++)
        {
            cellInfo currentConnection;

            currentConnection.streetsegment_id = intersectionInformation[i].street_segment_index[j];
            currentConnection.travelTime = TravelTime[currentConnection.streetsegment_id];

            if (streetSegmentInfo[currentConnection.streetsegment_id].oneWay)
            {

                if (streetSegmentInfo[currentConnection.streetsegment_id].from == currentIntersection.id)
                {
                    currentConnection.intersection_id = streetSegmentInfo[currentConnection.streetsegment_id].to;
                    currentIntersection.directed_connections.push_back(currentConnection);
                }

            }
            else
            {
                if (streetSegmentInfo[currentConnection.streetsegment_id].from == currentIntersection.id)
                {
                    currentConnection.intersection_id = streetSegmentInfo[currentConnection.streetsegment_id].to;
                    currentIntersection.directed_connections.push_back(currentConnection);
                }
                else
                {
                    currentConnection.intersection_id = streetSegmentInfo[currentConnection.streetsegment_id].from;
                    currentIntersection.directed_connections.push_back(currentConnection);
                }
            }
        }
        intersectionCells.push_back(currentIntersection);
        intersectionCells1.push_back(currentIntersection);
        intersectionCells2.push_back(currentIntersection);
        intersectionCells3.push_back(currentIntersection);
        intersectionCells4.push_back(currentIntersection);
        intersectionCells5.push_back(currentIntersection);
        intersectionCells6.push_back(currentIntersection);
        intersectionCells7.push_back(currentIntersection);
        intersectionCells8.push_back(currentIntersection);
        intersectionCells9.push_back(currentIntersection);
        intersectionCells10.push_back(currentIntersection);
        intersectionCells11.push_back(currentIntersection);
        intersectionCells12.push_back(currentIntersection);
        intersectionCells13.push_back(currentIntersection);
        intersectionCells14.push_back(currentIntersection);
        intersectionCells15.push_back(currentIntersection);
        intersectionCells16.push_back(currentIntersection);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////
// *End of loading data structures using public member function provided          //
//////////////////////////////////////////////////////////////////////////////////// 

void close_map()
{

    intersectionInformation.clear();
    intersectionIdsFromName.clear();
    intersectionStreetSeg.clear();
    intersectionCells.clear();
    intersectionCells1.clear();
    intersectionCells2.clear();
    intersectionCells3.clear();
    intersectionCells4.clear();
    intersectionCells5.clear();
    intersectionCells6.clear();
    intersectionCells7.clear();
    intersectionCells8.clear();
    intersectionCells9.clear();
    intersectionCells10.clear();
    intersectionCells11.clear();
    intersectionCells12.clear();
    intersectionCells13.clear();
    intersectionCells14.clear();
    intersectionCells15.clear();
    intersectionCells16.clear();
    allStreetIntersections.clear();
    streetSegmentInfo.clear();
    ssCurvePoint.clear();
    ssSegments.clear();
    streetIdsFromName.clear();
    rtreePOI.clear();
    rtreeIntersection.clear();
    ssLength.clear();
    streetLength.clear();
    TravelTime.clear();
    closeStreetDatabase();


}

std::vector<unsigned> find_street_ids_from_name(std::string street_name)
{
    return streetIdsFromName[street_name];
}

//Returns the street segments for the given intersection 

std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id)
{

    return intersectionStreetSeg[intersection_id];
}

//Returns the street names at the given intersection (includes duplicate street names in returned vector)

std::vector<std::string> find_intersection_street_names(unsigned intersection_id)
{

    return intersectionInformation[intersection_id].intersection_street_names;
}

//Returns true if you can get from intersection1 to intersection2 using a single street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself

bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2)
{

    bool directlyConnected = false;
    std::vector<unsigned> street1_segs = intersectionInformation[intersection_id1].street_segment_index; //street segments of intersection 1 
    std::vector<unsigned> street2_segs = intersectionInformation[intersection_id2].street_segment_index; //street segments of intersection 2

    unsigned commonSeg = 0;

    for (unsigned i = 0; i < street1_segs.size(); i++)
    { //iterate to find common segment for street 1 and 2
        for (unsigned j = 0; j < street2_segs.size(); j++)
        {
            if (street1_segs[i] == street2_segs[j])
            {

                directlyConnected = true;
                commonSeg = street1_segs[i];

            }
        }
    }

    StreetSegmentInfo street = getStreetSegmentInfo(commonSeg); //get common segment info 
    if (street.oneWay == true) //check one way
        if (street.from == intersection_id2) //check if intersection 2 is the starting point 
            directlyConnected = false;

    if (intersection_id1 == intersection_id2) //check corner case: self connect
        directlyConnected = true;

    return directlyConnected;
}

//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections

std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id)
{

    std::vector<IntersectionIndex> intersectionsInSegment;
    std::vector<unsigned> streetSegs = intersectionInformation[intersection_id].street_segment_index;

    IntersectionIndex intersectionID = intersection_id; //cast the input data type

    for (unsigned i = 0; i < streetSegs.size(); i++)
    {

        StreetSegmentInfo street = streetSegmentInfo[streetSegs[i]];

        //if one-way, check wrong way(from and to points)
        if (street.oneWay)
        {
            if (street.from == intersectionID)
                intersectionsInSegment.push_back(street.to); //valid one-way direction, add intersection ID to vector 
        }//not one-way
        else
        {
            if (street.from == intersectionID)
                intersectionsInSegment.push_back(street.to);
            else
                intersectionsInSegment.push_back(street.from);
        }
    }
    //NEED TO CHECK DUPLICATE(case with self connect intersection/two seg reach same intersection)
    std::sort(intersectionsInSegment.begin(), intersectionsInSegment.end());
    intersectionsInSegment.erase(std::unique(intersectionsInSegment.begin(), intersectionsInSegment.end()), intersectionsInSegment.end());
    return intersectionsInSegment;
}

//Returns all street segments for the given street

std::vector<unsigned> find_street_street_segments(unsigned street_id)
{
    return ssSegments[street_id];
}

//Returns all intersections along the a given street

std::vector<unsigned> find_all_street_intersections(unsigned street_id)
{
    std::vector<StreetSegmentIndex> IntersectionsInStreet = allStreetIntersections[street_id];
    IntersectionsInStreet.erase(std::unique(IntersectionsInStreet.begin(), IntersectionsInStreet.end()), IntersectionsInStreet.end());
    return IntersectionsInStreet;
}

//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
//However street names are not guarenteed to be unique, so more than 1 intersection id may exist

std::vector<unsigned> find_intersection_ids_from_street_names(std::string street_name1, std::string street_name2)
{
    std::vector<unsigned> commonIntersectionID;
    std::sort(intersectionIdsFromName[street_name2].begin(), intersectionIdsFromName[street_name2].end());
    std::set_intersection(intersectionIdsFromName[street_name1].begin(),
            intersectionIdsFromName[street_name1].end(),
            intersectionIdsFromName[street_name2].begin(),
            intersectionIdsFromName[street_name2].end(),
            std::back_inserter(commonIntersectionID));

    std::sort(commonIntersectionID.begin(), commonIntersectionID.end());
    commonIntersectionID.erase(std::unique(commonIntersectionID.begin(), commonIntersectionID.end()), commonIntersectionID.end());
    return commonIntersectionID;
}

//Retruns the distance between two coordinates in meters

double find_distance_between_two_points(LatLon point1, LatLon point2)
{
    double latAvg = (point1.lat() + point2.lat()) / 2.0;
    double deltaX = (point1.lon() - point2.lon()) * cos(latAvg * DEG_TO_RAD) * DEG_TO_RAD;
    double deltaY = (point1.lat() - point2.lat()) * DEG_TO_RAD;
    return (EARTH_RADIUS_IN_METERS * sqrt(pow(deltaY, 2) + pow(deltaX, 2)));
}

//Returns the length of the given street segment in meters

double find_street_segment_length(unsigned street_segment_id)
{
    return ssLength[street_segment_id];
}

//Returns the length of the specified street in meters

double find_street_length(unsigned street_id)
{
    return streetLength[street_id];
}

//Returns the travel time to drive a street segment in seconds (time = distance/speed_limit)

double find_street_segment_travel_time(unsigned street_segment_id)
{
    return TravelTime[street_segment_id];
}

//Returns the nearest point of interest to the given position

unsigned find_closest_point_of_interest(LatLon my_position)
{

    double newDistance;
    double distance;
    unsigned closestPOIIndex = 0;
    std::vector<point_info> nearest_hundred_points;
    point p;
    p.set<0>(my_position.lat());
    p.set<1>(my_position.lon());

    rtreePOI.query(bgi::nearest(p, 75), std::back_inserter(nearest_hundred_points));
    point inst;
    double x_inst = nearest_hundred_points[0].first.get<0>();
    double y_inst = nearest_hundred_points[0].first.get<1>();
    newDistance = find_distance_between_two_points(LatLon(x_inst, y_inst), my_position);
    distance = newDistance;

    for (unsigned i = 0; i < nearest_hundred_points.size(); ++i)
    {
        double x = nearest_hundred_points[i].first.get<0>();
        double y = nearest_hundred_points[i].first.get<1>();
        newDistance = find_distance_between_two_points(LatLon(x, y), my_position);
        if (distance > newDistance)
        {
            closestPOIIndex = nearest_hundred_points[i].second;
            distance = newDistance;
        }
    }
    return closestPOIIndex;
}

//Returns the the nearest intersection to the given position

unsigned find_closest_intersection(LatLon my_position)
{

    double newDistance;
    double distance;
    unsigned closestIntersectionIndex = 0;
    std::vector<point_info> nearest_hundred_points;
    point p;
    p.set<0>(my_position.lat());
    p.set<1>(my_position.lon());

    rtreeIntersection.query(bgi::nearest(p, 75), std::back_inserter(nearest_hundred_points));
    point inst;
    double x_inst = nearest_hundred_points[0].first.get<0>();
    double y_inst = nearest_hundred_points[0].first.get<1>();
    newDistance = find_distance_between_two_points(LatLon(x_inst, y_inst), my_position);
    distance = newDistance;

    for (unsigned i = 0; i < nearest_hundred_points.size(); ++i)
    {
        double x = nearest_hundred_points[i].first.get<0>();
        double y = nearest_hundred_points[i].first.get<1>();
        newDistance = find_distance_between_two_points(LatLon(x, y), my_position);
        if (distance > newDistance)
        {
            closestIntersectionIndex = nearest_hundred_points[i].second;
            distance = newDistance;
        }
    }
    return closestIntersectionIndex;
}

