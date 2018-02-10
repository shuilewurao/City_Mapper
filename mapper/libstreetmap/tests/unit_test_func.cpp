#include <algorithm>
#include <set>
#include <unittest++/UnitTest++.h>
#include "unit_test_util.h"
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include <random>

#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"
using ece297test::relative_error;

//struct MapFixture {
//
//    MapFixture() {
//        //Load the map
//        load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
//    }
//
//    ~MapFixture() {
//        //Clean-up
//        close_map();
//    }
//};

//SUITE(street_queries)
//{
//
//    TEST_FIXTURE(MapFixture, find_street_ids_from_name)
//    {
//        std::vector<unsigned> expected;
//        std::vector<unsigned> actual;
//
//
//        //checks for a street with 1 id
//        expected = {8};
//        actual = find_street_ids_from_name("Exit 375 Highway 404 / Don Valley Parkway");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {1019};
//        actual = find_street_ids_from_name("Boulton Avenue");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {1995};
//        actual = find_street_ids_from_name("Chudleigh Avenue");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//
//        //checks for a street with multiple ids
//        expected = {20806, 20903};
//        actual = find_street_ids_from_name("McDonald's drive-thru");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {4329, 6794, 21214};
//        actual = find_street_ids_from_name("South Service Road");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//
//
//        //tests for a non-existant street (corner))
//        expected = {};
//        actual = find_street_ids_from_name("Test Street");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_street_ids_from_name("Dollarama drive-thru");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        expected = {};
//        actual = find_street_ids_from_name("Xuan Street");
//        std::sort(actual.begin(), actual.end());
//        CHECK_EQUAL(expected, actual);
//
//        //checks all street names, automatically
//        for (unsigned i = 0; i < (unsigned) getNumberOfStreets(); i++)
//        {
//
//            std::string name = getStreetName(i);
//            actual = find_street_ids_from_name(name);
//            bool check = std::any_of(actual.begin(), actual.end(), [ = ](int id){return id == i;});
//            CHECK(check);
//        }
//
//
//    }
//
//
//
//}

using ece297test::relative_error;
using ece297test::path_is_legal;

struct MapFixture
{

    MapFixture()
    {
        rng = std::minstd_rand(4);
        rand_intersection = std::uniform_int_distribution<unsigned>(0, getNumberOfIntersections() - 1);
        rand_street = std::uniform_int_distribution<unsigned>(1, getNumberOfStreets() - 1);
        rand_segment = std::uniform_int_distribution<unsigned>(0, getNumberOfStreetSegments() - 1);
        rand_poi = std::uniform_int_distribution<unsigned>(0, getNumberOfPointsOfInterest() - 1);
        rand_lat = std::uniform_real_distribution<double>(43.48, 43.91998);
        rand_lon = std::uniform_real_distribution<double>(-79.78998, -79.00001);
        rand_turn_penalty = std::uniform_real_distribution<double>(0., 30.);
    }

    std::minstd_rand rng;
    std::uniform_int_distribution<unsigned> rand_intersection;
    std::uniform_int_distribution<unsigned> rand_street;
    std::uniform_int_distribution<unsigned> rand_segment;
    std::uniform_int_distribution<unsigned> rand_poi;
    std::uniform_real_distribution<double> rand_lat;
    std::uniform_real_distribution<double> rand_lon;
    std::uniform_real_distribution<double> rand_turn_penalty;
};

SUITE(inter_inter_simple_path_func_public)
{

    TEST(find_path_between_intersections_simple_legality)
    {
        std::vector<unsigned> path;

        path = find_path_between_intersections(5100, 5101, 0);
        CHECK(path_is_legal(5100, 5101, path));

        path = find_path_between_intersections(13222, 23746, 0);
        CHECK(path_is_legal(13222, 23746, path));

        path = find_path_between_intersections(25937, 24777, 0);
        CHECK(path_is_legal(25937, 24777, path));

        path = find_path_between_intersections(31204, 31203, 0);
        CHECK(path_is_legal(31204, 31203, path));

        path = find_path_between_intersections(36012, 66455, 0);
        CHECK(path_is_legal(36012, 66455, path));

        path = find_path_between_intersections(50444, 49782, 0);
        CHECK(path_is_legal(50444, 49782, path));

        path = find_path_between_intersections(50814, 68574, 0);
        CHECK(path_is_legal(50814, 68574, path));

        path = find_path_between_intersections(53232, 53250, 0);
        CHECK(path_is_legal(53232, 53250, path));

        path = find_path_between_intersections(57720, 57747, 0);
        CHECK(path_is_legal(57720, 57747, path));

        path = find_path_between_intersections(27925, 62494, 0);
        CHECK(path_is_legal(27925, 62494, path));

        path = find_path_between_intersections(63326, 93138, 0);
        CHECK(path_is_legal(63326, 93138, path));

        path = find_path_between_intersections(64448, 64449, 0);
        CHECK(path_is_legal(64448, 64449, path));

        path = find_path_between_intersections(65709, 65672, 0);
        CHECK(path_is_legal(65709, 65672, path));

        path = find_path_between_intersections(34406, 34409, 0);
        CHECK(path_is_legal(34406, 34409, path));

        path = find_path_between_intersections(71692, 11626, 0);
        CHECK(path_is_legal(71692, 11626, path));

        path = find_path_between_intersections(33188, 73114, 0);
        CHECK(path_is_legal(33188, 73114, path));

        path = find_path_between_intersections(26296, 74002, 0);
        CHECK(path_is_legal(26296, 74002, path));

        path = find_path_between_intersections(69186, 32340, 0);
        CHECK(path_is_legal(69186, 32340, path));

        path = find_path_between_intersections(82598, 82590, 0);
        CHECK(path_is_legal(82598, 82590, path));

        path = find_path_between_intersections(83507, 83509, 0);
        CHECK(path_is_legal(83507, 83509, path));

        path = find_path_between_intersections(86217, 89186, 0);
        CHECK(path_is_legal(86217, 89186, path));

        path = find_path_between_intersections(86592, 86591, 0);
        CHECK(path_is_legal(86592, 86591, path));

        path = find_path_between_intersections(19278, 87490, 0);
        CHECK(path_is_legal(19278, 87490, path));

        path = find_path_between_intersections(9, 13445, 0);
        CHECK(path_is_legal(9, 13445, path));

        path = find_path_between_intersections(89779, 89785, 0);
        CHECK(path_is_legal(89779, 89785, path));

        path = find_path_between_intersections(90809, 90810, 0);
        CHECK(path_is_legal(90809, 90810, path));

        path = find_path_between_intersections(63858, 63857, 0);
        CHECK(path_is_legal(63858, 63857, path));

        path = find_path_between_intersections(94321, 98486, 0);
        CHECK(path_is_legal(94321, 98486, path));

        path = find_path_between_intersections(106035, 89025, 0);
        CHECK(path_is_legal(106035, 89025, path));

        path = find_path_between_intersections(106719, 106723, 0);
        CHECK(path_is_legal(106719, 106723, path));

    } //find_path_between_intersections_simple_legality

    TEST(find_path_between_intersections_simple_optimality)
    {
        std::vector<unsigned> path;
        path = find_path_between_intersections(5100, 5101, 0);
        CHECK(path_is_legal(5100, 5101, path));
        CHECK(compute_path_travel_time(path, 0) <= 14.77362826908273);

        path = find_path_between_intersections(13222, 23746, 0);
        CHECK(path_is_legal(13222, 23746, path));
        CHECK(compute_path_travel_time(path, 0) <= 24.43654735586321);

        path = find_path_between_intersections(25937, 24777, 0);
        CHECK(path_is_legal(25937, 24777, path));
        CHECK(compute_path_travel_time(path, 0) <= 8.578245951487707);

        path = find_path_between_intersections(31204, 31203, 0);
        CHECK(path_is_legal(31204, 31203, path));
        CHECK(compute_path_travel_time(path, 0) <= 6.70541882901776);

        path = find_path_between_intersections(36012, 66455, 0);
        CHECK(path_is_legal(36012, 66455, path));
        CHECK(compute_path_travel_time(path, 0) <= 26.56293100758358);

        path = find_path_between_intersections(50444, 49782, 0);
        CHECK(path_is_legal(50444, 49782, path));
        CHECK(compute_path_travel_time(path, 0) <= 28.47874124602301);

        path = find_path_between_intersections(50814, 68574, 0);
        CHECK(path_is_legal(50814, 68574, path));
        CHECK(compute_path_travel_time(path, 0) <= 5.494479989845622);

        path = find_path_between_intersections(53232, 53250, 0);
        CHECK(path_is_legal(53232, 53250, path));
        CHECK(compute_path_travel_time(path, 0) <= 33.3938773235985);

        path = find_path_between_intersections(57720, 57747, 0);
        CHECK(path_is_legal(57720, 57747, path));
        CHECK(compute_path_travel_time(path, 0) <= 0.7449090749409762);

        path = find_path_between_intersections(27925, 62494, 0);
        CHECK(path_is_legal(27925, 62494, path));
        CHECK(compute_path_travel_time(path, 0) <= 8.734875472091367);

        path = find_path_between_intersections(63326, 93138, 0);
        CHECK(path_is_legal(63326, 93138, path));
        CHECK(compute_path_travel_time(path, 0) <= 2.1344193676628);

        path = find_path_between_intersections(64448, 64449, 0);
        CHECK(path_is_legal(64448, 64449, path));
        CHECK(compute_path_travel_time(path, 0) <= 1.01370935569944);

        path = find_path_between_intersections(65709, 65672, 0);
        CHECK(path_is_legal(65709, 65672, path));
        CHECK(compute_path_travel_time(path, 0) <= 48.33413232175537);

        path = find_path_between_intersections(34406, 34409, 0);
        CHECK(path_is_legal(34406, 34409, path));
        CHECK(compute_path_travel_time(path, 0) <= 5.427970985368932);

        path = find_path_between_intersections(71692, 11626, 0);
        CHECK(path_is_legal(71692, 11626, path));
        CHECK(compute_path_travel_time(path, 0) <= 23.63816286891876);

        path = find_path_between_intersections(33188, 73114, 0);
        CHECK(path_is_legal(33188, 73114, path));
        CHECK(compute_path_travel_time(path, 0) <= 1.342731300080379);

        path = find_path_between_intersections(26296, 74002, 0);
        CHECK(path_is_legal(26296, 74002, path));
        CHECK(compute_path_travel_time(path, 0) <= 2.150030561864719);

        path = find_path_between_intersections(69186, 32340, 0);
        CHECK(path_is_legal(69186, 32340, path));
        CHECK(compute_path_travel_time(path, 0) <= 9.407552962388356);

        path = find_path_between_intersections(82598, 82590, 0);
        CHECK(path_is_legal(82598, 82590, path));
        CHECK(compute_path_travel_time(path, 0) <= 2.445212553940233);

        path = find_path_between_intersections(83507, 83509, 0);
        CHECK(path_is_legal(83507, 83509, path));
        CHECK(compute_path_travel_time(path, 0) <= 11.32856798748073);

        path = find_path_between_intersections(86217, 89186, 0);
        CHECK(path_is_legal(86217, 89186, path));
        CHECK(compute_path_travel_time(path, 0) <= 4.245568575084612);

        path = find_path_between_intersections(86592, 86591, 0);
        CHECK(path_is_legal(86592, 86591, path));
        CHECK(compute_path_travel_time(path, 0) <= 2.18468372533349);

        path = find_path_between_intersections(19278, 87490, 0);
        CHECK(path_is_legal(19278, 87490, path));
        CHECK(compute_path_travel_time(path, 0) <= 10.19777845760323);

        path = find_path_between_intersections(9, 13445, 0);
        CHECK(path_is_legal(9, 13445, path));
        CHECK(compute_path_travel_time(path, 0) <= 15.13158661041701);

        path = find_path_between_intersections(89779, 89785, 0);
        CHECK(path_is_legal(89779, 89785, path));
        CHECK(compute_path_travel_time(path, 0) <= 4.926351251571414);

        path = find_path_between_intersections(90809, 90810, 0);
        CHECK(path_is_legal(90809, 90810, path));
        CHECK(compute_path_travel_time(path, 0) <= 1.212837627983459);

        path = find_path_between_intersections(63858, 63857, 0);
        CHECK(path_is_legal(63858, 63857, path));
        CHECK(compute_path_travel_time(path, 0) <= 2.443939312876431);

        path = find_path_between_intersections(94321, 98486, 0);
        CHECK(path_is_legal(94321, 98486, path));
        CHECK(compute_path_travel_time(path, 0) <= 10.60383801298999);

        path = find_path_between_intersections(106035, 89025, 0);
        CHECK(path_is_legal(106035, 89025, path));
        CHECK(compute_path_travel_time(path, 0) <= 2.301580363211564);

        path = find_path_between_intersections(106719, 106723, 0);
        CHECK(path_is_legal(106719, 106723, path));
        CHECK(compute_path_travel_time(path, 0) <= 6.154833140351119);

    } //find_path_between_intersections_simple_optimality

} //inter_inter_simple_path_func_public


//TEST_FIXTURE(MapFixture, find_street_ids_from_name){
//    for(unsigned i = 0; i< getNumberOfStreets(); i++){
//        std::vector<unsigned> expected;
//        std::vector<unsigned> actual;
//        std::string street = getStreetName(i); 
//        
//        for(unsigned j = 0; j< getNumberOfStreets(); j++){
//            if(getStreetName(j) == street)
//                expected.push_back(j);
//        }
//        
//        actual = find_street_ids_from_name(street);
//        
//        CHECK_EQUAL(expected,actual); 
//    }
//}