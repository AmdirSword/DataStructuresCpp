#include <string>
#include <stdexcept>

#include <stdio.h>

#include "orgraph.hpp"

int main( void )
{
    ds::orgraph::orgraph<int,int> og;
    
    ds::orgraph::node_ref<int,int> start = og.add_node( 1 );
    ds::orgraph::node_ref<int,int> n2    = og.add_node( 0 );
    ds::orgraph::node_ref<int,int> n3    = og.add_node( 3 );
    ds::orgraph::node_ref<int,int> stop  = og.add_node( 4 );
    
    ds::orgraph::edge_ref<int,int> e1 = og.add_edge( 10, start, n2 );
    ds::orgraph::edge_ref<int,int> e2 = og.add_edge(  0, start, n3 );
    ds::orgraph::edge_ref<int,int> e3 = og.add_edge( 30, n2, stop );
    ds::orgraph::edge_ref<int,int> e4 = og.add_edge( 40, n3, stop );
    
    int num_printed = 0;
    for ( auto& e : start.succ_edges() )
    {
        printf( "Succ of start: edge %d\n", *e );
        num_printed++;
    }
    printf( "Num printed: %d\n\n", num_printed );
    // Succ of start: edge 10
    // Succ of start: edge 0
    // Num printed: 2
    
    num_printed = 0;
    for ( auto& n : start.succ_nodes() )
    {
        printf( "Succ of start: node %d\n", *n );
        num_printed++;
    }
    printf( "Num printed: %d\n\n", num_printed );
    // Succ of start: node 0
    // Succ of start: node 3
    // Num printed: 2
    
    *n2 =  2;
    *e2 = 20;
    
    num_printed = 0;
    for ( auto& e : start.succ_edges() )
    {
        printf( "Succ of start: edge %d\n", *e );
        num_printed++;
    }
    printf( "Num printed: %d\n\n", num_printed );
    // Succ of start: edge 10
    // Succ of start: edge 20
    // Num printed: 2
    
    num_printed = 0;
    for ( auto& n : start.succ_nodes() )
    {
        printf( "Succ of start: node %d\n", *n );
        num_printed++;
    }
    printf( "Num printed: %d\n\n", num_printed );
    // Succ of start: node 2
    // Succ of start: node 3
    // Num printed: 2
    
    og.remove_edge( e4 );
    og.remove_node( n2 );
    
    num_printed = 0;
    for ( auto& e : start.succ_edges() )
    {
        printf( "Succ of start: edge %d\n", *e );
        num_printed++;
    }
    printf( "Num printed: %d\n\n", num_printed );
    // Succ of start: edge 20
    // Num printed: 1
    
    num_printed = 0;
    for ( auto& e : stop.pred_edges() )
    {
        printf( "Pred of stop: edge %d\n", *e );
        num_printed++;
    }
    printf( "Num printed: %d\n\n", num_printed );
    // Num printed: 0
    
    return 0;
}
