/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m2_helper.h"
#include "SearchCell.h"
#include <unordered_map>
#include <vector>
#include <string>
std::unordered_map<std::string, std::vector<unsigned>> street_Id_From_Name;

extern std::vector<double> TravelTime;
extern std::vector<intersectionInfo> intersectionInformation;
extern std::vector<StreetSegmentInfo> streetSegmentInfo;
extern std::vector<double> ssLength;
extern std::unordered_map<std::string, std::vector<unsigned>> streetIdsFromName;
extern std::unordered_map<std::string, std::vector<unsigned>> poiFromName;
extern unsigned DESIRED_POI_NUMBER;

//============================================================================//
// * Initializations used to keep track of the map border coordinates         //
//============================================================================//
t_bound_box INITIAL_MAP_COORDS;
double maxLon = -180;
double minLon = 180;
double maxLat = -90;
double minLat = 90;
double avgLat;

unsigned click_on_intersection = 0;
unsigned click_on_POI = 0;
unsigned click_path_intersection = 0;
unsigned search_path_to_POI = 0;
unsigned click_path_to_POI = 0;

//============================================================================//
// * Find street name by street index                                         //
//============================================================================//
std::vector<string> allStreetNames;
//============================================================================//
// * Colors to be used for the drawing of the contents                        //
//============================================================================//
const t_color highwayColor = t_color(254, 216, 157);
const t_color streetColor = t_color(255, 255, 255);
const t_color parkColor = t_color(203, 230, 163);
const t_color beachColor = t_color(250, 242, 199);
const t_color waterColor = t_color(163, 204, 255);
const t_color shorelineColor = t_color(234, 234, 234);
const t_color buildingColor = t_color(244, 235, 214);

//============================================================================//
// * Containers containing different type of street                           //
//============================================================================//
std::vector<segmentInfo> localStreets;
std::vector<segmentInfo> cityStreets;
std::vector<segmentInfo> highway;
std::vector<segmentInfo> ALLSTREETS;
const float HIGHWAY_SPEED_LIMIT = 75;
const float CITY_STREET_SPEED_LIMIT = 50;
const float LOCAL_STREET_SPEED_LIMI = 20;

//============================================================================//
// * Containers containing the information of each feature by its type        //
//============================================================================//
std::vector<featureInfo> featureUnknown;
std::vector<featureInfo> featurePark;
std::vector<featureInfo> featureBeach;
std::vector<featureInfo> featureLake;
std::vector<featureInfo> featureRiver;
std::vector<featureInfo> featureIsland;
std::vector<featureInfo> featureShoreline;
std::vector<featureInfo> featureBuilding;
std::vector<featureInfo> featureGreenspace;
std::vector<featureInfo> featureGolfcourse;
std::vector<featureInfo> featureStream;

//============================================================================//
// * Containers containing the information of each feature by its surface     //
// * area                                                                     //
//============================================================================//
std::vector<featureInfo> featureArea0;
std::vector<featureInfo> featureArea1;
std::vector<featureInfo> featureArea2;
std::vector<featureInfo> featureArea3;
std::vector<featureInfo> featureArea4;
std::vector<featureInfo> featureArea5;

//============================================================================//
// * Containers containing the information of each POI, the index is the      //
// * same as the POI index                                                    //
//============================================================================//
std::vector<POIInfo> restaurantPOI;
std::vector<POIInfo> fastFoodPOI;
std::vector<POIInfo> fuelPOI;
std::vector<POIInfo> cafePOI;
std::vector<POIInfo> busStationPOI;
std::vector<POIInfo> hospitalPOI;
std::vector<POIInfo> collegePOI;
std::vector<POIInfo> allPOI;
//============================================================================//
// * file path of each point of interest type logo                            //
//============================================================================//
char* RESTAURANT_LOGO_PATH = "libstreetmap/resources/restaurant.png";
char* CAFE_LOGO_PATH = "libstreetmap/resources/cafe.png";
char* FUEL_LOGO_PATH = "libstreetmap/resources/fuel.png";
char* BANK_LOGO_PATH = "libstreetmap/resources/bank.png";
char* FAST_FOOD_LOGO_PATH = "libstreetmap/resources/fast-food.png";
char* NIGHT_LIFE_LOGO_PATH = "libstreetmap/resources/night-life.png";
char* BUS_STATION_LOGO_PATH = "libstreetmap/resources/bus-station.png";
char* PIN_LOGO_PATH = "libstreetmap/resources/pin.png";
char* HEALTH_LOGO_PATH = "libstreetmap/resources/health.png";
char* EDUCATION_LOGO_PATH = "libstreetmap/resources/education.png";
char* DEFAULT_LOGO_PATH = "libstreetmap/resources/default.png";

//============================================================================//
// * Global variables for                                                     // 
//============================================================================//
const std::string ARROW = "\u2190";
unsigned ZOOM_LEVEL;
float widthFactor = 1;

//============================================================================//
// * Variables for search bar                                                 // 
//============================================================================//
bool ifFind = false;
bool ifPath = false;
bool draw_second = false;
bool firstIntersection = false;
unsigned index1 = 0;
unsigned index2 = 0;
std::string key1;
std::string key2;
std::string key3;
std::string key4;
std::string name1;
std::string name2;
std::string name3;
std::string name4;
bool firstEnter = false;
std::vector<unsigned> streetID1;
std::vector<unsigned> streetID2;
unsigned click_intersection_index = 0;
unsigned click_POI_index = 0;
bool first_click = false;

////////////////////////////////////////////////////////////////////////////////
// * Find the four corners of the current city based on the max/min positions //
// * of the intersections                                                     //
////////////////////////////////////////////////////////////////////////////////

t_bound_box find_map_coordinates(void)
{

    for (unsigned i = 0; i < intersectionInformation.size(); ++i)
    {
        double currentLat = intersectionInformation[i].intersection_position.lat();
        double currentLon = intersectionInformation[i].intersection_position.lon();

        if (currentLat > maxLat)
            maxLat = currentLat;

        if (currentLat < minLat)
            minLat = currentLat;

        if (currentLon > maxLon)
            maxLon = currentLon;

        if (currentLon < minLon)
            minLon = currentLon;
    }

    avgLat = (minLat + maxLat) / 2.0;

    float x1 = (minLon) * cos(avgLat * DEG_TO_RAD);
    float x2 = (maxLon) * cos(avgLat * DEG_TO_RAD);
    float y1 = (minLat);
    float y2 = (maxLat);

    INITIAL_MAP_COORDS.left() = x1;
    INITIAL_MAP_COORDS.right() = x2;
    INITIAL_MAP_COORDS.bottom() = y1;
    INITIAL_MAP_COORDS.top() = y2;

    return INITIAL_MAP_COORDS;
}

////////////////////////////////////////////////////////////////////////////////
// * Conversion between two sets of coordinates                               //
////////////////////////////////////////////////////////////////////////////////

t_point LatLon_to_xy(LatLon latlon)
{

    t_point xy;

    xy.x = (latlon.lon()) * cos(avgLat * DEG_TO_RAD);
    xy.y = (latlon.lat());

    return (xy);
}

////////////////////////////////////////////////////////////////////////////////
// * Converts the x y coordinates to lat lon coordinates                      //
////////////////////////////////////////////////////////////////////////////////

LatLon xy_to_LatLon(t_point xy)
{

    LatLon latlon;

    float lon = (xy.x) / cos(avgLat * DEG_TO_RAD);
    float lat = xy.y;
    latlon = LatLon(lat, lon);
    return (latlon);
}

////////////////////////////////////////////////////////////////////////////////
// * Find the area to a polygon using x & y points of                         //
////////////////////////////////////////////////////////////////////////////////

float find_polygon_area(std::vector<float> x, std::vector<float> y, unsigned numPoint)
{


    float area = 0;
    unsigned j = numPoint - 1;

    for (unsigned i = 0; i < numPoint; ++i)
    {
        area = area + (x[j] + x[i]) * (y[j] - y[i]);
        j = i;
    }
    return abs(area / 2);
}

bool if_in_visible_world(t_bound_box boundary, t_point location)
{

    if (location.x > boundary.left() && location.x < boundary.right() && location.y > boundary.bottom() && location.y < boundary.top())
        return true;

    return false;

}

std::string direction_between_two_segments(t_point point1, t_point point2, t_point point3)
{

    // middle - first
    // dy > 0 = north
    // dx > 0 = east
    float delta_x_1 = point2.x - point1.x;
    float delta_y_1 = point2.y - point1.y;
    float delta_x_2 = point3.x - point2.x;
    float delta_y_2 = point3.y - point2.y;

    // moving left && right
    if (abs(delta_x_1) > abs(delta_y_1))
    {
        // moving right
        if (delta_x_1 > 0)
        {
            if (delta_y_2 > 0)
                return ("Turn left onto ");
            else if (delta_y_2 < 0)
                return ("Turn right onto ");
            else
                return ("Continue straight onto ");
        }
        else if (delta_x_1 < 0)
        {
            if (delta_y_2 > 0)
                return ("Turn right onto ");
            else if (delta_y_2 < 0)
                return ("Turn left onto ");
            else
                return ("Continue straight onto ");
        }
        else
            return ("Continue straight onto ");
    }
    else if (abs(delta_y_1) > abs(delta_x_1))
    {
        if (delta_y_1 > 0)
        {
            if (delta_x_2 > 0)
                return ("Turn right onto ");
            else if (delta_x_2 < 0)
                return ("Turn left onto ");
            else
                return ("Continue straight onto ");
        }
        else if (delta_y_1 < 0)
        {
            if (delta_x_2 > 0)
                return ("Turn left onto ");
            else if (delta_x_2 < 0)
                return ("Turn right onto ");
            else
                return ("Continue straight onto ");
        }
        else
            return ("Continue straight onto ");
    }
    else
    {
        if ((abs(delta_x_2) - abs(delta_x_1) < 0.2) && (abs(delta_y_2) - abs(delta_y_1) < 0.2))
            return ("Continue straight onto ");
        else if (delta_y_2 > 0)
            return ("Turn left onto ");
        else if (delta_y_2 < 0)
            return ("Turn right onto ");
        else
            return ("Continue straight onto ");
    }

}

std::vector<std::string> direction_of_path(std::vector<unsigned> path)
{
    std::vector<std::string> direction_instruction;

    direction_instruction.push_back("Starting travel route: ");
    if (path.size() == 1)
    {
        std::string temp;
        temp = "Continue onto ";
        temp.append(getStreetName(streetSegmentInfo[path[0]].streetID));
        direction_instruction.push_back(temp);
    }
    else
    {
        for (unsigned i = 0; i < path.size(); ++i)
        {

            if (i > 0)
            {
                if (streetSegmentInfo[path[i]].streetID != streetSegmentInfo[path[i - 1]].streetID)
                {
                    std::string temp;
                    t_point point1 = ALLSTREETS[path[i - 1]].curvePointPosition[ALLSTREETS[path[i - 1]].information.curvePointCount + 1];
                    std::cout << point1.x << "     " << point1.y << std::endl;
                    t_point point2 = ALLSTREETS[path[i]].curvePointPosition[1];
                    std::cout << point2.x << "     " << point2.y << std::endl;
                    t_point point3 = ALLSTREETS[path[i]].curvePointPosition[ALLSTREETS[path[i]].information.curvePointCount + 1];
                    std::cout << point3.x << "     " << point3.y << std::endl;
                    temp = direction_between_two_segments(point1, point2, point3);
                    temp.append(getStreetName(streetSegmentInfo[path[i]].streetID));
                    //temp.append(" for ");
                    //temp.append(std::to_string((int) ALLSTREETS[path[i + 1]].segmentLength));
                    //temp.append(" m");
                    direction_instruction.push_back(temp);
                }
            }
            else if (i == 0)
            {
                std::string temp;
                //temp.append(std::to_string(compute_path_travel_time(path, 15)));
                //temp.append(" s");
                //temp.append("\n");

                temp.append("Get on ");
                temp.append(getStreetName(streetSegmentInfo[path[0]].streetID));
                //temp.append(" for ");
                //temp.append(std::to_string((int) ALLSTREETS[path[i]].segmentLength));
                //temp.append(" m");
                direction_instruction.push_back(temp);
            }
        }
    }
    return direction_instruction;

}

void load_data(void)
{

    ////////////////////////////////////////////////////////////////////////////
    // * Store all street names in to a vector with index same as the         //
    // * street index                                                         //
    ////////////////////////////////////////////////////////////////////////////
    for (unsigned i = 0; i < getNumberOfStreets(); ++i)
    {

        allStreetNames.push_back(getStreetName(i));

    }

    ////////////////////////////////////////////////////////////////////////////
    // * Load street segment information based on speed limits to distinguish  //
    // * between different types of streets                                    //
    ////////////////////////////////////////////////////////////////////////////
    for (unsigned i = 0; i < getNumberOfStreetSegments(); ++i)
    {

        segmentInfo temp;

        temp.information = getStreetSegmentInfo(i);
        temp.index = i;

        unsigned curveCount = temp.information.curvePointCount;

        LatLon tempFrom = getIntersectionPosition(temp.information.from);
        LatLon tempTo = getIntersectionPosition(temp.information.to);

        t_point xyFrom, xyTo;

        xyFrom = LatLon_to_xy(tempFrom);
        xyTo = LatLon_to_xy(tempTo);

        temp.curvePointPosition.push_back(xyFrom);

        temp.segmentLength = 0;

        if (curveCount == 0)
        {

            temp.curvePointPosition.push_back(xyTo);

            float curveLength = sqrt(pow((xyFrom.x - xyTo.x), 2) + pow((xyFrom.y - xyTo.y), 2));

            temp.curveLength.push_back(curveLength);
            temp.segmentLength = curveLength;

            float angle = atan2((xyTo.y - xyFrom.y), (xyTo.x - xyFrom.x)) / DEG_TO_RAD;

            if (angle > 90)
                angle = angle - 180;
            else if (angle < 90)
                angle = angle + 180;
            settextrotation(angle);

            temp.curvePointAngle.push_back(angle);

        }
        else
        {

            for (unsigned j = 0; j < curveCount - 1; j++)
            {

                LatLon tempPoint = getStreetSegmentCurvePoint(temp.index, j + 1);
                xyFrom = LatLon_to_xy(tempPoint);
                temp.curvePointPosition.push_back(xyFrom);

                float curveLength = sqrt(pow((xyFrom.x - temp.curvePointPosition[j].x), 2) + pow((xyFrom.y - temp.curvePointPosition[j].y), 2));

                temp.curveLength.push_back(curveLength);
                temp.segmentLength += curveLength;

                float angle = atan2((xyTo.y - xyFrom.y), (xyTo.x - xyFrom.x)) / DEG_TO_RAD;

                if (angle > 90)
                    angle = angle - 180;
                else if (angle < 90)
                    angle = angle + 180;

                temp.curvePointAngle.push_back(angle);

            }

            temp.curvePointPosition.push_back(xyTo);

            float curveLength = sqrt(pow((xyTo.x - xyFrom.x), 2) + pow((xyTo.y - xyFrom.y), 2));

            temp.curveLength.push_back(curveLength);
            temp.segmentLength += curveLength;

            float angle = atan2((xyTo.y - xyFrom.y), (xyTo.x - xyFrom.x)) / DEG_TO_RAD;

            if (angle > 90)
                angle = angle - 180;
            else if (angle < 90)
                angle = angle + 180;

            temp.curvePointAngle.push_back(angle);

        }
        /*--------------------------------------------------------------------*/
        /* Separate segments based on the type of streets                     */
        /*--------------------------------------------------------------------*/

        ALLSTREETS.push_back(temp);

        if (temp.information.speedLimit >= HIGHWAY_SPEED_LIMIT)
        {

            highway.push_back(temp);

        }
        else if (temp.information.speedLimit >= CITY_STREET_SPEED_LIMIT)
        {

            cityStreets.push_back(temp);

        }
        else if (temp.information.speedLimit >= LOCAL_STREET_SPEED_LIMI)
        {

            localStreets.push_back(temp);

        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // * Load every feature into the corresponding feature type               //
    ////////////////////////////////////////////////////////////////////////////
    for (unsigned i = 0; i < getNumberOfFeatures(); ++i)
    {

        featureInfo temp;
        temp.index = i;
        temp.featureName = getFeatureName(i);
        temp.featureType = getFeatureType(i);
        temp.featureOSMID = getFeatureOSMID(i);
        temp.featurePointCount = getFeaturePointCount(i);

        std::vector<float> x;
        std::vector<float> y;

        LatLon start = getFeaturePoint(i, 0);
        LatLon end = getFeaturePoint(i, (temp.featurePointCount - 1));

        for (unsigned j = 0; j < temp.featurePointCount; ++j)
        {
            t_point tempxy = LatLon_to_xy(getFeaturePoint(i, j));
            temp.featurePoint.push_back(tempxy);
            x.push_back(tempxy.x);
            y.push_back(tempxy.y);
        }

        if (start.lat() == end.lat() && start.lon() == end.lon())
        {

            temp.polygon = true;

            float polygonArea = find_polygon_area(x, y, temp.featurePointCount);
            temp.area = polygonArea;

        }
        //std::cout << temp.area << std::endl;
        auto type = temp.featureType;
        switch (type)
        {
            case Unknown:
                featureUnknown.push_back(temp);
                break;
            case Park:
                featurePark.push_back(temp);
                break;
            case Beach:
                featureBeach.push_back(temp);
                break;
            case Lake:
                featureLake.push_back(temp);
                break;
            case River:
                featureRiver.push_back(temp);
                break;
            case Island:
                featureIsland.push_back(temp);
                break;
            case Shoreline:
                featureShoreline.push_back(temp);
                break;
            case Building:
                featureBuilding.push_back(temp);
                break;
            case Greenspace:
                featureGreenspace.push_back(temp);
                break;
            case Golfcourse:
                featureGolfcourse.push_back(temp);
                break;
            case Stream:
                featureStream.push_back(temp);
                break;
        }

        ////////////////////////////////////////////////////////////////////////
        // * Load every feature by surface area                               //
        ////////////////////////////////////////////////////////////////////////
        if (temp.polygon == true)
        {
            if (temp.area >= AREA_0 * (1 / 200))
            {

                featureArea0.push_back(temp);

            }
            else if (temp.area >= AREA_1 * (1 / 200))
            {

                featureArea1.push_back(temp);

            }
            else if (temp.area >= AREA_2 * (1 / 200))
            {

                featureArea2.push_back(temp);

            }
            else if (temp.area >= AREA_3 * (1 / 200))
            {

                featureArea3.push_back(temp);

            }
            else if (temp.area >= AREA_4 * (1 / 200))
            {

                featureArea4.push_back(temp);

            }
            else
            {

                featureArea5.push_back(temp);

            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    // * Load every point of interest by its type                             //
    ////////////////////////////////////////////////////////////////////////////
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); ++i)
    {

        POIInfo temp;

        temp.POItype = getPointOfInterestType(i);
        temp.POIname = getPointOfInterestName(i);
        temp.XY_position = LatLon_to_xy(getPointOfInterestPosition(i));
        temp.POIOSMID = getPointOfInterestOSMNodeID(i);

        std::string type = temp.POItype;
        allPOI.push_back(temp);
        if (type == "restaurant")

            restaurantPOI.push_back(temp);

        else if (type == "fast_food")

            fastFoodPOI.push_back(temp);

        else if (type == "fuel")

            fuelPOI.push_back(temp);

        else if (type == "cafe")

            cafePOI.push_back(temp);

        else if (type == "bus_station")

            busStationPOI.push_back(temp);

        else if (type == "hospital")

            hospitalPOI.push_back(temp);

        else if (type == "college")

            collegePOI.push_back(temp);

    }

}

void draw_feature(std::vector<featureInfo> featureInformation, t_color color)
{

    setcolor(color);

    for (unsigned i = 0; i < featureInformation.size(); ++i)
    {

        setlinewidth(1);
        setlinestyle(SOLID, ROUND);

        if (featureInformation[i].polygon == true)
        {
            setlinewidth(1);
            setlinestyle(SOLID, ROUND);

            t_point pointptr[static_cast<int> (featureInformation[i].featurePointCount)];

            unsigned count = featureInformation[i].featurePointCount;
            for (unsigned j = 0; j < (count - 1); ++j)
            {


                /*------------------------------------------------------------*/
                /* Draw outline of the features                               */
                /*------------------------------------------------------------*/
                drawline(featureInformation[i].featurePoint[j], featureInformation[i].featurePoint[j + 1]);
                pointptr[j].x = featureInformation[i].featurePoint[j].x;
                pointptr[j].y = featureInformation[i].featurePoint[j].y;
            }
            pointptr[count - 1].x = featureInformation[i].featurePoint[count - 1].x;
            pointptr[count - 1].y = featureInformation[i].featurePoint[count - 1].y;

            fillpoly(pointptr, static_cast<int> (featureInformation[i].featurePointCount));

        }/*--------------------------------------------------------------------*/
            /* Feature is consisted of lines only                                 */
            /*--------------------------------------------------------------------*/
        else
        {
            setlinewidth(1);
            setlinestyle(SOLID, ROUND);
            for (unsigned j = 0; j < (featureInformation[i].featurePointCount - 1); ++j)
            {

                drawline(featureInformation[i].featurePoint[j], featureInformation[i].featurePoint[j + 1]);

            }
        }
    }
}

void print_street_name(std::vector<segmentInfo> segmentInformation)
{

    setcolor(BLACK);

    std::vector<bool> ifPrinted;
    std::vector<t_point> temp;
    ifPrinted.resize(getNumberOfStreets());
    unsigned scale;
    if (ZOOM_LEVEL == 4)
        scale = 50;
    else
        scale = 30;

    for (unsigned k = 0; k < ifPrinted.size(); ++k)
        ifPrinted[k] = false;

    for (unsigned i = 0; i < segmentInformation.size(); ++i)
    {
        std::string name = allStreetNames[segmentInformation[i].information.streetID];

        if (name != "<unknown>" && ssLength[segmentInformation[i].index] > scale)
        {

            if (segmentInformation[i].information.curvePointCount == 0)
            {

                /*----------------------------------------------------------------*/
                /* Case 1: no curve points                                        */
                /*----------------------------------------------------------------*/
                if (if_in_visible_world(get_visible_world(), segmentInformation[i].curvePointPosition[0]) == true)
                {

                    setcolor(t_color(128, 128, 128));
                    double angle = segmentInformation[i].curvePointAngle[0];

                    t_bound_box text_box = t_bound_box(segmentInformation[i].curvePointPosition[0], segmentInformation[i].curvePointPosition[1]);
                    t_point midpoint = text_box.get_center();



                    if (ifPrinted[segmentInformation[i].information.streetID] == false)
                    {

                        settextrotation(angle);

                        if (angle > 90)
                            settextrotation(angle - 180);

                        drawtext(midpoint, name, 100, 100);
                        ifPrinted[segmentInformation[i].information.streetID] = true;
                    }
                    else if (segmentInformation[i].information.oneWay == true)
                    {
                        settextrotation(angle);
                        drawtext(midpoint, ARROW, 20, 20);

                    }
                }
            }/*----------------------------------------------------------------*/
                /* Case 2: at least one curve point                               */
                /*----------------------------------------------------------------*/
            else
            {

                for (unsigned j = 0; j < segmentInformation[i].curvePointPosition.size() - 1; ++j)
                {

                    if (if_in_visible_world(get_visible_world(), segmentInformation[i].curvePointPosition[j]) == true &&
                            if_in_visible_world(get_visible_world(), segmentInformation[i].curvePointPosition[j + 1]) == true)
                    {

                        temp.push_back(segmentInformation[i].curvePointPosition[j]);

                    }
                }

                if (temp.size() != 0)
                {

                    double distance = 0;

                    unsigned tempPoint = 0;

                    /*------------------------------------------------------------*/
                    /* Find the longest curve length in a segment to print name   */
                    /*------------------------------------------------------------*/
                    for (unsigned counter = 0; counter < temp.size() - 1; counter++)
                    {

                        double currentDistance = (pow((temp[counter + 1].x - temp[counter].x), 2) + pow((temp[counter + 1].y - temp[counter].y), 2));
                        if (currentDistance > distance)
                        {
                            distance = currentDistance;
                            tempPoint = counter;
                        }
                    }

                    setcolor(t_color(128, 128, 128));
                    double angle = segmentInformation[i].curvePointAngle[0];



                    t_bound_box text_box = t_bound_box(segmentInformation[i].curvePointPosition[tempPoint], segmentInformation[i].curvePointPosition[tempPoint + 1]);
                    t_point midpoint = text_box.get_center();

                    if (ifPrinted[segmentInformation[i].information.streetID] == false)
                    {
                        settextrotation(angle);
                        if (angle > 90)
                            settextrotation(angle - 180);
                        //                        if (angle > 90)
                        //                            angle = angle - 180;
                        //                        else if (angle < -90)
                        //                            angle = angle + 180;
                        //                        settextrotation(angle);
                        if (distance > 20)
                        {
                            drawtext(midpoint, name, 100, 100);
                            ifPrinted[segmentInformation[i].information.streetID] = true;
                        }
                    }
                    else if (segmentInformation[i].information.oneWay == true && ZOOM_LEVEL >= 4)
                    {
                        settextrotation(angle);
                        drawtext(midpoint, ARROW, 20, 20);

                    }

                }
            }

            temp.clear();
        }
    }
}

//void print_street_name(std::vector<segmentInfo> segmentInformation)
//{
//
//    setcolor(BLACK);
//
//    std::vector<bool> ifPrinted;
//    std::vector<t_point> temp;
//    ifPrinted.resize(getNumberOfStreets());
//
//    for (unsigned k = 0; k < ifPrinted.size(); ++k)
//        ifPrinted[k] = false;
//
//    for (unsigned i = 0; i < segmentInformation.size(); ++i)
//    {
//        if (segmentInformation[i].information.curvePointCount == 0)
//        {
//
//            /*----------------------------------------------------------------*/
//            /* Case 1: no curve points                                        */
//            /*----------------------------------------------------------------*/
//            if (if_in_visible_world(get_visible_world(), segmentInformation[i].curvePointPosition[0]) == true)
//            {
//
//                t_bound_box boundary;
//                boundary.left() = (segmentInformation[i].curvePointPosition[1].x + segmentInformation[i].curvePointPosition[0].x) / 2 - 4;
//                boundary.right() = (segmentInformation[i].curvePointPosition[1].x + segmentInformation[i].curvePointPosition[0].x) / 2 + 4;
//                boundary.bottom() = (segmentInformation[i].curvePointPosition[1].y + segmentInformation[i].curvePointPosition[0].y) / 2 - 2;
//                boundary.top() = (segmentInformation[i].curvePointPosition[1].y + segmentInformation[i].curvePointPosition[0].y) / 2 + 2;
//
//                setcolor(t_color(128, 128, 128));
//
//
//
//                if (ifPrinted[segmentInformation[i].information.streetID] == false)
//                {
//
//                    if (segmentInformation[i].information.oneWay == true)
//                    {
//                        settextrotation(segmentInformation[i].curvePointAngle[0]);
//                        drawtext_in(boundary, ARROW);
//                    }
//                    else
//                    {
//
//                        int angle = segmentInformation[i].curvePointAngle[0];
//                        if (angle > 90)
//                            angle = angle - 180;
//
//                        drawtext_in(boundary, allStreetNames[segmentInformation[i].information.streetID]);
//                        ifPrinted[segmentInformation[i].information.streetID] = true;
//                        settextrotation(0);
//                    }
//
//                }
//            }
//        }/*----------------------------------------------------------------*/
//            /* Case 2: at least one curve point                               */
//            /*----------------------------------------------------------------*/
//        else
//        {
//
//            for (unsigned j = 0; j < segmentInformation[i].curvePointPosition.size() - 1; ++j)
//            {
//
//                if (if_in_visible_world(get_visible_world(), segmentInformation[i].curvePointPosition[j]) == true &&
//                        if_in_visible_world(get_visible_world(), segmentInformation[i].curvePointPosition[j + 1]) == true)
//                {
//
//                    temp.push_back(segmentInformation[i].curvePointPosition[j]);
//
//                }
//            }
//
//            if (temp.size() != 0)
//            {
//
//                double distance = 0;
//
//                unsigned tempPoint = 0;
//
//                /*------------------------------------------------------------*/
//                /* Find the longest curve length in a segment to print name   */
//                /*------------------------------------------------------------*/
//                for (int counter = 0; counter < temp.size() - 1; counter++)
//                {
//
//                    double currentDistance = (pow((temp[counter + 1].x - temp[counter].x), 2) + pow((temp[counter + 1].y - temp[counter].y), 2));
//                    if (currentDistance > distance)
//                    {
//                        distance = currentDistance;
//                        tempPoint = counter;
//                    }
//                }
//
//                setcolor(t_color(128, 128, 128));
//                settextrotation(segmentInformation[i].curvePointAngle[0]);
//
//                t_bound_box boundary;
//                boundary.left() = (segmentInformation[i].curvePointPosition[tempPoint + 1].x + segmentInformation[i].curvePointPosition[tempPoint].x) / 2 - 4;
//                boundary.right() = (segmentInformation[i].curvePointPosition[tempPoint + 1].x + segmentInformation[i].curvePointPosition[tempPoint].x) / 2 + 4;
//                boundary.bottom() = (segmentInformation[i].curvePointPosition[tempPoint + 1].y + segmentInformation[i].curvePointPosition[tempPoint].y) / 2 - 2;
//                boundary.top() = (segmentInformation[i].curvePointPosition[tempPoint + 1].y + segmentInformation[i].curvePointPosition[tempPoint].y) / 2 + 2;
//
//                if (ifPrinted[segmentInformation[i].information.streetID] == false)
//                {
//                    if (segmentInformation[i].information.oneWay == true && ZOOM_LEVEL >= 4)
//                    {
//
//                        drawtext_in(boundary, ARROW);
//                    }
//                    else
//                    {
//
//                        int angle = segmentInformation[i].curvePointAngle[0];
//                        if (angle > 90)
//                            angle = angle - 180;
//                        else if (angle < -90)
//                            angle = angle + 180;
//                        settextrotation(angle);
//
//                        drawtext_in(boundary, allStreetNames[segmentInformation[i].information.streetID]);
//                        ifPrinted[segmentInformation[i].information.streetID] = true;
//                        settextrotation(0);
//                    }
//
//                }
//            }
//        }
//        temp.clear();
//    }
//}

void print_feature_name(std::vector<featureInfo> featureInformation)
{

    t_bound_box boundary;

    for (unsigned i = 0; i < featureInformation.size(); ++i)
    {

        auto type = featureInformation[i].featureType;

        // Set color based on feature type
        switch (type)
        {
            case Park:
                setcolor(t_color(34, 139, 34));
                break;
            case Lake:
                setcolor(t_color(135, 206, 250));
                break;
            case Building:
                setcolor(t_color(210, 105, 30));
                break;
            case River:
                setcolor(t_color(135, 206, 250));
                break;
            case Island:
                setcolor(t_color(34, 139, 34));
                break;
            case Greenspace:
                setcolor(t_color(34, 139, 34));
                break;
            case Golfcourse:
                setcolor(t_color(34, 139, 34));
                break;
            default:
                setcolor(BLACK);
                break;
        }

        // Make sure the feature is within the current viewing window
        if (if_in_visible_world(get_visible_world(), featureInformation[i].featurePoint[0]) == true)
        {

            float maxx = featureInformation[i].featurePoint[0].x;
            float maxy = featureInformation[i].featurePoint[0].y;
            float minx = featureInformation[i].featurePoint[0].x;
            float miny = featureInformation[i].featurePoint[0].y;


            for (unsigned j = 1; j < featureInformation[i].featurePointCount; ++j)
            {

                float currentx = featureInformation[i].featurePoint[j].x;
                float currenty = featureInformation[i].featurePoint[j].y;

                if (currentx > maxx)
                    maxx = currentx;

                if (currentx < minx)
                    minx = currentx;

                if (currenty > maxy)
                    maxy = currenty;

                if (currenty < miny)
                    miny = currenty;
            }

            if (featureInformation[i].polygon == 1)
            {

                boundary.left() = minx;
                boundary.right() = maxx;
                boundary.bottom() = miny;
                boundary.top() = maxy;

                if (featureInformation[i].featureName != "<noname>")
                {
                    settextrotation(0);
                    drawtext(boundary.get_center(), featureInformation[i].featureName, boundary);

                }
            }
            else
            {
                boundary.left() = minx - 2;
                boundary.right() = maxx + 2;
                boundary.bottom() = miny - 2;
                boundary.top() = maxy + 2;
                settextrotation(0);
                drawtext(boundary.get_center(), featureInformation[i].featureName, boundary);
            }
        }
    }
}

void draw_point_of_interest(std::vector<POIInfo> poiInformation, char* path)
{

    Surface logo = load_png_from_file(path);

    for (unsigned i = 0; i < poiInformation.size(); i = i + 25)
    {

        /*--------------------------------------------------------------------*/
        /* Only draw POI when its location is within the viewing window       */
        /*--------------------------------------------------------------------*/

        if (if_in_visible_world(get_visible_world(), poiInformation[i].XY_position) == true)
        {

            draw_surface(logo, poiInformation[i].XY_position.x, poiInformation[i].XY_position.y);

        }
    }

}

void print_point_of_interest_name(std::vector<POIInfo> poiInformation)
{

    setcolor(BLACK);

    for (unsigned i = 0; i < poiInformation.size(); ++i)
    {

        /*--------------------------------------------------------------------*/
        /* Only print POI when its location is within the viewing window      */
        /*--------------------------------------------------------------------*/

        if (if_in_visible_world(get_visible_world(), poiInformation[i].XY_position) == true)
        {

            if (poiInformation[i].POIname != "<noname>")
                settextrotation(0);
            drawtext(poiInformation[i].XY_position, poiInformation[i].POIname, INITIAL_MAP_COORDS);
        }
    }
}

void act_on_intersection(void (*drawscreen_ptr) (void))
{
    drawscreen_ptr();
    event_loop(act_on_button, NULL, NULL, drawscreen);

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*======================Search bar related functions==========================*/

// Initialize search bar

void act_on_key_press(char c, int keysym)
{
    vector<unsigned> indexFirst, indexSecond;
    string error_message;

    if ((c <= 126 && c >= 32 && (ifFind || ifPath || search_path_to_POI == 1)) || keysym == 65293 || c == 8)
    {
        if (keysym == 65293)
        {
            if (ifFind)
            {
                streetID1.clear();
                streetID2.clear();
                draw_second = false;
                parser();
                key1 = "";
                key2 = "";
                drawscreen();
                highlight_street();
            }
            else if (ifPath)
            {
                if (firstEnter == false)
                {
                    draw_second = false;
                    parser_path();
                    firstEnter = true;
                }
                else
                {
                    draw_second = false;
                    parser_path();
                    key1 = "";
                    key2 = "";
                    key3 = "";
                    key4 = "";
                    //drawscreen();
                    indexFirst = find_intersection_ids_from_street_names(name1, name2);
                    indexSecond = find_intersection_ids_from_street_names(name3, name4);
                    if (indexFirst.empty() && indexSecond.empty())
                    {
                        error_message = "Invalid input for both intersections.";
                        draw_error(error_message);
                    }
                    else if (indexFirst.empty())
                    {
                        error_message = "Invalid input for first intersection.";
                        draw_error(error_message);
                    }
                    else if (indexSecond.empty())
                    {
                        error_message = "Invalid input for second intersection.";
                        draw_error(error_message);
                    }
                    else
                        draw_path(indexFirst[0], indexSecond[0]);
                    firstEnter = false;

                }
            }
            else if (search_path_to_POI == 1)
            {
                if (firstEnter == false)
                {
                    draw_second = false;
                    parser_path();
                    firstEnter = true;
                }
                else
                {
                    draw_second = false;
                    parser_path();
                    key1 = "";
                    key2 = "";
                    key3 = "";
                    key4 = "";
                    //drawscreen();
                    indexFirst = find_intersection_ids_from_street_names(name1, name2);
                    if (indexFirst.empty() && name3 == "false")
                    {
                        error_message = "Invalid input for both intersection and POI name.";
                        draw_error(error_message);
                    }
                    else if (indexFirst.empty())
                    {
                        error_message = "Invalid input for the intersections.";
                        draw_error(error_message);
                    }
                    else if (name3 == "false")
                    {
                        error_message = "Invalid input for the POI.";
                        draw_error(error_message);
                    }
                    else
                        draw_path_to_poi(indexFirst[0], name3);
                    firstEnter = false;
                }
            }
        }
        else if (c == 8)
        {
            if (firstEnter == false)
            {
                if (key1.size() != 0)
                {
                    key1.pop_back();
                }
            }
            else if (firstEnter == true)
            {
                if (key3.size() != 0)
                {
                    key3.pop_back();
                }
            }
        }
        else
        {
            if (firstEnter == false)
                key1 += c;
            else if (firstEnter == true)
                key3 += c;
        }
        draw_search_box();
    }
}

void act_on_Click_Path_button(void (*drawscreen_ptr) (void))
{
    drawscreen_ptr();
    if (click_path_intersection == 0)
    {
        click_path_intersection = 1;
        change_button_text("Click Path", "Click Path<");
    }
    else if (click_path_intersection == 1)
    {
        click_path_intersection = 0;
        change_button_text("Click Path<", "Click Path");
        drawscreen();
        name1 = "";
        name2 = "";
        name3 = "";
        name4 = "";
    }
    event_loop(act_on_click, NULL, NULL, drawscreen);
}

void act_on_Search_Path_to_POI_button(void (*drawscreen_ptr) (void))
{

    if (search_path_to_POI == 0)
    {
        firstEnter = false;
        search_path_to_POI = 1;
        change_button_text("Search POI", "Search POI<");
    }
    else if (search_path_to_POI == 1)
    {
        search_path_to_POI = 0;
        change_button_text("Search POI<", "Search POI");
        name1 = "";
        name2 = "";
        name3 = "";
        name4 = "";
    }
    key1 = "";
    key2 = "";
    streetID1.clear();
    streetID2.clear();
    drawscreen_ptr();
    event_loop(NULL, NULL, act_on_key_press, drawscreen_for_button);
}

void act_on_Click_Path_to_POI_button(void (*drawscreen_ptr) (void))
{
    drawscreen_ptr();
    if (click_path_to_POI == 0)
    {
        click_path_to_POI = 1;
        change_button_text("Click POI", "Click POI<");
    }
    else if (click_path_to_POI == 1)
    {
        click_path_to_POI = 0;
        change_button_text("Click POI<", "Click POI");
        drawscreen();
        name1 = "";
        name2 = "";
        name3 = "";
        name4 = "";
    }
    event_loop(act_on_click_to_POI, NULL, NULL, drawscreen);
}

void act_on_click(float x, float y, t_event_buttonPressed buttonPressed)
{

    t_point xyPosition;
    xyPosition.x = x;
    xyPosition.y = y;
    LatLon latlonPosition;

    if (click_path_intersection == 1)
    {

        if (buttonPressed.button == 1)
        {
            if (firstIntersection == false)
            {
                latlonPosition = xy_to_LatLon(xyPosition);
                index1 = find_closest_intersection(latlonPosition);
                firstIntersection = true;

            }
            else if (firstIntersection == true)
            {
                latlonPosition = xy_to_LatLon(xyPosition);
                index2 = find_closest_intersection(latlonPosition);
                firstIntersection = false;

            }
        }
        if (firstIntersection == false && (index1 != 0 || index2 != 0))
        {
            //drawscreen();
            draw_path(index1, index2);
            //index1 = 0;
            //index2 = 0;
        }
    }
}

void act_on_click_to_POI(float x, float y, t_event_buttonPressed buttonPressed)
{
    t_point xyPosition;
    xyPosition.x = x;
    xyPosition.y = y;
    LatLon latlonPosition;
    string POIname;

    if (click_path_to_POI == 1)
    {

        if (buttonPressed.button == 1)
        {
            if (firstIntersection == false)
            {
                latlonPosition = xy_to_LatLon(xyPosition);
                click_intersection_index = find_closest_intersection(latlonPosition);
                firstIntersection = true;

            }
            else if (firstIntersection == true)
            {
                latlonPosition = xy_to_LatLon(xyPosition);
                click_POI_index = find_closest_point_of_interest(latlonPosition);
                firstIntersection = false;

            }
        }
        if (firstIntersection == false && (click_intersection_index != 0 || click_POI_index != 0))
        {
            //drawscreen();
            POIname = allPOI[click_POI_index].POIname;
            draw_path_to_poi(click_intersection_index, POIname);
            //click_intersection_index = 0;
            //click_POI_index = 0;
        }
    }
}

void draw_search_box()
{

    if (ifFind)
    {

        set_coordinate_system(GL_SCREEN);
        t_bound_box current_view = get_visible_screen();
        float xleft, xright, ytop, ybot;

        xleft = current_view.left() + 20;
        xright = current_view.left() + 800;
        ytop = current_view.top() + 10;
        ybot = current_view.top() + 80;

        settextrotation(0);
        setfontsize(20);
        setlinewidth(5);

        setcolor(WHITE);
        fillrect(xleft, ytop, xright, ybot);
        setcolor(BLACK);
        drawrect(xleft, ytop, xright, ybot);
        drawtext_in(t_bound_box(xleft, ytop, xright, ybot), key1);
        set_coordinate_system(GL_WORLD);

    }
    else if (ifPath || search_path_to_POI == 1)
    {
        //first box
        set_coordinate_system(GL_SCREEN);
        t_bound_box current_view = get_visible_screen();
        float xleft, xright, ytop, ybot;

        xleft = current_view.left() + 20;
        xright = current_view.left() + 800;
        ytop = current_view.top() + 10;
        ybot = current_view.top() + 80;

        settextrotation(0);
        setfontsize(20);
        setlinewidth(5);

        setcolor(WHITE);
        fillrect(xleft, ytop, xright, ybot);
        setcolor(BLACK);
        drawrect(xleft, ytop, xright, ybot);
        drawtext_in(t_bound_box(xleft, ytop, xright, ybot), key1);
        set_coordinate_system(GL_WORLD);

        //second box
        set_coordinate_system(GL_SCREEN);
        current_view = get_visible_screen();

        xleft = current_view.left() + 20;
        xright = current_view.left() + 800;
        ytop = current_view.top() + 90;
        ybot = current_view.top() + 160;

        settextrotation(0);
        setfontsize(20);
        setlinewidth(5);

        setcolor(WHITE);
        fillrect(xleft, ytop, xright, ybot);
        setcolor(BLACK);
        drawrect(xleft, ytop, xright, ybot);
        drawtext_in(t_bound_box(xleft, ytop, xright, ybot), key3);
        set_coordinate_system(GL_WORLD);

    }
    else
    {
        set_coordinate_system(GL_SCREEN);
        t_bound_box current_view = get_visible_screen();
        float xleft, xright, ytop, ybot;

        xleft = current_view.left() + 20;
        xright = current_view.left() + 800;
        ytop = current_view.top() + 10;
        ybot = current_view.top() + 80;

        setlinewidth(5);

        setcolor(255, 255, 255, 100);
        fillrect(xleft, ytop, xright, ybot);
        setcolor(0, 0, 0, 100);
        drawrect(xleft, ytop, xright, ybot);
        set_coordinate_system(GL_WORLD);
    }
    copy_off_screen_buffer_to_screen();
}

void act_on_help_button(void (*drawscreen_ptr) (void))
{

    set_coordinate_system(GL_SCREEN);
    t_bound_box current_view = get_visible_screen();
    float xleft, xright, ytop, ybot;

    xleft = current_view.left() + 50;
    xright = current_view.right() - 50;
    ytop = current_view.top() + 300;
    ybot = current_view.bottom() - 100;

    setlinewidth(5);

    setcolor(WHITE);
    fillrect(xleft, ytop, xright, ybot);
    setcolor(BLACK);
    drawrect(xleft, ytop, xright, ybot);

    settextrotation(0);
    setfontsize(14);

    ybot = ytop + 50;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "Instruction for each buttons");
    setfontsize(14);
    ytop = ytop + 30;
    ybot = ybot + 30;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "Feature Name:");
    setfontsize(11);
    ytop = ytop + 20;
    ybot = ybot + 20;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "Display feature names in the current viewing window");
    setfontsize(14);
    ytop = ytop + 25;
    ybot = ybot + 25;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "Intersection:");
    setfontsize(11);
    ytop = ytop + 20;
    ybot = ybot + 20;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "1st click, activation, click on any intersection to display the name");

    ytop = ytop + 15;
    ybot = ybot + 15;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "2nd click, deactivation ");
    setfontsize(14);
    ytop = ytop + 25;
    ybot = ybot + 25;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "POI:");
    setfontsize(11);
    ytop = ytop + 20;
    ybot = ybot + 20;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "1st click, activation, click on any POI to display the name");

    ytop = ytop + 15;
    ybot = ybot + 15;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "2nd click, deactivation ");
    setfontsize(14);
    ytop = ytop + 25;
    ybot = ybot + 25;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "Find:");
    setfontsize(11);
    ytop = ytop + 20;
    ybot = ybot + 20;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "1st click, activation, display any street or intersection by inputting the name followed by the “Enter” key");

    ytop = ytop + 15;
    ybot = ybot + 15;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "2nd click, deactivation ");
    setfontsize(14);
    ytop = ytop + 25;
    ybot = ybot + 25;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "Search Path:");
    setfontsize(11);
    ytop = ytop + 20;
    ybot = ybot + 20;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "1st click, activation, display path between any two intersections by inputting the names followed by the “Enter” key");

    ytop = ytop + 15;
    ybot = ybot + 15;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "2nd click, deactivation ");
    setfontsize(14);
    ytop = ytop + 25;
    ybot = ybot + 25;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "Click Path:");
    setfontsize(11);
    ytop = ytop + 20;
    ybot = ybot + 20;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "1st click, activation, display path between any two intersections by clicking the desired locations");

    ytop = ytop + 15;
    ybot = ybot + 15;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "2nd click, deactivation ");
    setfontsize(14);
    ytop = ytop + 25;
    ybot = ybot + 25;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "Search POI:");
    setfontsize(11);
    ytop = ytop + 20;
    ybot = ybot + 20;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "1st click, activation, display path between one intersection and one POI by inputting the names followed by the “Enter” key");

    ytop = ytop + 15;
    ybot = ybot + 15;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "2nd click, deactivation ");
    setfontsize(14);
    ytop = ytop + 25;
    ybot = ybot + 25;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "Click POI:");
    setfontsize(11);
    ytop = ytop + 20;
    ybot = ybot + 20;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "1st click, activation, display path between one intersection and one POI by clicking the desired locations ");

    ytop = ytop + 15;
    ybot = ybot + 15;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "2nd click, deactivation ");
    setfontsize(14);
    ytop = ytop + 25;
    ybot = ybot + 25;
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), "Exit help menu by clicking on any other button");

    set_coordinate_system(GL_WORLD);

    copy_off_screen_buffer_to_screen();

}

void act_on_Find_button(void (*drawscreen_ptr) (void))
{

    if (ifFind == false)
    {
        ifFind = true;
        change_button_text("Find", "Find <");
    }
    else if (ifFind == true)
    {
        ifFind = false;
        change_button_text("Find <", "Find");
    }
    key1 = "";
    key2 = "";
    streetID1.clear();
    streetID2.clear();
    drawscreen_ptr();
    event_loop(NULL, NULL, act_on_key_press, drawscreen_for_button);

}

void act_on_Path_button(void (*drawscreen_ptr) (void))
{

    if (ifPath == false)
    {
        firstEnter = false;
        ifPath = true;
        change_button_text("Search Path", "Search Path<");
    }
    else if (ifPath == true)
    {
        ifPath = false;
        change_button_text("Search Path<", "Search Path");
        drawscreen();
        name1 = "";
        name2 = "";
        name3 = "";
        name4 = "";
    }
    key1 = "";
    key2 = "";
    streetID1.clear();
    streetID2.clear();
    drawscreen_ptr();
    event_loop(NULL, NULL, act_on_key_press, drawscreen_for_button);

}

void drawscreen_for_button()
{
    drawscreen();
    draw_search_box();
}

void parser()
{

    std::stringstream input(key1);
    std::stringstream parsed_street_1, parsed_street_2;
    std::string temp;

    bool street_1_Fin = false;

    // Parse user input
    while (!input.eof())
    {
        input >> temp;
        if (temp[0] >= 97)
        {
            temp[0] -= ('a' - 'A');
        }

        if (temp == "-")
        {

            street_1_Fin = true;

        }
        if (!street_1_Fin)
        {

            parsed_street_1 << temp;
            parsed_street_1 << " ";

        }
        else
        {
            if (temp != "-")
            {

                draw_second = true;
                parsed_street_2 << temp;
                parsed_street_2 << " ";
                //std::cout << "two streets" << std::endl;

            }
        }
    }

    parsed_street_1 << "\0";
    key1 = parsed_street_1.str();
    key1.resize(key1.size() - 1);
    name1 = key1;
    string inst = fuzzy_search(name1);
    if (inst != "false")
        name1 = inst;
    //std::cout << key1 << std::endl;
    // i is the street segment index
    for (unsigned i = 0; i < ALLSTREETS.size(); ++i)
    {

        if (allStreetNames[ALLSTREETS[i].information.streetID] == name1)
        {
            streetID1.push_back(i);
        }
    }

    if (draw_second)
    {

        parsed_street_2 << "\0";
        key2 = parsed_street_2.str();
        key2.resize(key2.size() - 1);
        name2 = key2;
        inst = fuzzy_search(name2);
        if (inst != "false")
            name2 = inst;
        //std::cout << key2 << std::endl;
        for (unsigned i = 0; i < ALLSTREETS.size(); ++i)
        {

            if (allStreetNames[ALLSTREETS[i].information.streetID] == name2)
            {

                streetID2.push_back(i);

            }
        }
    }
    cout << name1 << "-" << name2 << endl;

}

void parser_path(void)
{


    std::stringstream parsed_street_1, parsed_street_2;
    std::string temp;
    string inst;
    string key1copy;

    bool street_1_Fin = false;

    // Parse user input
    if (ifPath)
    {
        if (firstEnter == false)
        {
            key1copy = key1;
            std::stringstream input(key1);
            while (!input.eof())
            {
                input >> temp;
                if (temp[0] >= 97)
                {
                    temp[0] -= ('a' - 'A');
                }

                if (temp == "-")
                {

                    street_1_Fin = true;

                }
                if (!street_1_Fin)
                {

                    parsed_street_1 << temp;
                    parsed_street_1 << " ";

                }
                else
                {
                    if (temp != "-")
                    {

                        draw_second = true;
                        parsed_street_2 << temp;
                        parsed_street_2 << " ";
                    }
                }
            }

            parsed_street_1 << "\0";
            key1 = parsed_street_1.str();
            key1.resize(key1.size() - 1);
            name1 = key1;

            if (draw_second)
            {
                parsed_street_2 << "\0";
                key2 = parsed_street_2.str();
                key2.resize(key2.size() - 1);
                name2 = key2;
            }

            inst = fuzzy_search(name1);
            if (inst != "false")
                name1 = inst;
            inst = fuzzy_search(name2);
            if (inst != "false")
                name2 = inst;

            key1 = key1copy;

        }
        else if (firstEnter == true)
        {
            std::stringstream input(key3);
            while (!input.eof())
            {
                input >> temp;
                if (temp[0] >= 97)
                {
                    temp[0] -= ('a' - 'A');
                }

                if (temp == "-")
                {

                    street_1_Fin = true;

                }
                if (!street_1_Fin)
                {

                    parsed_street_1 << temp;
                    parsed_street_1 << " ";

                }
                else
                {
                    if (temp != "-")
                    {

                        draw_second = true;
                        parsed_street_2 << temp;
                        parsed_street_2 << " ";
                    }
                }
            }

            parsed_street_1 << "\0";
            key3 = parsed_street_1.str();
            key3.resize(key3.size() - 1);
            name3 = key3;

            if (draw_second)
            {
                parsed_street_2 << "\0";
                key4 = parsed_street_2.str();
                key4.resize(key4.size() - 1);
                name4 = key4;
            }

            inst = fuzzy_search(name3);
            if (inst != "false")
                name3 = inst;
            inst = fuzzy_search(name4);
            if (inst != "false")
                name4 = inst;
        }
    }
    if (search_path_to_POI == 1)
    {
        if (firstEnter == false)
        {
            key1copy = key1;
            std::stringstream input(key1);
            while (!input.eof())
            {
                input >> temp;
                if (temp[0] >= 97)
                {
                    temp[0] -= ('a' - 'A');
                }

                if (temp == "-")
                {

                    street_1_Fin = true;

                }
                if (!street_1_Fin)
                {

                    parsed_street_1 << temp;
                    parsed_street_1 << " ";

                }
                else
                {
                    if (temp != "-")
                    {

                        draw_second = true;
                        parsed_street_2 << temp;
                        parsed_street_2 << " ";
                    }
                }
            }

            parsed_street_1 << "\0";
            key1 = parsed_street_1.str();
            key1.resize(key1.size() - 1);
            name1 = key1;

            if (draw_second)
            {
                parsed_street_2 << "\0";
                key2 = parsed_street_2.str();
                key2.resize(key2.size() - 1);
                name2 = key2;
            }

            inst = fuzzy_search(name1);
            if (inst != "false")
                name1 = inst;
            inst = fuzzy_search(name2);
            if (inst != "false")
                name2 = inst;

            key1 = key1copy;

        }
        else if (firstEnter == true)
        {

            name3 = key3;

            inst = fuzzy_search_POI(name3);
            name3 = inst;
        }
    }

}

void draw_path(unsigned intersection1, unsigned intersection2)
{

    std::vector<unsigned> path;
    path = find_path_between_intersections(intersection1, intersection2, 15);
    if (!path.empty())
    {
        std::vector<std::string> directions = direction_of_path(path);

        //        for (unsigned i = 0; i < directions.size(); i++)
        //        {
        //            std::cout << directions[i] << std::endl;
        //        }
    }
    string name1, name2;
    vector<string> streetNames;
    LatLon latlonLocation;
    t_point xyLocation;

    for (unsigned i = 0; i < path.size(); ++i)
    {

        setlinewidth(4);
        setlinestyle(SOLID, ROUND);
        setcolor(t_color(95, 158, 181));
        unsigned index = path[i];

        for (unsigned j = 0; j < (ALLSTREETS[index].curvePointPosition.size() - 1); ++j)
        {

            // Highlight street
            drawline(ALLSTREETS[index].curvePointPosition[j], ALLSTREETS[index].curvePointPosition[j + 1]);

        }
    }

    streetNames = find_intersection_street_names(intersection1);
    sort(streetNames.begin(), streetNames.end());
    streetNames.erase(std::unique(streetNames.begin(), streetNames.end()), streetNames.end());

    for (unsigned i = 0; i < streetNames.size(); ++i)
    {

        name1.append(streetNames[i]);
        if (i != streetNames.size() - 1)
            name1.append(" & ");

    }

    streetNames = find_intersection_street_names(intersection2);
    sort(streetNames.begin(), streetNames.end());
    streetNames.erase(std::unique(streetNames.begin(), streetNames.end()), streetNames.end());

    for (unsigned i = 0; i < streetNames.size(); ++i)
    {

        name2.append(streetNames[i]);
        if (i != streetNames.size() - 1)
            name2.append(" & ");

    }

    setfontsize(12);
    setcolor(t_color(114, 86, 129));
    settextrotation(0);

    latlonLocation = intersectionInformation[intersection1].intersection_position;
    xyLocation = LatLon_to_xy(latlonLocation);
    drawtext(xyLocation, name1, find_map_coordinates());

    latlonLocation = intersectionInformation[intersection2].intersection_position;
    xyLocation = LatLon_to_xy(latlonLocation);
    drawtext(xyLocation, name2, find_map_coordinates());

    Surface logo = load_png_from_file(PIN_LOGO_PATH);
    draw_surface(logo, xyLocation.x, xyLocation.y);

    draw_instruction(path);

}

void draw_path_to_poi(unsigned intersection_index, string POI_name)
{

    vector<unsigned> path;
    path = find_path_to_point_of_interest(intersection_index, POI_name, 15);
    string name1, name2;
    vector<string> streetNames;
    LatLon latlonLocation;
    t_point xyLocation;

    for (unsigned i = 0; i < path.size(); ++i)
    {

        setlinewidth(4);
        setlinestyle(SOLID, ROUND);
        setcolor(t_color(95, 158, 181));
        unsigned index = path[i];

        for (unsigned j = 0; j < (ALLSTREETS[index].curvePointPosition.size() - 1); ++j)
        {
            // Highlight street
            drawline(ALLSTREETS[index].curvePointPosition[j], ALLSTREETS[index].curvePointPosition[j + 1]);

        }
    }

    streetNames = find_intersection_street_names(intersection_index);
    sort(streetNames.begin(), streetNames.end());
    streetNames.erase(std::unique(streetNames.begin(), streetNames.end()), streetNames.end());

    for (unsigned i = 0; i < streetNames.size(); ++i)
    {

        name1.append(streetNames[i]);
        if (i != streetNames.size() - 1)
            name1.append(" & ");

    }

    setfontsize(12);
    setcolor(RED);
    settextrotation(0);

    latlonLocation = intersectionInformation[intersection_index].intersection_position;
    xyLocation = LatLon_to_xy(latlonLocation);
    drawtext(xyLocation, name1, find_map_coordinates());

    /*std::unordered_map<std::string, std::vector<unsigned>>::iterator iter = poiFromName.begin();
    unsigned poi_index;
    bool foundIndex = false;

    while (iter != poiFromName.end() && foundIndex == false)
    {

        if (POI_name == iter->first)
        {
            poi_index = iter->second[0];
            foundIndex = true;
        }
        iter++;
    }*/

    xyLocation = allPOI[DESIRED_POI_NUMBER].XY_position;

    drawtext(xyLocation, POI_name, find_map_coordinates());

    Surface logo = load_png_from_file(PIN_LOGO_PATH);
    draw_surface(logo, xyLocation.x, xyLocation.y);

    draw_instruction(path);

}

void draw_error(string error_message)
{

    set_coordinate_system(GL_SCREEN);
    t_bound_box current_view = get_visible_screen();
    float xleft, xright, ytop, ybot;

    xleft = current_view.left() + 20;
    xright = current_view.left() + 800;
    ytop = current_view.top() + 180;
    ybot = current_view.top() + 250;

    setcolor(t_color(255, 153, 51));
    fillrect(xleft, ytop, xright, ybot);

    settextrotation(0);
    setfontsize(20);
    setcolor(BLACK);
    drawtext_in(t_bound_box(xleft, ytop, xright, ybot), error_message);
    set_coordinate_system(GL_WORLD);

}

void draw_instruction(vector<unsigned> path)
{

    vector<string> instructions;

    instructions = direction_of_path(path);
    /*for(unsigned i=1;i<5;i++)
        instructions.push_back("abc")*/

    //instructions[0] = "Starting travel route: ";

    set_coordinate_system(GL_SCREEN);
    t_bound_box current_view = get_visible_screen();
    float xleft, xright, ytop, ybot, ytopfirst;

    xleft = current_view.left() + 20;
    xright = current_view.left() + 400;
    ytop = current_view.top() + 180 - 24; //width=24
    ybot = current_view.top() + 204 - 24;

    ytopfirst = ytop + 24;

    for (unsigned i = 0; i < instructions.size(); i++)
    {
        setcolor(WHITE);
        ytop += 24;
        ybot += 24;
        fillrect(xleft, ytop, xright, ybot);

        setcolor(BLACK);
        setlinewidth(1);
        drawrect(xleft, ytop, xright, ybot);

        settextrotation(0);
        setfontsize(14);
        setcolor(BLACK);
        drawtext_in(t_bound_box(xleft, ytop, xright, ybot), instructions[i]);
    }

    setcolor(BLACK);
    setlinewidth(4);
    drawrect(xleft, ytopfirst, xright, ybot);

    set_coordinate_system(GL_WORLD);

}

void highlight_street()
{

    for (unsigned i = 0; i < streetID1.size(); ++i)
    {

        setlinewidth(2);
        setlinestyle(SOLID, ROUND);
        setcolor(RED);
        unsigned index = streetID1[i];

        for (unsigned j = 0; j < (ALLSTREETS[index].curvePointPosition.size() - 1); ++j)
        {

            // Highlight street
            drawline(ALLSTREETS[index].curvePointPosition[j], ALLSTREETS[index].curvePointPosition[j + 1]);

        }
    }

    if (draw_second)
    {

        for (unsigned j = 0; j < streetID2.size(); ++j)
        {

            setlinewidth(2);
            setlinestyle(SOLID, ROUND);
            setcolor(BLUE);
            unsigned index = streetID2[j];

            for (unsigned k = 0; k < (ALLSTREETS[index].curvePointPosition.size() - 1); ++k)
            {

                // Highlight street
                drawline(ALLSTREETS[index].curvePointPosition[k], ALLSTREETS[index].curvePointPosition[k + 1]);
                Surface logo = load_png_from_file(DEFAULT_LOGO_PATH);

                std::vector<unsigned> intersectionIndex = find_intersection_ids_from_street_names(name1, name2);

                for (unsigned l = 0; l < intersectionIndex.size(); ++l)
                {

                    // Highlight intersection
                    t_point xyIntersectionLocation = LatLon_to_xy(intersectionInformation[intersectionIndex[l]].intersection_position);

                    draw_surface(logo, xyIntersectionLocation.x, xyIntersectionLocation.y);

                }
            }

        }
    }
}

unsigned levenshtein_distance(string s1, string s2)
{

    s1.erase(std::remove_if(s1.begin(), s1.end(), ::isspace), s1.end());
    std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);

    s2.erase(std::remove_if(s2.begin(), s2.end(), ::isspace), s2.end());
    std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);

    unsigned lenth1, lenth2;
    lenth1 = s1.size();
    lenth2 = s2.size();
    std::vector<std::vector<unsigned>> d((lenth1 + 1), std::vector<unsigned int>(lenth2 + 1));

    d[0][0] = 0;
    for (unsigned i = 1; i <= lenth1; i++)
        d[i][0] = i;
    for (unsigned i = 1; i <= lenth2; i++)
        d[0][i] = i;

    for (unsigned i = 1; i <= lenth1; i++)
        for (unsigned j = 1; j <= lenth2; j++)
            d[i][j] = std::min(std::min(d[i - 1][j] + 1, d[i][j - 1] + 1), d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1));

    return d[lenth1][lenth2];
}

string fuzzy_search(string inputName)
{
    //return most similar name when levenshtein distance less than 2, else return string "false"

    std::unordered_map<std::string, std::vector<unsigned>>::iterator iter = streetIdsFromName.begin();
    string streetName;
    unsigned Ldistance = 10;
    unsigned LdistanceOld = 10;
    string foundName;

    while (iter != streetIdsFromName.end())
    {

        streetName = iter->first;

        Ldistance = levenshtein_distance(inputName, streetName);

        if (Ldistance < LdistanceOld)
        {
            foundName = streetName;
            LdistanceOld = Ldistance;
        }

        iter++;
    }

    if (LdistanceOld <= 2)
        return foundName;

    foundName = "false";
    return foundName;
}

string fuzzy_search_POI(string inputName)
{
    std::unordered_map<std::string, std::vector<unsigned>>::iterator iter = poiFromName.begin();
    string POIName;
    unsigned Ldistance = 10;
    unsigned LdistanceOld = 10;
    string foundName;

    while (iter != poiFromName.end())
    {

        POIName = iter->first;

        Ldistance = levenshtein_distance(inputName, POIName);

        if (Ldistance < LdistanceOld)
        {
            foundName = POIName;
            LdistanceOld = Ldistance;
        }

        iter++;
    }

    if (LdistanceOld <= 4)
        return foundName;

    foundName = "false";
    return foundName;
}


/*======================Search bar related functions==========================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


void act_print_intersection_name(void (*drawscreen_ptr) (void))
{

    drawscreen_ptr();
    if (click_on_intersection == 0)
    {
        click_on_intersection = 1;
        change_button_text("Intersection", "Intersection <");
    }
    else if (click_on_intersection == 1)
    {
        click_on_intersection = 0;
        change_button_text("Intersection <", "Intersection");
    }

}

void act_print_POI_name(void (*drawscreen_ptr) (void))
{

    drawscreen_ptr();
    if (click_on_POI == 0)
    {
        click_on_POI = 1;
        change_button_text("POI", "POI <");
    }
    else if (click_on_POI == 1)
    {
        click_on_POI = 0;
        change_button_text("POI <", "POI");
    }

}

// Mouse click activities based on which button is clicked

void act_on_button(float x, float y, t_event_buttonPressed buttonPressed)
{

    t_point xyPosition;
    xyPosition.x = x;
    xyPosition.y = y;

    if (click_on_intersection == 1)
    {

        if (buttonPressed.button == 1)
        {
            LatLon latlonPosition = xy_to_LatLon(xyPosition);
            unsigned intersectionIndex = find_closest_intersection(latlonPosition);

            std::string names;
            std::vector<std::string> streetNames;
            streetNames = find_intersection_street_names(intersectionIndex);

            std::sort(streetNames.begin(), streetNames.end());
            streetNames.erase(std::unique(streetNames.begin(), streetNames.end()), streetNames.end());

            for (unsigned i = 0; i < streetNames.size(); ++i)
            {

                names.append(streetNames[i]);
                if (i != streetNames.size() - 1)
                    names.append(" & ");

            }

            LatLon latlonLocation = intersectionInformation[intersectionIndex].intersection_position;
            t_point xyLocation = LatLon_to_xy(latlonLocation);

            setfontsize(12);
            setcolor(RED);

            settextrotation(0);
            drawtext(xyLocation, names, find_map_coordinates());
            Surface logo = load_png_from_file(PIN_LOGO_PATH);
            draw_surface(logo, xyLocation.x, xyLocation.y);

        }
    }

    if (click_on_POI == 1)
    {

        if (buttonPressed.button == 1)
        {
            LatLon latlonPosition = xy_to_LatLon(xyPosition);
            unsigned poiIndex = find_closest_point_of_interest(latlonPosition);

            std::string POInames;
            POInames = allPOI[poiIndex].POIname;

            t_point xyLocation = allPOI[poiIndex].XY_position;

            setfontsize(12);
            setcolor(BLUE);
            settextrotation(0);
            drawtext(xyLocation, POInames, find_map_coordinates());

        }

    }
}

void act_print_feature_name(void (*drawscreen_ptr) (void))
{

    if (ZOOM_LEVEL == 1)
    {

        print_feature_name(featureArea0);
        print_feature_name(featureArea1);

    }
    else if (ZOOM_LEVEL == 2)
    {

        print_feature_name(featureArea0);
        print_feature_name(featureArea1);
        print_feature_name(featureArea2);

    }
    else if (ZOOM_LEVEL == 3)
    {

        print_feature_name(featureArea0);
        print_feature_name(featureArea1);
        print_feature_name(featureArea2);
        print_feature_name(featureArea3);

    }
    else if (ZOOM_LEVEL == 4)
    {
        print_feature_name(featureBuilding);
        print_feature_name(featureArea0);
        print_feature_name(featureArea1);
        print_feature_name(featureArea2);
        print_feature_name(featureArea3);
        print_feature_name(featureArea4);

    }
    else if (ZOOM_LEVEL == 5)
    {

        print_feature_name(featureArea0);
        print_feature_name(featureArea1);
        print_feature_name(featureArea2);
        print_feature_name(featureArea3);
        print_feature_name(featureArea5);

    }
}

////////////////////////////////////////////////////////////////////////////////
// * Draw streets by drawing a line between every curve point of a segment    //
////////////////////////////////////////////////////////////////////////////////

void draw_streets(std::vector<segmentInfo> streets, t_color color, float width)
{

    setlinewidth(width);
    setlinestyle(SOLID, ROUND);
    setcolor(color);

    for (unsigned i = 0; i < streets.size(); ++i)
    {

        for (unsigned j = 0; j < (streets[i].curvePointPosition.size() - 1); ++j)
        {

            drawline(streets[i].curvePointPosition[j], streets[i].curvePointPosition[j + 1]);

        }
    }
}

void draw_scale()
{

    t_bound_box temp = get_visible_world();

    float distance = (find_distance_between_two_points(xy_to_LatLon(temp.bottom_left()), xy_to_LatLon(t_point(temp.right(), temp.bottom()))) / 10.0);

    set_coordinate_system(GL_SCREEN);

    t_bound_box current_view = get_visible_screen();

    float xleft, xright, ytop, ybot;

    xleft = current_view.right() - 250;
    xright = current_view.right() - 250 + current_view.right() / 10;
    ytop = current_view.bottom() - 100;
    ybot = current_view.bottom() - 170;

    setcolor(t_color(105, 105, 105));
    setlinestyle(SOLID, BUTT);
    setlinewidth(8);

    drawline(t_point(xleft, ytop), t_point(xright, ytop));
    settextrotation(0);

    setfontsize(12);

    drawtext_in(t_bound_box(xleft, ybot + 35, xright, ytop), to_string((int) distance) + " m");
    set_coordinate_system(GL_WORLD);
}

////////////////////////////////////////////////////////////////////////////////
// * Get different zooming level by comparing the width of the current        //
// * viewing window and the initial viewing window                            //
////////////////////////////////////////////////////////////////////////////////

float get_zoom_level()
{

    t_bound_box current_map_coords = get_visible_world();

    float zoom_factor;

    if (INITIAL_MAP_COORDS.get_width() >= INITIAL_MAP_COORDS.get_height())
    {

        zoom_factor = INITIAL_MAP_COORDS.get_width() / current_map_coords.get_width();

    }
    else
    {

        zoom_factor = INITIAL_MAP_COORDS.get_height() / current_map_coords.get_height();

    }

    return (zoom_factor);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// Draw functions based on the zooming levels

void draw_zoom_level_0(void)
{

    draw_feature(featureLake, waterColor);
    draw_feature(featureIsland, parkColor);
    draw_streets(highway, highwayColor, 2);

}

void draw_zoom_level_1(void)
{

    draw_feature(featureLake, waterColor);
    draw_feature(featureIsland, parkColor);
    draw_feature(featurePark, parkColor);
    draw_feature(featureRiver, waterColor);
    draw_feature(featureGreenspace, parkColor);
    draw_feature(featureGolfcourse, parkColor);

    draw_streets(cityStreets, streetColor, 1);
    draw_streets(highway, highwayColor, 3);

}

void draw_zoom_level_2(void)
{

    draw_feature(featureLake, waterColor);
    draw_feature(featureIsland, parkColor);
    draw_feature(featurePark, parkColor);
    draw_feature(featureBeach, beachColor);
    draw_feature(featureRiver, waterColor);
    draw_feature(featureGreenspace, parkColor);
    draw_feature(featureGolfcourse, parkColor);

    draw_streets(cityStreets, streetColor, 3);
    draw_streets(highway, highwayColor, 8);

}

void draw_zoom_level_3(void)
{

    draw_streets(highway, highwayColor, 8);

    draw_feature(featureLake, waterColor);
    draw_feature(featureIsland, parkColor);
    draw_feature(featurePark, parkColor);
    draw_feature(featureBeach, beachColor);
    draw_feature(featureRiver, waterColor);
    draw_feature(featureShoreline, shorelineColor);
    draw_feature(featureGreenspace, parkColor);
    draw_feature(featureGolfcourse, parkColor);

    draw_streets(cityStreets, streetColor, 6);
    draw_streets(localStreets, streetColor, 4);

    draw_point_of_interest(busStationPOI, BUS_STATION_LOGO_PATH);
    draw_point_of_interest(fuelPOI, FUEL_LOGO_PATH);

    setfontsize(7);
    print_street_name(highway);

}

void draw_zoom_level_4(void)
{

    draw_streets(highway, highwayColor, 8);
    draw_feature(featureLake, waterColor);
    draw_feature(featureIsland, parkColor);
    draw_feature(featurePark, parkColor);
    draw_feature(featureBeach, beachColor);
    draw_feature(featureRiver, waterColor);
    draw_feature(featureShoreline, shorelineColor);
    draw_feature(featureGreenspace, parkColor);
    draw_feature(featureGolfcourse, parkColor);

    draw_streets(cityStreets, streetColor, 10);
    draw_streets(localStreets, streetColor, 5);

    draw_feature(featureStream, waterColor);
    draw_feature(featureBuilding, buildingColor);

    draw_point_of_interest(restaurantPOI, RESTAURANT_LOGO_PATH);
    draw_point_of_interest(fastFoodPOI, FAST_FOOD_LOGO_PATH);
    draw_point_of_interest(collegePOI, EDUCATION_LOGO_PATH);
    draw_point_of_interest(cafePOI, CAFE_LOGO_PATH);
    draw_point_of_interest(busStationPOI, BUS_STATION_LOGO_PATH);
    draw_point_of_interest(fuelPOI, FUEL_LOGO_PATH);

    setfontsize(9);
    print_street_name(highway);
    setfontsize(9);
    print_street_name(cityStreets);

}

void draw_zoom_level_5(void)
{

    draw_streets(highway, highwayColor, 3);
    draw_feature(featureLake, waterColor);
    draw_feature(featureIsland, parkColor);
    draw_feature(featurePark, parkColor);
    draw_feature(featureBeach, beachColor);
    draw_feature(featureRiver, waterColor);
    draw_feature(featureShoreline, shorelineColor);
    draw_feature(featureGreenspace, parkColor);
    draw_feature(featureGolfcourse, parkColor);
    draw_streets(cityStreets, streetColor, 13);
    draw_streets(localStreets, streetColor, 9);
    draw_feature(featureStream, waterColor);
    draw_feature(featureBuilding, buildingColor);

    draw_point_of_interest(restaurantPOI, RESTAURANT_LOGO_PATH);
    draw_point_of_interest(fastFoodPOI, FAST_FOOD_LOGO_PATH);
    draw_point_of_interest(collegePOI, EDUCATION_LOGO_PATH);
    draw_point_of_interest(cafePOI, CAFE_LOGO_PATH);
    draw_point_of_interest(busStationPOI, BUS_STATION_LOGO_PATH);
    draw_point_of_interest(fuelPOI, FUEL_LOGO_PATH);

    setfontsize(9);
    print_street_name(highway);
    print_street_name(cityStreets);
    print_street_name(localStreets);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void drawscreen(void)
{

    clearscreen(); /* Should precede drawing for all drawscreens */
    set_drawing_buffer(ON_SCREEN);

    //load_search_cell();
    load_data();

    if (get_zoom_level() <= ZOOM_LEVEL_1)
    {

        draw_zoom_level_1();
        ZOOM_LEVEL = 1;

    }
    else if (get_zoom_level() <= ZOOM_LEVEL_2)
    {

        draw_zoom_level_2();
        ZOOM_LEVEL = 2;


    }
    else if (get_zoom_level() <= ZOOM_LEVEL_3)
    {

        draw_zoom_level_3();
        ZOOM_LEVEL = 3;


    }
    else if (get_zoom_level() <= ZOOM_LEVEL_4)
    {


        draw_zoom_level_4();
        ZOOM_LEVEL = 4;


    }
    else if (get_zoom_level() <= ZOOM_LEVEL_5)
    {


        draw_zoom_level_5();
        ZOOM_LEVEL = 5;


    }
    else
    {

        draw_zoom_level_0();

    }
    draw_scale();

    vector<unsigned> indexFirst, indexSecond;

    indexFirst = find_intersection_ids_from_street_names(name1, name2);
    indexSecond = find_intersection_ids_from_street_names(name3, name4);

    if (ifPath)
    {
        if (indexFirst.empty() || indexSecond.empty())
        {
            //do nothing
        }
        else
            draw_path(indexFirst[0], indexSecond[0]);
    }


    vector<unsigned> secondIndex;
    secondIndex = find_intersection_ids_from_street_names(name1, name2);
    if (search_path_to_POI == 1)
    {
        if (secondIndex.empty() || name3 == "false")
        {
            //do nothing
        }
        else
            draw_path_to_poi(secondIndex[0], name3);
    }

    if (click_path_intersection == 1)
    {
        draw_path(index1, index2);
    }

    string POIname;
    if (click_path_to_POI == 1)
    {
        POIname = allPOI[click_POI_index].POIname;
        draw_path_to_poi(click_intersection_index, POIname);
    }




    //draw_path(25454, 6484);
    //std::cout << "=====" << get_visible_world().area() << "=====" << ZOOM_LEVEL << "=====" << std::endl;

    draw_search_box();
    copy_off_screen_buffer_to_screen();
}

void clear_map()
{

    allStreetNames.clear();
    localStreets.clear();
    cityStreets.clear();
    highway.clear();
    ALLSTREETS.clear();
    featureUnknown.clear();
    featurePark.clear();
    featureBeach.clear();
    featureLake.clear();
    featureRiver.clear();
    featureIsland.clear();
    featureShoreline.clear();
    featureBuilding.clear();
    featureGreenspace.clear();
    featureGolfcourse.clear();
    featureStream.clear();
    featureArea0.clear();
    featureArea1.clear();
    featureArea2.clear();
    featureArea3.clear();
    featureArea4.clear();
    featureArea5.clear();
    restaurantPOI.clear();
    fastFoodPOI.clear();
    fuelPOI.clear();
    cafePOI.clear();
    busStationPOI.clear();
    hospitalPOI.clear();
    collegePOI.clear();
    allPOI.clear();
    streetID1.clear();
    streetID2.clear();

}

float find_distance(t_point xy1, t_point xy2)
{

    return (sqrt(pow(xy2.y - xy1.y, 2) + pow(xy2.x - xy1.x, 2)));

}
