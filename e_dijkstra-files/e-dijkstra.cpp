#include "e-dijkstra.h"
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <limits>

using CityIndex = int;
using Distance = int;
using CityMap = std::unordered_map<CityIndex, std::unordered_map<CityIndex, Distance>>;
using VisitedMap = std::unordered_map<CityIndex, std::unordered_map<CityIndex, bool>>;

struct FileInput
{
    CityMap theMap;
    int numLocations;
    int maximumRecharge;
    int numEdges;
};

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
            std::cout << "problem reading " << fileName << std::endl;
            return;
        }
        in >> Source;
        in >> Destination;
        in >> Cost;
        
        fileData.theMap[Source][Destination] = Cost;
    }
}

// A utility function to find the vertex with minimum distance value, from 
// the set of vertices not yet included in shortest path tree 
int minDistance(std::vector<bool> const & visited,std::vector<int> const & dist)
{
    // Initialize min value 
    int min = std::numeric_limits<int>().max(), min_index;

    for (int v = 0; v < visited.size(); v++)
        if (visited[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}


bool e_djikstra_i(FileInput &data, int vehicleRange, CityIndex source, CityIndex destination)
{
    // Note: If a path was found, we can just return true.
    // Vehicle starts empty.

    std::vector<bool> visited(data.numLocations);
    std::vector<int> dist(data.numLocations);


    // Initialize all distances as INFINITE and stpSet[] as false 
    for (int i = 0; i < data.numLocations; i++)
        dist[i] = std::numeric_limits<int>().max(), visited[i] = false;
    
    // Mark the source node as visited.
    visited[source] = true;

    // Find shortest path for all vertices 
    for (int count = 0; count < data.numLocations - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not 
        // yet processed. u is always equal to src in the first iteration. 
        int u = minDistance(visited, dist);

        // Mark the picked vertex as processed 
        visited[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex. 
        for (int v = 0; v < data.numLocations; v++)
        {

            if (!visited[v] && data.theMap[u][v] && dist[u] != std::numeric_limits<int>().max()
                && dist[u] + data.theMap[u][v] < dist[v])
            {
                dist[v] = dist[u] + data.theMap[u][v];
            }
        }
    }
    return true;
}

bool e_dijkstra( char const * fileName, int vehicleRange)
{
    // First read in the fileName
    FileInput FileData;
    ReadFile(fileName, FileData);
    
    // Keep a lookup table of all the places A can go to for optimization
    VisitedMap visitedMap;

    // We want to find if every city can go to every city
    for (int i = 0; i < FileData.numLocations; ++i)
    {
        for (int j = i + 1; j < FileData.numLocations; ++j)
        {
            // Then we do dijktra on it. If there are any 
            // places we can't reach return false
            if (!e_djikstra_i(FileData, vehicleRange, i, j))
                return false;
        }
    }
    return true;
}