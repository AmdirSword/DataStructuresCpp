/**
 * Oriented graph.
 */
#pragma once

/****************************************************************************************/

/**
 * What do I want from orgraph:
 *      1.  Filling and editing graph:
 *          1.  Creating node - returns ref to node.
 *          2.  Deleting node by ref.
 *          3.  Connecting 2 nodes - returns ref to edge.
 *          4.  Deleting edge by ref.
 *          5.  Changing data in node by ref.
 *          6.  Changing data in edge by ref.
 *      2.  Lookup:
 *          7.  Looking for node with specified data.
 *          8.  Looking for edge with specified data.
 *          9.  Iterating through all nodes.
 *          10. Iterating through all edges.
 *          11. Iterating through all pred edges of node.
 *          12. Iterating through all succ edges of node.
 * 
 * So there are 3 visible types:
 *      1.  orgraph<Tnode,Tedge>
 *      2.  node_ref<Tnode> - works as ref to edge
 *      3.  edge_ref<Tedge> - works as ref to edge
 * And others are invisible:
 * 
 * Oriented graph interface (visible methods):
 *      
 *      class orgraph<Tnode,Tedge>
 *      {
 *          
 *      };
 */

/****************************************************************************************/

#include <vector>
#include <map>
#include <set>
#include <string>
#include <stdexcept>
#include <functional>
#include <optional>

#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

#include <stdint.h>

#ifdef DEBUG_DS_ORGRAPH
#include <assert.h>
#endif /* DEBUG_DS_ORGRAPH */

/****************************************************************************************/

namespace ds
{
    namespace orgraph
    {
        /********************************************************************************/
        
        /**
         * Forward declarations.
         */
        template <typename Tnode, typename Tedge> class node_ref;
        template <typename Tnode, typename Tedge> class edge_ref;
        template <typename Tnode, typename Tedge> class orgraph;
        
        /********************************************************************************/
        
        template <typename Tnode, typename Tedge>
        class orgraph
        {
            /*****************************************************************************
                                    Inner types and friend types
            *****************************************************************************/
            friend class node_ref<Tnode,Tedge>;
            friend class edge_ref<Tnode,Tedge>;
            
        private:
            /****************************************************************************/
            
            /**
             * Class of node id.
             * Is needed to distinguish different id's in graph member types.
             */
            class node_id
            {
            private:
                int32_t m_id;
                
            public:
                node_id( int32_t new_id ) : m_id( new_id ) {}
                
                int32_t& operator() ()
                {
                    return m_id;
                }
                
                int32_t operator() () const
                {
                    return m_id;
                }
                
                bool operator<( const node_id& n1 ) const
                {
                    return ( m_id < n1() );
                }
                
                node_id next() const
                {
                    return node_id( m_id + 1 );
                }
            };
            
            /****************************************************************************/
            
            /**
             * Class of edge id.
             * Is needed to distinguish different id's in graph member types.
             */
            class edge_id
            {
            private:
                int32_t m_id;
                
            public:
                edge_id( int32_t new_id ) : m_id( new_id ) {}
                
                int32_t& operator() ()
                {
                    return m_id;
                }
                
                int32_t operator() () const
                {
                    return m_id;
                }
            
                bool operator<( const edge_id& n1 ) const
                {
                    return ( m_id < n1() );
                }
                
                edge_id next() const
                {
                    return edge_id( m_id + 1 );
                }
            };
            
            /****************************************************************************/
            
            /**
             * Node class.
             */
            class node
            {
            private:
                Tnode m_data;
            
                // these fields are set once in constructor and then cannot be changed
                const node_id                m_id;
                orgraph<Tnode,Tedge> * const m_graph_p;
                
                std::set<edge_id> m_pred_edges;
                std::set<edge_id> m_succ_edges;
                
            public:
                node( orgraph<Tnode,Tedge> * const graph_p, const node_id id ) :
                    m_id( id ),
                    m_graph_p( graph_p )
                {}
                node( orgraph<Tnode,Tedge> * const graph_p, const node_id id,
                      const Tnode& data ) :
                    m_id( id ),
                    m_graph_p( graph_p ),
                    m_data( data )
                {}
                
                Tnode& data()
                {
                    return m_data;
                }
                
                const Tnode data() const
                {
                    return m_data;
                }
                
                const node_id id() const
                {
                    return m_id;
                }
                
                void add_pred_edge_id( edge_id pred_edge )
                {
                    m_pred_edges.insert( pred_edge );
                    return;
                }
                
                void add_succ_edge_id( edge_id succ_edge )
                {
                    m_succ_edges.insert( succ_edge );
                    return;
                }
                
                void remove_pred_edge_id( edge_id pred_edge )
                {
                    m_pred_edges.erase( pred_edge );
                    return;
                }
                
                void remove_succ_edge_id( edge_id succ_edge )
                {
                    m_succ_edges.erase( succ_edge );
                    return;
                }
                
                std::set<edge_id>& preds()
                {
                    return m_pred_edges;
                }
                
                std::set<edge_id>& succs()
                {
                    return m_succ_edges;
                }
                
                node_ref<Tnode,Tedge> make_ref() const
                {
                    return node_ref<Tnode,Tedge>( m_graph_p, m_id );
                }
            };
            
            /****************************************************************************/
            
            /**
             * Edge class.
             * Its pred and succ are unchangeable.
             */
            class edge
            {
            private:
                Tedge m_data;
            
                // these fields are set once in constructor and then cannot be changed
                const edge_id                m_id;
                orgraph<Tnode,Tedge> * const m_graph_p;
                const node_id m_pred_node;
                const node_id m_succ_node;
                
            public:
                edge( orgraph<Tnode,Tedge> * const graph_p, const edge_id id,
                      node_id pred_node, node_id succ_node ) :
                    m_id( id ),
                    m_graph_p( graph_p ),
                    m_pred_node( pred_node ),
                    m_succ_node( succ_node )
                {}
                edge( orgraph<Tnode,Tedge> * const graph_p, const edge_id id,
                      const Tnode& data,
                      const node_id pred_node, const node_id succ_node ) :
                    m_id( id ),
                    m_graph_p( graph_p ),
                    m_data( data ),
                    m_pred_node( pred_node ),
                    m_succ_node( succ_node )
                {}
                
                Tedge& data()
                {
                    return m_data;
                }
                
                const Tedge data() const
                {
                    return m_data;
                }
                
                const edge_id id() const
                {
                    return m_id;
                }
                
                const node_id pred() const
                {
                    return m_pred_node;
                }
                
                const node_id succ() const
                {
                    return m_succ_node;
                }
                
                edge_ref<Tnode,Tedge> make_ref() const
                {
                    return edge_ref<Tnode,Tedge>( m_graph_p, m_id );
                }
            };
            
            /*****************************************************************************
                                                Data
            *****************************************************************************/
        private:
            /**
             * Data of orgraph.
             */
            std::map< node_id, node > m_nodes;
            std::map< edge_id, edge > m_edges;
            
            node_id next_node_id = node_id( 0 );
            edge_id next_edge_id = edge_id( 0 );
            
            /*****************************************************************************
                                          Public interface
            *****************************************************************************/
        public:
            /**
             * Adds node.
             * Returns: ref to created node.
             */
            node_ref<Tnode,Tedge> add_node( const Tnode& new_node_data )
            {
                node new_node( this, next_node_id, new_node_data );
                auto [it,success] = m_nodes.insert( { next_node_id, new_node } );
#ifdef DEBUG_DS_ORGRAPH
                assert( success );
#endif /* DEBUG_DS_ORGRAPH */
                node_ref<Tnode,Tedge> ref = it->second.make_ref();
                
                next_node_id = next_node_id.next();
                
                return ref;
            }
            
            /**
             * Adds edge.
             * Returns: ref to created edge.
             */
            edge_ref<Tnode,Tedge> add_edge( const Tedge& new_edge_data,
                                            node_ref<Tnode,Tedge> node_start,
                                            node_ref<Tnode,Tedge> node_end )
            {
                edge new_edge( this, next_edge_id, new_edge_data,
                               node_start.id(), node_end.id() );
                auto [it,success] = m_edges.insert( { next_edge_id, new_edge } );
#ifdef DEBUG_DS_ORGRAPH
                assert( success );
#endif /* DEBUG_DS_ORGRAPH */
                edge_ref<Tnode,Tedge> ref = it->second.make_ref();
                
                next_edge_id = next_edge_id.next();
                
                m_nodes.at( node_start.id() ).add_succ_edge_id( new_edge.id() );
                m_nodes.at( node_end.id() ).add_pred_edge_id( new_edge.id() );
                
                return ref;
            }
            
            /**
             * Removes node.
             * Invalidates all refs to this node and all refs to neighbour edges
             * as all neighbour edges are also removed.
             */
            void remove_node( const node_ref<Tnode,Tedge>& rm_node_ref )
            {
                std::vector<edge_id> copy_pred_edges;
                for ( auto e : m_nodes.at( rm_node_ref.id() ).preds() )
                {
                    copy_pred_edges.push_back( e );
                }
                for ( auto e : copy_pred_edges )
                {
                    remove_edge( m_edges.at( e ).make_ref() );
                }
                
                std::vector<edge_id> copy_succ_edges;
                for ( auto e : m_nodes.at( rm_node_ref.id() ).succs() )
                {
                    copy_succ_edges.push_back( e );
                }
                for ( auto e : copy_succ_edges )
                {
                    remove_edge( m_edges.at( e ).make_ref() );
                }
                
                m_nodes.erase( rm_node_ref.id() );
                return;
            }
            
            /**
             * Removes edge. Invalidates all refs to this edge.
             */
            void remove_edge( const edge_ref<Tnode,Tedge>& rm_edge_ref )
            {
                node_id pred_node_id = m_edges.at( rm_edge_ref.id() ).pred();
                m_nodes.at( pred_node_id ).remove_succ_edge_id( rm_edge_ref.id() );
                
                node_id succ_node_id = m_edges.at( rm_edge_ref.id() ).succ();
                m_nodes.at( succ_node_id ).remove_pred_edge_id( rm_edge_ref.id() );
                
                m_edges.erase( rm_edge_ref.id() );
                return;
            }
            
            /**
             * Finds node with specified data.
             * Returns: ref to found node.
             * Note: Tnode should have implemented operator==.
             */
            std::optional< node_ref<Tnode,Tedge> > find_node( Tnode node_data ) const
            {
                for ( const auto& [id, cur_node] : m_nodes )
                {
                    if ( cur_node.data() == node_data )
                    {
                        return std::optional{ cur_node.make_ref() };
                    }
                }
                
                return std::nullopt;
            }
            
            /**
             * Finds edge with specified data.
             * Returns: ref to found edge.
             * Note: Tedge should have implemented operator==.
             */
            std::optional< edge_ref<Tnode,Tedge> > find_edge( Tedge edge_data ) const
            {
                for ( const auto& [id, cur_edge] : m_edges )
                {
                    if ( cur_edge.data() == edge_data )
                    {
                        return std::optional{ cur_edge.make_ref() };
                    }
                }
                
                return std::nullopt;
            }
            
            /**
             * Gives a container of refs to all nodes of graph.
             */
            std::vector< node_ref<Tnode,Tedge> > nodes() const
            {
                std::vector< node_ref<Tnode,Tedge> > out;
                
                for ( const auto& [id, cur_node] : m_nodes )
                {
                    out.push_back( cur_node.make_ref() );
                }
                
                return out;
            }
            
            /**
             * Gives a container of refs to all edges of graph.
             */
            std::vector< edge_ref<Tnode,Tedge> > edges() const
            {
                std::vector< edge_ref<Tnode,Tedge> > out;
                
                for ( const auto& [id, cur_edge] : m_edges )
                {
                    out.push_back( cur_edge.make_ref() );
                }
                
                return out;
            }
        };
        
        /********************************************************************************/
        
        template <typename Tnode, typename Tedge>
        class node_ref
        {
            /*****************************************************************************
                                    Inner types and friend types
            *****************************************************************************/
            friend class edge_ref<Tnode,Tedge>;
            friend class orgraph<Tnode,Tedge>;
            
            /*****************************************************************************
                                                Data
            *****************************************************************************/
        private:
            const typename orgraph<Tnode,Tedge>::node_id m_id;
            orgraph<Tnode,Tedge> * const m_graph_p;
            
            /*****************************************************************************
                                Accessible by friend classes interface
            *****************************************************************************/
        private:
            const typename orgraph<Tnode,Tedge>::node_id id() const
            {
                return m_id;
            }
            
            node_ref( orgraph<Tnode,Tedge> * const graph_p,
                      const typename orgraph<Tnode,Tedge>::node_id id ) :
                m_id( id ),
                m_graph_p( graph_p )
            {}
            
            /*****************************************************************************
                                          Public interface
            *****************************************************************************/
        public:
            /**
             * Dereference operator for accessing data of node.
             */
            Tnode& operator*()
            {
                return m_graph_p->m_nodes.at( m_id ).data();
            }
            
            /**
             * Dereference operator for accessing data of node.
             */
            const Tnode operator*() const
            {
                return m_graph_p->m_nodes.at( m_id ).data();
            }
            
            /**
             * Gives a container of refs to all pred edges of node.
             */
            std::vector< edge_ref<Tnode,Tedge> > pred_edges() const
            {
                std::vector< edge_ref<Tnode,Tedge> > out;
                
                std::set< typename orgraph<Tnode,Tedge>::edge_id >& edge_ids =
                    m_graph_p->m_nodes.at( m_id ).preds();
                for ( const auto& cur_edge_id : edge_ids )
                {
                    out.push_back( m_graph_p->m_edges.at( cur_edge_id ).make_ref() );
                }
                
                return out;
            }
            
            /**
             * Gives a container of refs to all pred nodes of node.
             */
            std::vector< node_ref<Tnode,Tedge> > pred_nodes() const
            {
                std::vector< node_ref<Tnode,Tedge> > out;
                
                std::set< typename orgraph<Tnode,Tedge>::edge_id >& edge_ids =
                    m_graph_p->m_nodes.at( m_id ).preds();
                for ( const auto& cur_edge_id : edge_ids )
                {
                    typename orgraph<Tnode,Tedge>::node_id cur_node_id =
                        m_graph_p->m_edges.at( cur_edge_id ).pred();
                    out.push_back( m_graph_p->m_nodes.at( cur_node_id ).make_ref() );
                }
                
                return out;
            }
            
            /**
             * Gives a container of refs to all succ edges of node.
             */
            std::vector< edge_ref<Tnode,Tedge> > succ_edges() const
            {
                std::vector< edge_ref<Tnode,Tedge> > out;
                
                std::set< typename orgraph<Tnode,Tedge>::edge_id >& edge_ids =
                    m_graph_p->m_nodes.at( m_id ).succs();
                for ( const auto& cur_edge_id : edge_ids )
                {
                    out.push_back( m_graph_p->m_edges.at( cur_edge_id ).make_ref() );
                }
                
                return out;
            }
            
            /**
             * Gives a container of refs to all succ nodes of node.
             */
            std::vector< node_ref<Tnode,Tedge> > succ_nodes() const
            {
                std::vector< node_ref<Tnode,Tedge> > out;
                
                std::set< typename orgraph<Tnode,Tedge>::edge_id >& edge_ids =
                    m_graph_p->m_nodes.at( m_id ).succs();
                for ( const auto& cur_edge_id : edge_ids )
                {
                    typename orgraph<Tnode,Tedge>::node_id cur_node_id =
                        m_graph_p->m_edges.at( cur_edge_id ).succ();
                    out.push_back( m_graph_p->m_nodes.at( cur_node_id ).make_ref() );
                }
                
                return out;
            }
        };
        
        /********************************************************************************/
        
        template <typename Tnode, typename Tedge>
        class edge_ref
        {
            /*****************************************************************************
                                    Inner types and friend types
            *****************************************************************************/
            friend class node_ref<Tnode,Tedge>;
            friend class orgraph<Tnode,Tedge>;
            
        private:
            
            /*****************************************************************************
                                                Data
            *****************************************************************************/
        private:
            const typename orgraph<Tnode,Tedge>::edge_id m_id;
            orgraph<Tnode,Tedge> * const m_graph_p;
            
            /*****************************************************************************
                                Accessible by friend classes interface
            *****************************************************************************/
        private:
            const typename orgraph<Tnode,Tedge>::edge_id id() const
            {
                return m_id;
            }
            
            edge_ref( orgraph<Tnode,Tedge> * const graph_p,
                      const typename orgraph<Tnode,Tedge>::edge_id id ) :
                m_id( id ),
                m_graph_p( graph_p )
            {}
            
            /*****************************************************************************
                                          Public interface
            *****************************************************************************/
        public:
            /**
             * Dereference operator for accessing data of edge.
             */
            Tedge& operator*()
            {
                return m_graph_p->m_edges.at( m_id ).data();
            }
            
            /**
             * Dereference operator for accessing data of edge.
             */
            const Tedge operator*() const
            {
                return m_graph_p->m_edges.at( m_id ).data();
            }
            
            /**
             * Dereference operator for accessing data of edge.
             */
            const node_ref<Tnode,Tedge> pred() const
            {
                typename orgraph<Tnode,Tedge>::node_id pred_node_id =
                    m_graph_p->m_edges.at( m_id ).pred();
                return m_graph_p->m_nodes.at( pred_node_id ).make_ref();
            }
            
            /**
             * Dereference operator for accessing data of edge.
             */
            const node_ref<Tnode,Tedge> succ() const
            {
                typename orgraph<Tnode,Tedge>::node_id succ_node_id =
                    m_graph_p->m_edges.at( m_id ).succ();
                return m_graph_p->m_nodes.at( succ_node_id ).make_ref();
            }
        };
        
        /********************************************************************************/
    }
}

/****************************************************************************************/

#if 0

/****************************************************************************************/

namespace ds
{
    namespace orgraph
    {
        /********************************************************************************/
        
        /**
         * Forward declarations.
         */
        template <typename T> class node;
        template <typename T> class edge;
        template <typename T> class orgraph;
        
        /********************************************************************************/
        
        /**
         * Class of node id.
         * Is needed to distinguish different id's in graph member types.
         */
        class node_id
        {
        private:
            int32_t m_id;
            
        public:
            node_id( int32_t new_id ) : m_id( new_id ) {}
            
            int32_t& operator() ()
            {
                return m_id;
            }
            
            int32_t operator() () const
            {
                return m_id;
            }
            
            int32_t& at()
            {
                return m_id;
            }
            
            int32_t at() const
            {
                return m_id;
            }
        };
        
        bool operator<( const node_id& n0, const node_id& n1 )
        {
            return ( n0() < n1() );
        }
        
        /**
         * Class of edge id.
         * Is needed to distinguish different id's in graph member types.
         */
        class edge_id
        {
        private:
            int32_t m_id;
            
        public:
            edge_id( int32_t new_id ) : m_id( new_id ) {}
            
            int32_t& operator() ()
            {
                return m_id;
            }
            
            int32_t operator() () const
            {
                return m_id;
            }
            
            int32_t& at()
            {
                return m_id;
            }
            
            int32_t at() const
            {
                return m_id;
            }
        };
        
        bool operator<( const edge_id& n0, const edge_id& n1 )
        {
            return ( n0() < n1() );
        }
        
        /********************************************************************************/
        
        /**
         * Something like pointer to node in orgraph.
         */
        template <typename T>
        class node_ref
        {
            friend class node<T>;
            
            template<typename T2>
            friend class orgraph<T,T2>;
            
        private:
            const node_id     m_id;
            const orgraph<T> *m_graph_p;
            
        public:
            template<typename T2>
            node_ref( orgraph<T,T2> *graph_p, node_id id ) : m_graph_p( graph_p ),
                                                             m_id( id )
            {}
            
            node<T>& operator*()
            {
#ifdef DEBUG_DS_ORGRAPH
                assert( nullptr != m_graph_p );
#endif /* DEBUG_DS_ORGRAPH */
                return m_graph_p->at( m_id );
            }
            
            node<T>* operator->()
            {
#ifdef DEBUG_DS_ORGRAPH
                assert( nullptr != m_graph_p );
#endif /* DEBUG_DS_ORGRAPH */
                return &( m_graph_p->at( m_id ) );
            }
            
            friend bool operator<( const node_ref& r0, const node_ref& r1 );
        };
        
        template <typename T>
        bool operator<( const node_ref<T>& r0, const node_ref<T>& r1 )
        {
            if ( r0.m_graph_p != r1.m_graph_p )
            {
                return std::less< orgraph<T>* >{}( r0.m_graph_p, r1.m_graph_p );
            }
            
            return ( r0.m_id < r1.m_id );
        }
        
        /********************************************************************************/
        
        /**
         * Something like pointer to node in orgraph.
         */
        template <typename T>
        class edge_ref
        {
            friend class edge<T>;
            
            template<typename T2>
            friend class orgraph<T2,T>;
            
        private:
            const edge_id     m_id;
            const orgraph<T> *m_graph_p;
            
        public:
            template<typename T2>
            edge_ref( orgraph<T2,T> *graph_p, edge_id id ) : m_graph_p( graph_p ),
                                                             m_id( id )
            {}
            
            edge<T>& operator*()
            {
#ifdef DEBUG_DS_ORGRAPH
                assert( nullptr != m_graph_p );
#endif /* DEBUG_DS_ORGRAPH */
                return m_graph_p->at( m_id );
            }
            
            T* operator->()
            {
#ifdef DEBUG_DS_ORGRAPH
                assert( nullptr != m_graph_p );
#endif /* DEBUG_DS_ORGRAPH */
                return &( m_graph_p->at( m_id ) );
            }
            
            friend bool operator<( const node_ref& r0, const node_ref& r1 );
        };
        
        template <typename T>
        bool operator<( const node_ref<T>& r0, const node_ref<T>& r1 )
        {
            if ( r0.m_graph_p != r1.m_graph_p )
            {
                return std::less< orgraph<T>* >{}( r0.m_graph_p, r1.m_graph_p );
            }
            
            return ( r0.m_id < r1.m_id );
        }
        
        /********************************************************************************/
        
        /**
         * Graph node.
         */
        template <typename T>
        class node
        {
            template<typename T2>
            friend class edge<T2>;
            
            template<typename T2>
            friend class orgraph<T,T2>;
            
            friend class node_ref<T>;
            
        private:
            T m_data;
            
            // these fields are set once in constructor and then cannot be changed
            const node_id     m_id;
            const orgraph<T> *m_graph_p;
            
            std::set<edge_id> m_pred_edges;
            std::set<edge_id> m_succ_edges;
            
        public:
            template<typename T2>
            node( orgraph<T,T2> *graph_p, node_id id ) : m_graph_p( graph_p ),
                                                         m_id( id )
            {}
            template<typename T2>
            node( orgraph<T,T2> *graph_p, node_id id, T data ) : m_graph_p( graph_p ),
                                                                 m_id( id ),
                                                                 m_data( data )
            {}
            
            std::set<edge_id>& preds() const
            {
                return m_pred_edges;
            }
            
            std::set<edge_id>& succs() const
            {
                return m_succ_edges;
            }
            
            node_id id() const
            {
                return m_id;
            }
            
            orgraph<T>* graph() const
            {
                return m_graph_p;
            }
            
            T& operator*()
            {
                return m_data;
            }
            
            T* operator->()
            {
                return &m_data;
            }
            
        private:
            /**
             * Adding preds and succs.
             */
            void add_pred( edge_id new_pred_id )
            {
                m_pred_edges.insert( new_pred_id );
                return;
            }
            void add_pred( const edge& new_pred )
            {
                m_pred_edges.insert( new_pred.id() );
                return;
            }
            
            void add_succ( edge_id new_succ_id )
            {
                m_succ_edges.insert( new_succ_id );
                return;
            }
            void add_succ( const edge& new_succ )
            {
                m_succ_edges.insert( new_succ.id() );
                return;
            }
            
            /**
             * Removing preds and succs.
             */
            void remove_pred( edge_id new_pred_id )
            {
                m_pred_edges.erase( new_pred_id );
                return;
            }
            void remove_pred( const edge& new_pred )
            {
                m_pred_edges.erase( new_pred.id() );
                return;
            }
            
            void remove_succ( edge_id new_succ_id )
            {
                m_succ_edges.erase( new_succ_id );
                return;
            }
            void remove_succ( const edge& new_succ )
            {
                m_succ_edges.erase( new_succ.id() );
                return;
            }
        };
        
        /********************************************************************************/
        
        /**
         * Graph edge.
         * Pred and succ nodes are not changeable.
         */
        template <typename T>
        class edge
        {
            template<typename T2>
            friend class node<T2>;
            
            template<typename T2>
            friend class orgraph<T2,T>;
            
            friend class edge_ref<T>;
            
        private:
            T m_data;
            
            // these fields are set once in constructor and then cannot be changed
            const edge_id     m_id;
            const orgraph<T> *m_graph_p;
            const node_id     m_pred_node;
            const node_id     m_succ_node;
            
        public:
            template<typename T2>
            edge( orgraph<T2,T> *graph_p, edge_id id,
                  node_id pred_node, node_id succ_node ) :
                m_graph_p( graph_p ),
                m_id( id ),
                m_pred_node( pred_node ),
                m_succ_node( succ_node )
            {}
            template<typename T2>
            edge( orgraph<T2,T> *graph_p, edge_id id, T data,
                  node_id pred_node, node_id succ_node ) :
                m_graph_p( graph_p ),
                m_id( id ),
                m_data( data )
            {}
            
            edge_id id() const
            {
                return m_id;
            }
            
            orgraph<T>* graph() const
            {
                return m_graph_p;
            }
            
            node_id pred()
            {
                return m_pred_node;
            }
            
            node_id succ()
            {
                return m_succ_node;
            }
            
            T& operator*()
            {
                return m_data;
            }
            
            T* operator->()
            {
                return &m_data;
            }
        };
        
        /********************************************************************************/
        
        /**
         * Graph.
         */
        template <typename Tnode, typename Tedge>
        class orgraph
        {
            friend class node<Tnode>;
            friend class edge<Tedge>;
            
            friend class node_ref<Tnode>;
            friend class edge_ref<Tedge>;
            
        private:
            std::map< node_id, node<Tnode> > m_nodes;
            std::map< edge_id, edge<Tedge> > m_edges;
            
        private:
            node<Tnode>& at( const node_id& n )
            {
                return m_nodes.at( n );
            }
            node<Tnode> at( const node_id& n ) const
            {
                return m_nodes.at( n );
            }
            node<Tnode>& operator() ( const node_id& n )
            {
                return m_nodes.at( n );
            }
            node<Tnode> operator() ( const node_id& n ) const
            {
                return m_nodes.at( n );
            }
            
            edge<Tedge>& at( const edge_id& e )
            {
                return m_edges.at( e );
            }
            edge<Tedge> at( const edge_id& e ) const
            {
                return m_edges.at( e );
            }
            edge<Tedge>& operator() ( const edge_id& e )
            {
                return m_edges.at( e );
            }
            edge<Tedge> operator() ( const edge_id& e ) const
            {
                return m_edges.at( e );
            }
        };
        
        /********************************************************************************/
    }
}

/****************************************************************************************/
#endif /* 0 */
