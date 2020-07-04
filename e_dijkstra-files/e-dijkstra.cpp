#include "e-dijkstra.h"
#include <map>

using CityIndex = int;
using Distance = int;
using CityMap = std::map<CityIndex, std::map<CityIndex, Distance>>;

struct FileInput 
{
    CityMap theMap;
    int numLocations;
    int maximumRecharge;
    int numEdges;
}

void ReadFile(char const * fileName, FileInput& fileData)
{
    std::ifstream in(fileName, std::ifstream::in);
    if (!in.is_open()) {
        std::cout << "problem reading " << fileName << std::endl;
        return;
    }
    in >> fileData.numLocations;
    in >> fileData.maximumRecharge;
    in >> fileData.numEdges;
    
    for(int i = 0; i < fileData.numEdges; ++i)
    {
        int Source, Destination, Cost;
        
        if (!in.good())
        {
            std::cout << "problem reading " << filename << std::endl;
            return;
        }
        in >> Source;
        in >> Destination;
        in >> Cost;
        
        fileData.theMap[Source][Destination] = Cost;
    }
}

bool e_dijkstra( char const * fileName, int vehicleRange)
{
    // First read in the fileName
    FileInput FileData;
    ReadFile(fileName, FileData);
    
    // Then we do dijktra on it.
    
    // If we find a false early return
    
    // Else return true.
}