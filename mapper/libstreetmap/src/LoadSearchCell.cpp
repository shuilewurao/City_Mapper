/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "LoadSearchCell.h"

void load_search_cell(void) {
    for (unsigned i = 0; i < intersectionInformation.size(); ++i)
    {


        SearchCell tempCell;
        tempCell.id = i;
        tempCell.F = std::numeric_limits<float>::infinity();
        tempCell.G = 0;
        tempCell.H = 0;
        tempCell.parent_intersection = 0;
        tempCell.parent_street_segment = 0;
        tempCell.visited = false;

        std::vector<unsigned> currentSeg = intersectionStreetSeg[i];
        
        for (unsigned j = 0; j < currentSeg.size(); ++j)
        {

            cellInfo tempConnection;

            // the current street segment is one way
            if (streetSegmentInfo[currentSeg[j]].oneWay)
            {
                if (streetSegmentInfo[currentSeg[j]].from)
                {

                    tempConnection.intersection_id = streetSegmentInfo[currentSeg[j]].to;
                    tempConnection.streetsegment_id = currentSeg[j];
                    tempConnection.travelTime = TravelTime[currentSeg[j]];

                }
            }// the current street segment is not one way
            else
            {

                if (streetSegmentInfo[currentSeg[j]].from)
                {

                    tempConnection.intersection_id = streetSegmentInfo[currentSeg[j]].to;
                    tempConnection.streetsegment_id = currentSeg[j];
                    tempConnection.travelTime = TravelTime[currentSeg[j]];

                }
                else if (streetSegmentInfo[currentSeg[j]].to == tempCell.id)
                {

                    tempConnection.intersection_id = streetSegmentInfo[currentSeg[j]].from;
                    tempConnection.streetsegment_id = currentSeg[j];
                    tempConnection.travelTime = TravelTime[currentSeg[j]];

                }

            }

            tempCell.directed_connections.push_back(tempConnection);
        }

        std::sort(tempCell.directed_connections.begin(), tempCell.directed_connections.end(),
                [](const cellInfo &left, const cellInfo & right)
                {
                    return ( left.travelTime < right.travelTime); });

        intersectionCells.push_back(tempCell);

    }
}