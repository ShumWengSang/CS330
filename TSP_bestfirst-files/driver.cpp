#include "tsp.h"
#include <iostream>
#include <vector>
#include <limits>  // numeric_limits
#include <numeric> // accumulate
#include <fstream> // ifstream

void read( char const* filename, MAP& map, int& TotalCity )
{
    map.clear();
    std::ifstream in( filename, std::ifstream::in );
    if( !in.is_open() ) {
        std::cout << "problem reading " << filename << std::endl;
        return;
    }
    in >> TotalCity;
    for( int i = 0; i < TotalCity; ++i ) {
        std::vector<int> row;
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
}

bool check_legal( std::vector<int> const& sol, int const& num_cities ) 
{
    std::vector<int> visits( num_cities, 0 );

    for ( int const& i : sol ) {
        ++visits[i];
    }

    return sol.front() == 0 
        && sol.back() == 0
        && visits[0] == 2
        && std::accumulate( visits.begin()+1, visits.end(), 1, std::multiplies<int>() ) == 1;
}

void run( char const * filename ) {
    std::vector<int> sol = SolveTSP( filename );

    MAP map;
    int num_cities;
    // load & check
    read( filename, map, num_cities );

    int prev = 0;
    int total = 0;

    for( int i=1; i<num_cities+1; ++i ) {
        total += map[prev][sol[i]];
        prev=sol[i];
    }
    if ( check_legal( sol, num_cities ) ) {
        std::cout << total << '\n';
    } else {
        std::cout << "Path is illegal\n";
    }
}

void test0() { run( "map0" ); }
void test1() { run( "map1" ); }
void test2() { run( "map2" ); }
void test3() { run( "map3" ); }
void test4() { run( "map4" ); }
void test5() { run( "map5" ); }
void test6() { run( "map6" ); }
void test7() { run( "map7" ); }
void test8() { run( "map8" ); }
void test9() { run( "map9" ); }
void test10() { run( "map10" ); }
void test11() { run( "map11" ); }

void (*pTests[])() = { 
    test0,test1,test2,test3,test4,test5,
    test6,test7,test8,test9,test10,test11
}; 

int count(int S[], int m, int n)
{
    // table[i] will be storing the number of solutions for 
    // value i. We need n+1 rows as the table is constructed 
    // in bottom up manner using the base case (n = 0) 
    std::vector<int> table(n + 1);


    // Base case (If given value is 0) 
    table[0] = 1;

    // Pick all coins one by one and update the table[] values 
    // after the index greater than or equal to the value of the 
    // picked coin 
    for (int i = 0; i < m; i++)
        for (int j = S[i]; j <= n; j++)
            table[j] += table[j - S[i]];

    return table[n];
}

#include <cstdio> // sscanf
int main(int argc, char ** argv) {
    int S[] = { 1,7,10 };
    count(S, 3, 14);
    return 0;
}
