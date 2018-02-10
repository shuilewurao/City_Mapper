#include <iostream>
#include <string>
#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include "m2_helper.h"
#include "m3.h"
#include "m4.h"
#include "packageInfo.h"
#include "m4_helper.h"

extern std::vector<double> TravelTime;

int main(int argc, char** argv)
{

    std::string map_path;

    //    std::cout << "      _______________________________________________________________________________________________________ " << endl;
    //    std::cout << "     |                                                                                                       |" << endl;
    //    std::cout << "     |                                               Welcome                                                 |" << endl;
    //    std::cout << "     | _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ |" << endl;
    //    std::cout << "     |                                                                                                       |" << endl;
    //    std::cout << "     |                                please select the map you wish to see                                  |" << endl;
    //    std::cout << "     | _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ |" << endl;
    //    std::cout << "     |                                                                                                       |" << endl;
    //    std::cout << "     |     NORTH AMERICA                              EUROPE                         ASIA                    |" << endl;
    //    std::cout << "     |                                                                                                       |" << endl;
    //    std::cout << "     |1. Toronto, Canada                         5. London, England             7. Moscow, Russia            |" << endl;
    //    std::cout << "     |2. Hamilton, Canada                        6. Iceland                     8. Tehran, Iran              |" << endl;
    //    std::cout << "     |3. Golden Horseshoe, Canada                                               9. Beijing, China            |" << endl;
    //    std::cout << "     |4. New York, USA                                                         10. Hong Kong, China          |" << endl;
    //    std::cout << "     |                                                                         11. Tokyo, Japan              |" << endl;
    //    std::cout << "     |                                                                         12. New Delhi, India          |" << endl;
    //    std::cout << "     |                                                                         13. Singapore                 |" << endl;
    //    std::cout << "     |                                                                                                       |" << endl;
    //    std::cout << "     |                                                AFRICA                                                 |" << endl;
    //    std::cout << "     |                                                                                                       |" << endl;
    //    std::cout << "     |                                           14. Cairo, Egypt                                            |" << endl;
    //    std::cout << "     |          SOUTH AMERICA                    15. Cape Town, South Africa                                 |" << endl;
    //    std::cout << "     |                                           16. Saint Helena                                            |" << endl;
    //    std::cout << "     |     17. Rio De Janeiro, Brazil                                                     AUSTRALIA          |" << endl;
    //    std::cout << "     |                                                                                                       |" << endl;
    //    std::cout << "     |                                                                               18. Sydney. Australia   |" << endl;
    //    std::cout << "     |                                                                                                       |" << endl;
    //    std::cout << "     |                                                                                                       |" << endl;
    //    std::cout << "     |                                                                                                       |" << endl;
    //    std::cout << "     |_______________________________________________________________________________________________________|" << endl;
    //    std::cout << endl;
    //
    //    int userChoice;
    //    std::cout << "Enter your city number: " << std::endl;
    //    std::cin >> userChoice;
    //
    //    while (!cin.fail() && userChoice != 0) {
    //
    //        switch (userChoice) {
    //            case 1:
    //                map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
    //                break;
    //            case 2:
    //                map_path = "/cad2/ece297s/public/maps/hamilton_canada.streets.bin";
    //                break;
    //            case 3:
    //                map_path = "/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin";
    //                break;
    //            case 4:
    //                map_path = "/cad2/ece297s/public/maps/new-york_usa.streets.bin";
    //                break;
    //            case 5:
    //                map_path = "/cad2/ece297s/public/maps/london_england.streets.bin";
    //                break;
    //            case 6:
    //                map_path = "/cad2/ece297s/public/maps/iceland.streets.bin";
    //                break;
    //            case 7:
    //                map_path = "/cad2/ece297s/public/maps/moscow_russia.streets.bin";
    //                break;
    //            case 8:
    //                map_path = "/cad2/ece297s/public/maps/tehran_iran.streets.bin";
    //                break;
    //            case 9:
    //                map_path = "/cad2/ece297s/public/maps/beijing_china.streets.bin";
    //                break;
    //            case 10:
    //                map_path = "/cad2/ece297s/public/maps/hong-kong_china.streets.bin";
    //                break;
    //            case 11:
    //                map_path = "/cad2/ece297s/public/maps/tokyo_japan.streets.bin";
    //                break;
    //            case 12:
    //                map_path = "/cad2/ece297s/public/maps/new-delhi_india.streets.bin";
    //                break;
    //            case 13:
    //                map_path = "/cad2/ece297s/public/maps/singapore.streets.bin";
    //                break;
    //            case 14:
    //                map_path = "/cad2/ece297s/public/maps/cairo_egypt.streets.bin";
    //                break;
    //            case 15:
    //                map_path = "/cad2/ece297s/public/maps/cape-town_south-africa.streets.bin";
    //                break;
    //            case 16:
    //                map_path = "/cad2/ece297s/public/maps/saint-helena.streets.bin";
    //                break;
    //            case 17:
    //                map_path = "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin";
    //                break;
    //            case 18:
    //                map_path = "/cad2/ece297s/public/maps/sydney_australia.streets.bin";
    //                break;
    //            default:
    //                map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
    //                break;
    //        }
    //
    //        if (map_path != "0") {
    //            clear_map();
    //            //Load the map and related data structures
    //            bool load_success = load_map(map_path);
    //            if (!load_success) {
    //                std::cerr << "Failed to load map '" << map_path << "'\n";
    //                return 2;
    //            }
    //
    //            std::cout << "Successfully loaded map '" << map_path << "'\n";
    //
    //            std::cout << "finished" << std::endl;
    //            draw_map();
    //
    //            //Clean-up the map related data structures
    //            std::cout << "Closing map\n";
    //            close_map();
    //
    //            std::cout << "      _______________________________________________________________________________________________________ " << endl;
    //            std::cout << "     |                                                                                                       |" << endl;
    //            std::cout << "     |                                               Welcome                                                 |" << endl;
    //            std::cout << "     | _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ |" << endl;
    //            std::cout << "     |                                                                                                       |" << endl;
    //            std::cout << "     |                                please select the map you wish to see                                  |" << endl;
    //            std::cout << "     | _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ |" << endl;
    //            std::cout << "     |                                                                                                       |" << endl;
    //            std::cout << "     |     NORTH AMERICA                              EUROPE                         ASIA                    |" << endl;
    //            std::cout << "     |                                                                                                       |" << endl;
    //            std::cout << "     |1. Toronto, Canada                         5. London, England             7. Moscow, Russia            |" << endl;
    //            std::cout << "     |2. Hamilton, Canada                        6. Iceland                     8. Tehran, Iran              |" << endl;
    //            std::cout << "     |3. Golden Horseshoe, Canada                                               9. Beijing, China            |" << endl;
    //            std::cout << "     |4. New York, USA                                                         10. Hong Kong, China          |" << endl;
    //            std::cout << "     |                                                                         11. Tokyo, Japan              |" << endl;
    //            std::cout << "     |                                                                         12. New Delhi, India          |" << endl;
    //            std::cout << "     |                                                                         13. Singapore                 |" << endl;
    //            std::cout << "     |                                                                                                       |" << endl;
    //            std::cout << "     |                                                AFRICA                                                 |" << endl;
    //            std::cout << "     |                                                                                                       |" << endl;
    //            std::cout << "     |                                           14. Cairo, Egypt                                            |" << endl;
    //            std::cout << "     |          SOUTH AMERICA                    15. Cape Town, South Africa                                 |" << endl;
    //            std::cout << "     |                                           16. Saint Helena                                            |" << endl;
    //            std::cout << "     |     17. Rio De Janeiro, Brazil                                                                        |" << endl;
    //            std::cout << "     |                                                                                                       |" << endl;
    //            std::cout << "     |                                                                                    AUSTRALIA          |" << endl;
    //            std::cout << "     |                                                                                                       |" << endl;
    //            std::cout << "     |                                                                               18. Sydney. Australia   |" << endl;
    //            std::cout << "     |                                                                                                       |" << endl;
    //            std::cout << "     |_______________________________________________________________________________________________________|" << endl;
    //            std::cout << endl;
    //
    //            std::cout << "Enter your city: " << std::endl;
    //            std::cin >> userChoice;
    //        }
    //    }
    //
    //    if (argc == 1) {
    //        //Use a default map
    //        map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
    //    } else if (argc == 2) {
    //        //Get the map from the command line
    //        map_path = argv[1];
    //    } else {
    //        //Invalid arguments
    //        std::cerr << "Usage: " << argv[0] << " [map_file_path]\n";
    //        std::cerr << "  If no map_file_path is provided a default map is loaded.\n";
    //        return 1;
    //    }
    map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
    bool load_success = load_map(map_path);
    //    if (!load_success) {
    //        std::cerr << "Failed to load map '" << map_path << "'\n";
    //        return 2;
    //    }

    std::cout << "Successfully loaded map '" << map_path << "'\n";
    //You can now do something with the map
    //

    //    std::vector<DeliveryInfo> deliveries = {DeliveryInfo(73593, 104453), DeliveryInfo(1981, 47060), DeliveryInfo(58204, 31403), DeliveryInfo(102129, 63975), DeliveryInfo(41153, 63608)};
    //    std::vector<unsigned> depots = {2521, 5153, 41138};
    //    float turn_penalty = 15;
    //    std::vector<unsigned> result_path = traveling_courier(deliveries, depots, turn_penalty);
    //    std::cout << "----- " << "Path Size: " << result_path.size() << " -----" << std::endl;

    // hard Toronto
    //    std::vector<DeliveryInfo> deliveries = {DeliveryInfo(79796, 42839), DeliveryInfo(6620, 18908), DeliveryInfo(12917, 69324), DeliveryInfo(4009, 76027), DeliveryInfo(78640, 49449), DeliveryInfo(21674, 90763), DeliveryInfo(50896, 23880), DeliveryInfo(74090, 53653), DeliveryInfo(2111, 61148), DeliveryInfo(43971, 1223), DeliveryInfo(50063, 91913), DeliveryInfo(82083, 84759), DeliveryInfo(105386, 101774), DeliveryInfo(107769, 57682), DeliveryInfo(82981, 96244), DeliveryInfo(8738, 34980), DeliveryInfo(1342, 95587), DeliveryInfo(42483, 103377), DeliveryInfo(1161, 103664), DeliveryInfo(21428, 77598), DeliveryInfo(71316, 23235), DeliveryInfo(95619, 32911), DeliveryInfo(99061, 18848), DeliveryInfo(93949, 20039), DeliveryInfo(67982, 12095), DeliveryInfo(11945, 105365), DeliveryInfo(94347, 20391), DeliveryInfo(92583, 13875), DeliveryInfo(2811, 42442), DeliveryInfo(20951, 31516), DeliveryInfo(104712, 15493), DeliveryInfo(25003, 55427), DeliveryInfo(23213, 50781), DeliveryInfo(33297, 94451), DeliveryInfo(91792, 620), DeliveryInfo(94877, 17273), DeliveryInfo(42021, 30653), DeliveryInfo(45453, 80507), DeliveryInfo(19701, 9255), DeliveryInfo(100880, 11642), DeliveryInfo(43478, 97117), DeliveryInfo(37661, 18514), DeliveryInfo(27367, 105658), DeliveryInfo(4306, 54259), DeliveryInfo(107255, 358), DeliveryInfo(90985, 90550), DeliveryInfo(105483, 54918), DeliveryInfo(52218, 24878), DeliveryInfo(64146, 14569), DeliveryInfo(96695, 6359), DeliveryInfo(77408, 7132), DeliveryInfo(475, 94272), DeliveryInfo(94111, 10286), DeliveryInfo(102213, 28393), DeliveryInfo(31031, 5428), DeliveryInfo(61407, 87201), DeliveryInfo(52502, 25739), DeliveryInfo(64678, 11041), DeliveryInfo(7230, 43223), DeliveryInfo(42470, 32197), DeliveryInfo(17541, 79209), DeliveryInfo(38249, 15541), DeliveryInfo(44546, 79051), DeliveryInfo(11121, 102845), DeliveryInfo(55855, 31136), DeliveryInfo(38533, 45203), DeliveryInfo(82603, 34501), DeliveryInfo(84243, 24909), DeliveryInfo(69588, 744), DeliveryInfo(10546, 6312), DeliveryInfo(16898, 19546), DeliveryInfo(43784, 60601), DeliveryInfo(64789, 68213), DeliveryInfo(28003, 21779), DeliveryInfo(17561, 48204), DeliveryInfo(69514, 34766), DeliveryInfo(17040, 74080), DeliveryInfo(91356, 102978), DeliveryInfo(6292, 87024), DeliveryInfo(65826, 27845), DeliveryInfo(80009, 86251), DeliveryInfo(105642, 44253), DeliveryInfo(16066, 8023), DeliveryInfo(20106, 87653), DeliveryInfo(66771, 16683), DeliveryInfo(90038, 88410), DeliveryInfo(40917, 83054), DeliveryInfo(60050, 94321), DeliveryInfo(55413, 11814), DeliveryInfo(56359, 1693), DeliveryInfo(23931, 47427), DeliveryInfo(86665, 93996), DeliveryInfo(79695, 16400), DeliveryInfo(58624, 21665), DeliveryInfo(82028, 43899), DeliveryInfo(30893, 19071), DeliveryInfo(31114, 26049), DeliveryInfo(31066, 56617), DeliveryInfo(103338, 92746), DeliveryInfo(7629, 13181)};
    //    std::vector<unsigned>depots = {8552, 38211, 92758, 39860, 100277, 17123, 6130, 40760, 5413, 47813};
    //    float turn_penalty = 15;
    //    std::vector<unsigned> result_path = traveling_courier(deliveries, depots, turn_penalty);
    //    std::cout << "----- " << "Path quality: " << compute_path_travel_time(result_path, 15) << " -----" << std::endl;


    // simple legal 14
    //        std::vector<DeliveryInfo> deliveries = {DeliveryInfo(64489, 10192), DeliveryInfo(66348, 47055)};
    //        std::vector<unsigned> depots = {75020, 59249};
    //        float turn_penalty = 15;
    //        std::vector<unsigned> result_path = traveling_courier(deliveries, depots, turn_penalty);
    //        std::cout << "----- " << "Path Size: " << result_path.size() << " -----" << std::endl;

    //==========================================================================
    //    std::vector<unsigned> path = find_path_between_intersections(47055, 66348, 15);
    //    std::cout << compute_path_travel_time(path, 15) << std::endl;
    //    std::vector<closest_info> path2 = multi_destination_Dijkstra(47055,{10192, 66348}, 15);
    //    std::cout << path2[1].cost << std::endl;
    //==========================================================================
    //    std::vector<DeliveryInfo> deliveries = {DeliveryInfo(79796, 42839), DeliveryInfo(6620, 18908), DeliveryInfo(12917, 69324), DeliveryInfo(4009, 76027), DeliveryInfo(78640, 49449), DeliveryInfo(21674, 90763), DeliveryInfo(50896, 23880), DeliveryInfo(74090, 53653), DeliveryInfo(2111, 61148), DeliveryInfo(43971, 1223), DeliveryInfo(50063, 91913), DeliveryInfo(82083, 84759), DeliveryInfo(105386, 101774), DeliveryInfo(107769, 57682), DeliveryInfo(82981, 96244), DeliveryInfo(8738, 34980), DeliveryInfo(1342, 95587), DeliveryInfo(42483, 103377), DeliveryInfo(1161, 103664), DeliveryInfo(21428, 77598), DeliveryInfo(71316, 23235), DeliveryInfo(95619, 32911), DeliveryInfo(99061, 18848), DeliveryInfo(93949, 20039), DeliveryInfo(67982, 12095), DeliveryInfo(11945, 105365), DeliveryInfo(94347, 20391), DeliveryInfo(92583, 13875), DeliveryInfo(2811, 42442), DeliveryInfo(20951, 31516), DeliveryInfo(104712, 15493), DeliveryInfo(25003, 55427), DeliveryInfo(23213, 50781), DeliveryInfo(33297, 94451), DeliveryInfo(91792, 620), DeliveryInfo(94877, 17273), DeliveryInfo(42021, 30653), DeliveryInfo(45453, 80507), DeliveryInfo(19701, 9255), DeliveryInfo(100880, 11642), DeliveryInfo(43478, 97117), DeliveryInfo(37661, 18514), DeliveryInfo(27367, 105658), DeliveryInfo(4306, 54259), DeliveryInfo(107255, 358), DeliveryInfo(90985, 90550), DeliveryInfo(105483, 54918), DeliveryInfo(52218, 24878), DeliveryInfo(64146, 14569), DeliveryInfo(96695, 6359), DeliveryInfo(77408, 7132), DeliveryInfo(475, 94272), DeliveryInfo(94111, 10286), DeliveryInfo(102213, 28393), DeliveryInfo(31031, 5428), DeliveryInfo(61407, 87201), DeliveryInfo(52502, 25739), DeliveryInfo(64678, 11041), DeliveryInfo(7230, 43223), DeliveryInfo(42470, 32197), DeliveryInfo(17541, 79209), DeliveryInfo(38249, 15541), DeliveryInfo(44546, 79051), DeliveryInfo(11121, 102845), DeliveryInfo(55855, 31136), DeliveryInfo(38533, 45203), DeliveryInfo(82603, 34501), DeliveryInfo(84243, 24909), DeliveryInfo(69588, 744), DeliveryInfo(10546, 6312), DeliveryInfo(16898, 19546), DeliveryInfo(43784, 60601), DeliveryInfo(64789, 68213), DeliveryInfo(28003, 21779), DeliveryInfo(17561, 48204), DeliveryInfo(69514, 34766), DeliveryInfo(17040, 74080), DeliveryInfo(91356, 102978), DeliveryInfo(6292, 87024), DeliveryInfo(65826, 27845), DeliveryInfo(80009, 86251), DeliveryInfo(105642, 44253), DeliveryInfo(16066, 8023), DeliveryInfo(20106, 87653), DeliveryInfo(66771, 16683), DeliveryInfo(90038, 88410), DeliveryInfo(40917, 83054), DeliveryInfo(60050, 94321), DeliveryInfo(55413, 11814), DeliveryInfo(56359, 1693), DeliveryInfo(23931, 47427), DeliveryInfo(86665, 93996), DeliveryInfo(79695, 16400), DeliveryInfo(58624, 21665), DeliveryInfo(82028, 43899), DeliveryInfo(30893, 19071), DeliveryInfo(31114, 26049), DeliveryInfo(31066, 56617), DeliveryInfo(103338, 92746), DeliveryInfo(7629, 13181)};
    //    std::vector<unsigned> destinations;
    //    for (unsigned i = 0; i < deliveries.size(); ++i)
    //    {
    //        destinations.push_back(deliveries[i].dropOff);
    //        destinations.push_back(deliveries[i].pickUp);
    //    }
    //
    //    std::sort(destinations.begin(), destinations.end());
    //    destinations.resize(std::distance(destinations.begin(), std::unique(destinations.begin(), destinations.end())));
    //
    //    std::vector<closest_info> path2 = multi_destination_Dijkstra(44253, destinations, 15, 10);
    //
    //    std::cout << std::endl;
    //    std::cout << "found: " << path2[0].intersection_index << std::endl;
    //    std::cout << path2[0].cost << std::endl;
    //    std::cout << std::endl;
    //    std::cout << "found: " << path2[1].intersection_index << std::endl;
    //    std::cout << path2[1].cost << std::endl;
    //    std::cout << std::endl;
    draw_map();

    //Clean-up the map related data structures
    std::cout << "Closing map\n";
    //close_map();

    return 0;
}
