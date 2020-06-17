#include "tsp.h"
#include <fstream>
#include <iostream>
#include <limits>  // numeric_limits
#include <map>
#include <algorithm>

void readCopy( char const* filename, MAP& map, int& TotalCity )
{
    map.clear();
    std::ifstream in( filename, std::ifstream::in );
    if( !in.is_open() ) {
        std::cout << "problem reading " << filename << std::endl;
        return;
    }
    in >> TotalCity;
	map.reserve(TotalCity);
    for( int i = 0; i < TotalCity; ++i ) {
        std::vector<int> row;
		row.reserve(TotalCity);
        for( int j = 0; j < TotalCity; ++j ) {
            row.push_back( std::numeric_limits<int>::max() );
        }
        map.push_back( row );
    }
    for( int i = 0; i < TotalCity; ++i ) {
        for( int j = i + 1; j < TotalCity; ++j ) {
            if( !in.good() ) {
                std::cout << "problem reading " << filename << std::endl;
                return;
            }
            in >> map[i][j]; 
            map[j][i] = map[i][j];

        }
        map[i][i] = 0;
    }
	in.close();
}

struct TSPData
{
    MAP const& map;
    unsigned currPos;
    unsigned count;
    unsigned totalCity;
    std::vector<bool>& visitedCities;

    unsigned currDistance;
    std::vector<int>& visitingOrder;

    unsigned& bsf;
    std::vector<int>& bsfTour;

    TSPData(MAP const& mp, int currPos, unsigned count, unsigned totalCity, std::vector<bool> & v,
        unsigned currDistance, std::vector<int>& visitingOrder, unsigned& bsf, std::vector<int>& bsfTour) :
        map(mp), currPos(currPos), count(count), totalCity(totalCity), visitedCities(v), currDistance(currDistance),
        visitingOrder(visitingOrder), bsf(bsf), bsfTour(bsfTour)
    {

    }
};

void SolveTSPRecursive(TSPData& data);

std::vector<int> SolveTSP(char const* filename)
{
	// Do file I/O
	int totalCity;
	MAP map;
	
	readCopy(filename, map, totalCity);
	
	std::vector<bool> visitedArr(totalCity);
	std::vector<int> shortestAnswer;
	std::vector<int> visitingOrder;
	shortestAnswer.reserve(totalCity);
	visitingOrder.reserve(totalCity);
	
	unsigned initMinDistance = -1u;

    visitedArr[0] = true;
    visitingOrder.push_back(0);

    TSPData data(map, 0, 1, totalCity, visitedArr, 0, visitingOrder, initMinDistance, shortestAnswer);

    //TSPData data(map, visitedArr, 0,
    //    0, 1, totalCity, visitingOrder,
    //    initMinDistance, shortestAnswer);

    SolveTSPRecursive(data);
	
    shortestAnswer.push_back(0);

	return shortestAnswer;
}

unsigned CalcHeuristicRec(TSPData& data, int cityIndexSource, int count, int result)
{
    unsigned minimum = -1u;
    unsigned index = -1;

    if (data.totalCity == count)
        return data.map[cityIndexSource][0];

    for (int i = 1; i < data.totalCity; ++i)
    {
        if (data.visitedCities[i])
            continue;
        if (minimum > data.map[cityIndexSource][minimum])
        {
            minimum = data.map[cityIndexSource][minimum];
            index = i;
        }
    }
    // data.visitedCities[index] = true;
    unsigned heu = CalcHeuristicRec(data, index, count + 1, result + minimum);
    // data.visitedCities[index] = false;
    return heu;
}

unsigned CalcHeuristic(TSPData& data, int cityIndex)
{
    unsigned LowerBoundHeuristic = 0;
    unsigned DistanceHome = -1u;
    // Last node
    if (data.visitingOrder.size() == data.totalCity - 1)
        return 0;

    for (unsigned i = 1; i < data.totalCity; ++i)       
    {
        if (data.visitedCities[i])
            continue;

        int min_in_row = std::numeric_limits<int>::max();
        for (unsigned j = 1; j < data.totalCity; ++j)
        {
            if (j == cityIndex || (!data.visitedCities[j] && j != i))
            {
                if (min_in_row > data.map[i][j])
                    min_in_row = data.map[i][j];
            }
        }
        LowerBoundHeuristic += min_in_row;
        DistanceHome = std::min(static_cast<int>(DistanceHome), data.map[i][0]);
    }

    return LowerBoundHeuristic;
}

unsigned CalcHeuristic(TSPData& data)
{
    unsigned LowerBoundHeuristic = 0;
    unsigned DistanceHome = -1u;
    // Last node
    if (data.visitingOrder.size() == data.totalCity - 1)
		return 0;

	for (unsigned i = 1; i < data.totalCity; i++)
	{
		if (!data.visitedCities[i])
		{
			int min_in_row = std::numeric_limits<int>::max();
			for (unsigned j = 1; j < data.totalCity; ++j)
			{
				if (!data.visitedCities[j] && j != i)
				{
					if (min_in_row > data.map[i][j])
						min_in_row = data.map[i][j];
				}
			}
			LowerBoundHeuristic += min_in_row;
			DistanceHome = std::min(static_cast<int>(DistanceHome), data.map[i][0]);
		}
	}
	return LowerBoundHeuristic;
}

void SolveTSPRecursive(TSPData &data)
{
    // If last node is reached and it has a link 
    // to the starting node i.e the source then 
    // keep the minimum value out of the total cost 
    // of traversal and "ans" 
    // Finally return to check for more possible values 
    if (data.count == data.totalCity && data.map[data.currPos][0])
    {
        if (data.bsf > data.currDistance + data.map[data.currPos][0])
        {
            data.bsf = data.currDistance + data.map[data.currPos][0];
            data.bsfTour = data.visitingOrder;
        }

        return;
    }

    // Generate heuristic for each level
    using Index = int;
    using Heuristic = unsigned;
    //using HeuristicPair = std::pair<unsigned, Index>;

	std::multimap <Heuristic, Index> heuristics;

	for (unsigned i = 1; i < data.totalCity; ++i)
	{
		if (!data.visitedCities[i] && data.map[data.currPos][i])
		{
			// Simple check
			// if ((data.currDistance + data.map[data.currPos][i]) < data.bsf)
			{
				Heuristic lowerBoundHeuristic = CalcHeuristic(data);
				if (lowerBoundHeuristic + data.currDistance < data.bsf)
				{
					heuristics.insert(std::make_pair(lowerBoundHeuristic + data.currDistance, i));
				}
			}
		}
	}


    for (auto iter = heuristics.begin(); iter != heuristics.end(); ++iter)
    {
        if (iter->first > data.bsf)
            break;
        Index index = iter->second;

        // Mark as visited 
        data.visitedCities[index] = true;
        data.visitingOrder.push_back(index);

        TSPData newData(data.map, index, data.count + 1, data.totalCity, data.visitedCities, data.currDistance + data.map[data.currPos][index], data.visitingOrder,
            data.bsf, data.bsfTour);
        SolveTSPRecursive(newData);

        // Pop
        data.visitingOrder.pop_back();
        data.visitedCities[index] = false;
    }
}