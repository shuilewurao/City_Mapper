#include <algorithm>
#include <set>
#include <unittest++/UnitTest++.h>
#include "unit_test_util.h"
#include "m1.h"
#include "StreetsDatabaseAPI.h"

using ece297test::relative_error;

struct MapFixture {
    MapFixture() {
        //Load the map
        load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");

        //Initialize random number generators
        rng = std::minstd_rand(3);
        rand_intersection = std::uniform_int_distribution<unsigned>(0, getNumberOfIntersections()-1);
        rand_street = std::uniform_int_distribution<unsigned>(1, getNumberOfStreets()-1);
        rand_segment = std::uniform_int_distribution<unsigned>(0, getNumberOfStreetSegments()-1);
        rand_poi = std::uniform_int_distribution<unsigned>(0, getNumberOfPointsOfInterest()-1);
        rand_lat = std::uniform_real_distribution<double>(43.48, 43.91998);
        rand_lon = std::uniform_real_distribution<double>(-79.78998, -79.00001);
    }

    ~MapFixture() {
        //Clean-up
        close_map();
    }

    std::minstd_rand rng;
    std::uniform_int_distribution<unsigned> rand_intersection;
    std::uniform_int_distribution<unsigned> rand_street;
    std::uniform_int_distribution<unsigned> rand_segment;
    std::uniform_int_distribution<unsigned> rand_poi;
    std::uniform_real_distribution<double> rand_lat;
    std::uniform_real_distribution<double> rand_lon;
};

SUITE(find_street_ids_from_name) {
        TEST_FIXTURE(MapFixture,find_street_ids_from_name){
        std::vector<unsigned> expected;
        std::vector<unsigned> actual;
        
        
        //checks for a street with 1 id
        expected={8};
        actual=find_street_ids_from_name("Exit 375 Highway 404 / Don Valley Parkway");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected={1019};
        actual=find_street_ids_from_name("Boulton Avenue");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected={1995};
        actual=find_street_ids_from_name("Chudleigh Avenue");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual);
        
        
        //checks for a street with multiple ids
        expected = {20806, 20903};
        actual = find_street_ids_from_name("McDonald's drive-thru");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);
        
        expected={4329, 6794, 21214};
        actual=find_street_ids_from_name("South Service Road");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual); 
        

        
        //tests for a non-existant street (corner case))
        expected={};
        actual=find_street_ids_from_name("Collage Street");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual); 
        
        expected={};
        actual=find_street_ids_from_name("Dollarama drive-thru");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected={};
        actual=find_street_ids_from_name("Xuan Street");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected={};
        actual=find_street_ids_from_name("Jack Road");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected={};
        actual=find_street_ids_from_name("Jack Street");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected={};
        actual=find_street_ids_from_name("Jack Circle");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected={};
        actual=find_street_ids_from_name("Jack Crescent");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected={};
        actual=find_street_ids_from_name("Jack Avenue");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected={98};
        actual=find_street_ids_from_name("Whitney Avenue");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected,actual);
        
        //Generate random inputs
        std::vector<std::string> street_names;
        for(size_t i = 0; i < 1000000; i++) {
            street_names.push_back(getStreetName(rand_street(rng)));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(250);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 1000000; i++) {
                result = find_street_ids_from_name(street_names[i]);
            }
        }


    } //intersection_street_segments_perf
}

