#include "hull-bruteforce.h"
#include <algorithm>
#include <iostream>
#include <utility>

bool Point::operator==( Point const& arg2 ) const {
    return ( (x==arg2.x) && (y==arg2.y) );
}

std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//return value is (on left, on right)
std::pair<bool,bool> get_location(
		float const& p1x, //check which side of the line (p1x,p1y)-->(p2x,p2y) 
		float const& p1y, //point (qx,qy) is on
		float const& p2x,
		float const& p2y,
		float const& qx,
		float const& qy
		) 
{
	Point dir   = {p2x-p1x,p2y-p1y};
	Point norm  = {dir.y, -dir.x};
	Point point = {qx-p1x,qy-p1y};
	//scalar product is positive if on right side
	float scal_prod = norm.x*point.x + norm.y*point.y;
	return std::make_pair( (scal_prod<0), (scal_prod>0));
}

static bool arePointsOnOneSideOfLine(std::pair<Point, Point> const & line, std::vector<Point> const & points)
{
	bool isLeftSide = false;
	bool isRightSide = false;
	// If every point is on one side of iter_swap
	for(uint i = 0; i < points.size(); ++i)
	{
		std::pair<bool, bool> side = get_location(
			line.first.x, line.first.y, 
			line.second.x, line.second.y,
			points[i].x, points[i].y);
		bool onLeftSide = side.first;
		bool onRightSide = side.second;
		
		// Early exit if a point is detected to be not in the current side.
		if(isLeftSide && onRightSide)
		{
			return false;
		}
		if(isRightSide && onLeftSide)
		{
			return false;
		}
		if(!isLeftSide && onLeftSide)
		{
			isLeftSide = true;
		}
		if(!isRightSide && onRightSide)
		{
			isRightSide = true;
		}
	}
	// All points are on one side of line.
	return true;
}

static bool DoPointsExistsOnRightOfLine(std::pair<Point, Point> const & line, std::vector<Point> const & points)
{
	for(uint i = 0; i < points.size(); ++i)
	{
		std::pair<bool, bool> side = get_location(
			line.first.x, line.first.y, 
			line.second.x, line.second.y,
			points[i].x, points[i].y);
		if(side.second)
		{
			return true;
		}
	}
	return false;
}

//returns a set of indices of points that form convex hull
std::set<int> hullBruteForce ( std::vector< Point > const& points ) {
	int num_points = points.size();
	//std::cout << "number of points " << num_points << std::endl;
	if ( num_points < 3 ) throw "bad number of points";

	std::set<int> hull_indices;
	
	// For each pair
	for(uint i = 0; i < points.size(); ++i)
	{
		for(uint j = i + 1; j < points.size(); ++j)
		{
			std::pair<Point,Point> pairPoint = std::make_pair(points[i], points[j]);
			if(arePointsOnOneSideOfLine(pairPoint, points))
			{
				hull_indices.emplace(i);
				hull_indices.emplace(j);
			}
		}
	}
		
	return hull_indices;
}

int FindSmallestXPoint(std::vector<Point> const & points)
{
	uint smallestIndex = 999;
	float smallestX = 99999;
	for(uint i = 0; i < points.size(); ++i)
	{
		if(points[i].x < smallestX)
		{
			smallestIndex = i;
			smallestX = points[i].x;
		}
	}
	return smallestIndex;
}

std::vector<int> hullBruteForce2 ( std::vector< Point > const& points ) {
	uint num_points = points.size();
	if ( num_points < 3 ) throw "bad number of points";

	std::vector<int> hull_indices;

	// Find initial point for hull as smallest x*point
	uint initialPoint = FindSmallestXPoint(points);
	hull_indices.emplace_back(initialPoint);
	
	uint currentPoint = initialPoint;
	bool firstPoint = true;
	// Using the initial point and other points as a line
	// This should loop in a circle around all the points, creating the hull, and exiting
	// when we are back at the begining of the circle
	for(uint i = 0; i < points.size(); ++i)
	{
		if(i == currentPoint)
			continue;
		std::pair<Point,Point> pairPoint = std::make_pair(points[currentPoint], points[i]);
		// If there are no points on the right side.
		if(!DoPointsExistsOnRightOfLine(pairPoint, points))
		{
			// If we detect that we are back to the front, we exit the loop
			// Hack detection to determine if its the very first loop.
			if(i == initialPoint && !firstPoint)
			{
				break;
			}
			// Add this point to the 
			hull_indices.emplace_back(i);
			currentPoint = i;

			firstPoint = false;
			
			// Reset the loop
			i = -1;
		}
	}
	
	return hull_indices;
}
