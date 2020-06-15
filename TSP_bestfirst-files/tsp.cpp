#include "tsp.h"
#include <fstream>
#include <string>

void read( char const* filename, MAP& map, int& TotalCity )
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
    }
	filestream.close();
}


std::vector<int> SolveTSP(char const* filename)
{
	std::vector<int> shortestDistance;
	
	
	// Do file I/O
	int totalCity;
	MAP map;
	
	read(filename, map, totalCity);
	
	std::vector<bool> visitedArr(totalCity);
	std::vector<int> shortestAnswer;
	std::vector<int> visitingOrder;
	shortestAnswer.reserve(totalCity);
	visitingOrder.reserve(totalCity);
	
	unsigned initMinDistance = -1u;
	
	SolveTSPRecursive(map, visitedArr, 0, 0, 1, totalCity, 
		visitingOrder, initMinDistance, shortestAnswer);
	
	return shortestDistance;
}

void SolveTSPRecursive(MAP const & map, std::vector<bool> & visitedCities, int currPos,
	unsigned currDistance, int count, unsigned totalCity, std::vector<int> & visitingOrder,
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
  
    // BACKTRACKING STEP 
    // Loop to traverse the adjacency list 
    // of currPos node and increasing the count 
	// and updating cost
    for (int i = 0; i < totalCity; i++) 
	{ 
        if (!visitedCities[i] && map[currPos][i])
		{ 
            // Mark as visited 
            visitedCities[i] = true; 
			visitingOrder.push_back(currPos);
			
			SolveTSPRecursive(map, visitedCities, i,
				currDistance + map[currPos][i], totalCity,  count + 1, 
				currMinDistance, shortestAnswer);
			visitingOrder.pop_back();
            // Mark ith node as unvisited 
            visitedCities[i] = false; 
        } 
    } 
}