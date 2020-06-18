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

void SolveTSPRecursive(MAP const & map, std::vector<bool> & visitedCities, int currPos,
	unsigned currDistance, unsigned count, unsigned totalCity, std::vector<int> & visitingOrder,
	unsigned& currMinDistance, std::vector<int> & shortestAnswer);

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

	SolveTSPRecursive(map, visitedArr, 0, 
        0, 1, totalCity, visitingOrder, 
        initMinDistance, shortestAnswer);
	
    shortestAnswer.push_back(0);

	return shortestAnswer;
}

unsigned CalcHeruisticRec(MAP const& map, unsigned currPos, unsigned count, unsigned totalCity, std::vector<bool> const & visited)
{
	if(count == totalCity)
	{
		return map[currPos][0];
	}

	int min = std::numeric_limits<int>::max();
	int index = -1;
	for(unsigned i = 0; i < totalCity; ++i)
	{
		if(!visited[i])
		{
			if (min > map[currPos][i]) 
			{
            	min = map[currPos][i];
            	index = i;
			}
		}
	}
	return min + CalcHeruisticRec(map, index, count+1, totalCity, visited);
}

unsigned CalcHeuristic(MAP const & map, unsigned currPos, unsigned totalCity, std::vector<bool> const& visited, std::vector<int> const& visitingOrder)
{
    return CalcHeruisticRec(map, currPos, visitingOrder.size(), totalCity, visited);
}

void SolveTSPRecursive(MAP const & map, std::vector<bool> & visitedCities, int currPos,
	unsigned currDistance, unsigned count, unsigned totalCity, std::vector<int> & visitingOrder,
	unsigned& currMinDistance, std::vector<int> & shortestAnswer)
{
	// If last node is reached and it has a link 
    // to the starting node i.e the source then 
    // keep the minimum value out of the total cost 
    // of traversal and "ans" 
    // Finally return to check for more possible values 
    if (count == totalCity && map[currPos][0])
	{ 
		if(currMinDistance > currDistance + map[currPos][0])
		{
			currMinDistance = currDistance + map[currPos][0];
			shortestAnswer = visitingOrder; 
		}
		
        return; 
    } 
  
    // Generate heuristic for each level
    using Index = int;
    using Heuristic = unsigned;
    //using HeuristicPair = std::pair<unsigned, Index>;

    std::multimap <Heuristic, Index> heuristics;

    // std::vector<HeuristicPair> heuristics;
    // heuristics.reserve(totalCity);

    for (unsigned i = 1; i < totalCity; ++i)
    {
        if (!visitedCities[i] && map[currPos][i])
        {
            // Simple check
            if ((currDistance + map[currPos][i]) < currMinDistance)
            {
            	visitedCities[i] = true;
                Heuristic lowerBoundHeuristic = CalcHeuristic(map, i, totalCity, visitedCities, visitingOrder);
                if (lowerBoundHeuristic + currDistance < currMinDistance)
                {
                    heuristics.insert(std::make_pair(lowerBoundHeuristic + currDistance, i));
                    //heuristics.push_back(std::make_pair(lowerBoundHeuristic + currDistance, i));
                }
                visitedCities[i] = false;
            }
        }
    }

    //std::sort(heuristics.begin(), heuristics.end(), [](HeuristicPair const &a, HeuristicPair const &b)
    //    {
    //        return a.first < b.first;
    //    });

    for (auto iter = heuristics.begin(); iter != heuristics.end(); ++iter)
    {
        Index index = iter->second;
        // Mark as visited 
        visitedCities[index] = true;
        visitingOrder.push_back(index);

        SolveTSPRecursive(map, visitedCities, index,
            currDistance + map[currPos][index], count + 1, totalCity, visitingOrder,
            currMinDistance, shortestAnswer);
        visitingOrder.pop_back();
        visitedCities[index] = false;
    }

    //for (unsigned i = 0; i < heuristics.size(); ++i)
    //{
    //    Index index = heuristics[i].second;
    //    // Mark as visited 
    //    visitedCities[index] = true;
    //    visitingOrder.push_back(index);

    //    SolveTSPRecursive(map, visitedCities, index,
    //        currDistance + map[currPos][index], count + 1, totalCity, visitingOrder,
    //        currMinDistance, shortestAnswer);
    //    visitingOrder.pop_back();
    //    visitedCities[index] = false;
    //}
}