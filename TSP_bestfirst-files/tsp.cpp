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
        map[i][i] = std::numeric_limits<int>::max();
    }
	in.close();
}

struct TSPData
{
    MAP const& map;
    unsigned currPos;
    unsigned count;
    const unsigned totalCity;
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

int lb;
int recur;

std::vector<int> SolveTSP(char const* filename)
{
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


	std::cout << "Heuristic Recursive " << lb << std::endl;
	std::cout << "Recur " << recur << std::endl;

	return shortestAnswer;
}

static int LowerBound(MAP const& map,
    int size,
    std::vector<int>& curSolutionSoFar,
    int const& costSoFar)
{

	lb++;

    int lowerBound = costSoFar;
    int distToHome = map[0][curSolutionSoFar.back()];
    for (int i = 1; i < size; ++i)
    {
        if (std::find(curSolutionSoFar.begin(), curSolutionSoFar.end(), i) == curSolutionSoFar.end())
        {
            if (distToHome > map[0][i])
                distToHome = map[0][i];
            int minInRow = std::numeric_limits<int>::max();
            for (int j = 1; j < size; ++j)
            {
                int temp = curSolutionSoFar.back();
                curSolutionSoFar.pop_back();
                if (std::find(curSolutionSoFar.begin(), curSolutionSoFar.end(), j) == curSolutionSoFar.end())
                {
                    if (minInRow > map[i][j])
                        minInRow = map[i][j];
                }
                curSolutionSoFar.push_back(temp);
            }
            if (minInRow != std::numeric_limits<int>::max())
            {
                lowerBound += minInRow;
            }
            else
            {
                lowerBound += map[curSolutionSoFar.back()][i];
            }
        }
    }
    return lowerBound + distToHome;
}


unsigned CalcHeuristic(TSPData& data, int cityIndex)
{
    unsigned LowerBoundHeuristic = 0;
    unsigned DistanceHome = -1u;
    // Last node
    if (data.visitingOrder.size() == data.totalCity - 1)
        return data.map[cityIndex][0];

    data.visitedCities[data.currPos] = false;
    for (unsigned i = 1; i < data.totalCity; ++i)       
    {
        if (data.visitedCities[i])
            continue;

        int min_in_row = std::numeric_limits<int>::max();

        for (unsigned j = 1; j < data.totalCity; ++j)
        {
        	if ((!data.visitedCities[j] && j != i))
            {
                if (min_in_row > data.map[i][j])
                    min_in_row = data.map[i][j];
            }
        }
        LowerBoundHeuristic += min_in_row;
        DistanceHome = std::min(static_cast<int>(DistanceHome), data.map[i][0]);
    }
    data.visitedCities[data.currPos] = true;
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

	recur++;

    if (data.count == data.totalCity)
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

	// All cities I have not visited
	for (unsigned i = 1; i < data.totalCity; ++i)
	{
		if(data.visitedCities[i])
			continue;
		
		// Calculate a heuristic for those cities.
		data.visitingOrder.push_back(i);

		Heuristic lowerBoundHeuristic = LowerBound(data.map, data.totalCity, data.visitingOrder, 0);
		//Heuristic lowerBoundHeuristic = CalcHeuristic(data, i);
		if (lowerBoundHeuristic + data.currDistance < data.bsf)
		{
			heuristics.insert(std::make_pair(lowerBoundHeuristic + data.currDistance, i));
		}
		data.visitingOrder.pop_back();
	}


    for (auto iter = heuristics.begin(); iter != heuristics.end(); ++iter)
    {
        if (iter->first >= data.bsf)
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