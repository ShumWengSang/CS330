#include "closestpair.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>
#include <utility>

std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_aux (Point* data, int size);

////////////////////////////////////////////////////////////////////////////////
bool SortByX(Point const &a, Point const& b)
{
	return a.x < b.x;
}

bool SortByY(Point const &a, Point const& b)
{
	return a.y < b.y;
}

float Distance(Point& a, Point& b)
{
	return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Use brute force to find closest data point
float BruteForce(Point * data, int size)
{
	float min = std::numeric_limits<float>().max();  
    for (int i = 0; i < size; ++i) 
     {
        for (int j = i + 1; j < size; ++j)
        {
        	min = std::min(min, Distance(data[i], data[j]));
        }
    }
    return min;  
}

float closestPair ( std::vector< Point > const& points )
{
	int size = points.size();

	if (size==0) throw "zero size subset";
	if (size==1) return std::numeric_limits<float>::max();

	// Sort the points based on x
	std::vector<Point> xOrdering(points.begin(), points.end());
	std::sort(xOrdering.begin(),xOrdering.end(), SortByX);

	return closestPair_aux(xOrdering.data(), xOrdering.size());
}

////////////////////////////////////////////////////////////////////////////////
float StripContention(std::vector<Point>& arr, float bsf)
{
	// Sort the data by y first
	std::sort(arr.begin(), arr.end(), SortByY);

	float d = bsf;

	for(unsigned i = 0; i < arr.size(); ++i)
	{
		// Given that y is sorted, we can have a break condition 
		//if the points are on the same Y
		for(unsigned j = i + 1; j < arr.size() && (arr[i].y - arr[i].y) < d; ++j)
		{
			d = std::min(Distance(arr[i], arr[j]), d);
		}
	}
	return d;
}

float closestPair_aux (Point* data, int size) 
{
	if (size == 0) throw "zero size subset";
	if (size == 1) return std::numeric_limits<float>::max();

	// If there are 2 or 3 points, then use brute force  
    if (size <= 3)  
        return BruteForce(data, size);  
  
    // Find the middle point  
    int mid = size / 2;  
    Point midPoint = data[mid];  
  
    // Split into left and right non-empty subsets
    float dl = closestPair_aux(data, mid);  
    float dr = closestPair_aux(data + mid, size - mid); 

    // Get the minimum
    float d = std::min(dl,dr);

    // Build an array of points that are close enough to the middle
    // to contest minimum distance
    std::vector<Point> contentionPoints;
	contentionPoints.reserve(size);

    for (int i = 0; i < size; i++)  
        if (std::abs(data[i].x - midPoint.x) < d)  
            contentionPoints.push_back(data[i]);  

    // Find the minimum distance in the contention points
    float contention_d = StripContention(contentionPoints, d);

	return std::min(contention_d, d);
}

