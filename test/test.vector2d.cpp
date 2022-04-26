#include <string>
#include <stdexcept>

#include <stdio.h>

#include "vector2d.hpp"

void Set( ds::vector2d::vector2d<int> &v, int32_t x, int32_t y, int val )
{
    try
    {
        v(x,y) = val;
    } catch ( const std::out_of_range& oor )
    {
        printf( "Out of Range error at(%d,%d): %s\n", x, y, oor.what() );
    }
    
    return;
}

int main( void )
{
    ds::vector2d::vector2d<int> v(3,2);
    
    Set(v,0,0,0);
    Set(v,0,1,1);
    Set(v,1,0,2);
    Set(v,1,1,3);
    Set(v,2,0,4);
    Set(v,2,1,5);
    Set(v,2,2,6);
    
    printf( "Values of vector2d:\n" );
    for ( auto val : v )
    {
        printf( "    %d\n", val );
    }
    
    return 0;
}
