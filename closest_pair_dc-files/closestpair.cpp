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
float closestPair_aux ( .................... );

////////////////////////////////////////////////////////////////////////////////
float closestPair ( std::vector< Point > const& points ) {
	int size = points.size();

	if (size==0) throw "zero size subset";
	if (size==1) return std::numeric_limits<float>::max();

	return closestPair_aux( ....... );
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_aux ( ..................... ) {
	int size = indices.size();

	if (size==0) throw "zero size subset";
	if (size==1) return std::numeric_limits<float>::max();

	.................

	return min_dist;
}

