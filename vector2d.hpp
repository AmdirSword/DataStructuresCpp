/**
 * 2d vector.
 */
#pragma once

/****************************************************************************************/

#include <vector>
#include <string>
#include <stdexcept>

#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

#include <stdint.h>

#ifdef DEBUG_DS_VECTOR2D
#include <assert.h>
#endif /* DEBUG_DS_VECTOR2D */

/****************************************************************************************/

namespace ds
{
    namespace vector2d
    {
        /********************************************************************************/
    
        /**
         * 2d point.
         */
        struct point2d
        {
            int32_t x = 0;
            int32_t y = 0;
            
            point2d( int32_t new_x, int32_t new_y ) : x( new_x ), y( new_y ) {}
            
            inline operator std::string() const
            {
                std::string out = "(" + std::to_string(x) + "," + std::to_string(y) + ")";
                return out;
            }
        };
        
        inline point2d operator+( const point2d& p0, const point2d& p1 )
        {
            return point2d( p0.x + p1.x, p0.y + p1.y );
        }
        
        inline point2d operator-( const point2d& p0, const point2d& p1 )
        {
            return point2d( p0.x - p1.x, p0.y - p1.y );
        }
        
        inline bool operator<( const point2d& p0, const point2d& p1 )
        {
            if ( p0.y < p1.y )
            {
                return true;
            } else if ( p0.y > p1.y )
            {
                return false;
            }
            
            if ( p0.x < p1.x )
            {
                return true;
            }
            return false;
        }
        
        inline bool operator!=( const point2d& p0, const point2d& p1 )
        {
            return ( p0 < p1 || p1 < p0 );
        }
        
        inline bool operator==( const point2d& p0, const point2d& p1 )
        {
            return !( p0 != p1 );
        }
        
        /********************************************************************************/
        
        /**
         * 2d vector.
         */
        template <typename T>
        class vector2d
        {
        private:
            point2d m_size;
            
            std::vector<T> data;
            
        private:
            bool CheckIndex( const point2d& pos )
            {
                if ( pos.x >= m_size.x || pos.y >= m_size.y || pos.x < 0 || pos.y < 0 )
                {
                    return false;
                }
                return true;
            }
            
            int32_t CountIndexWithoutCheck( const point2d& pos )
            {
                return ( pos.y * m_size.x + pos.x );
            }
            
            int32_t CountIndex( const point2d& pos )
            {
                if ( !CheckIndex( pos ) )
                {
                    throw std::out_of_range( "index " + (std::string)pos + " is out of size " + (std::string)m_size );
                }
                
                return CountIndexWithoutCheck( pos );
            }
            
            int32_t SizeInt()
            {
                return ( m_size.x * m_size.y );
            }
            
        public:
            vector2d( const point2d& new_size ) : m_size( new_size )
            {
                if ( new_size.x < 0 || new_size.y < 0 )
                {
                    throw std::out_of_range( "size " + (std::string)new_size + " has parts < 0" );
                }
                data.reserve( new_size.x * new_size.y );
                data.resize(  new_size.x * new_size.y );
            }
            vector2d( int32_t x, int32_t y ) : m_size( point2d( x, y ) )
            {
                if ( m_size.x < 0 || m_size.y < 0 )
                {
                    throw std::out_of_range( "size " + (std::string)m_size + " has parts < 0" );
                }
                data.reserve( m_size.x * m_size.y );
                data.resize(  m_size.x * m_size.y );
            }
            
            point2d size() const
            {
                return m_size;
            }
            
            void resize( const point2d& new_size )
            {
                if ( new_size.x < 0 || new_size.y < 0 )
                {
                    throw std::out_of_range( "size " + (std::string)new_size + " has parts < 0" );
                }
                
                std::vector<T> new_data;
                new_data.reserve( new_size.x * new_size.y );
                
                for ( int32_t y = 0 ; y < new_size.y; y++ )
                {
                    for ( int32_t x = 0 ; x < new_size.x; x++ )
                    {
                        if ( x < m_size.x && y < m_size.y )
                        {
                            new_data[ new_size.x * y + x ] = data[ m_size.x * y + x ];
                        }
                    }
                }
                
                data = new_data;
                
                return;
            }
            
            T& operator() ( const point2d& pos )
            {
                if ( !CheckIndex( pos ) )
                {
                    throw std::out_of_range ( "index " + (std::string)pos + " is out of size " + (std::string)m_size );
                }
                return data.at( CountIndex( pos ) );
            }
            
            T  operator() ( const point2d& pos ) const
            {
                if ( !CheckIndex( pos ) )
                {
                    throw std::out_of_range ( "index " + (std::string)pos + " is out of size " + (std::string)m_size );
                }
                return data.at( CountIndex( pos ) );
            }
            
            T& operator() ( int32_t x, int32_t y )
            {
                point2d pos = { x, y };
                if ( !CheckIndex( pos ) )
                {
                    throw std::out_of_range ( "index " + (std::string)pos + " is out of size " + (std::string)m_size );
                }
                return data.at( CountIndex( pos ) );
            }
            
            T  operator() ( int32_t x, int32_t y ) const
            {
                point2d pos = { x, y };
                if ( !CheckIndex( pos ) )
                {
                    throw std::out_of_range ( "index " + (std::string)pos + " is out of size " + (std::string)m_size );
                }
                return data.at( CountIndex( pos ) );
            }
            
            typename std::vector<T>::iterator begin()
            {
                return data.begin();
            }
            typename std::vector<T>::iterator end()
            {
                return data.end();
            }
        };
    }
    
    /************************************************************************************/
}

/****************************************************************************************/
