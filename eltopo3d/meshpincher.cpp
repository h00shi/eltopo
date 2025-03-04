// ---------------------------------------------------------
//
//  meshpincher.cpp
//  Tyson Brochu 2011
//  
//  Look for a loop of three vertices that can be "pinched" to separate a volume into two.
//
// ---------------------------------------------------------

#include "meshpincher.h"

#include "broadphase.h"
#include "collisionpipeline.h"
#include "surftrack.h"

// --------------------------------------------------------
///
///
///
// --------------------------------------------------------

MeshPincher::PrePinchInfo::PrePinchInfo(size_t vertex_index, 
                                        const std::vector<TriangleSet>& connected_components,
                                        const TriangleSet& triangles_to_delete,
                                        const std::vector<Vec3st>& triangles_to_add,
                                        const std::vector<size_t>& vertices_added ) :
    m_vertex_index( vertex_index ),
    m_connected_components(connected_components),
    m_triangles_to_delete(triangles_to_delete),
    m_triangles_to_add(triangles_to_add),
    m_vertices_added(vertices_added)
{}

// --------------------------------------------------------
///
///
///
// --------------------------------------------------------

MeshPincher::PostPinchInfo::PostPinchInfo( PrePinchInfo& pre_pinch_info ) :
    m_pre_pinch_info( pre_pinch_info )
{}

// --------------------------------------------------------
///
/// Partition the triangles incident to a vertex into connected components
///
// --------------------------------------------------------

void MeshPincher::partition_vertex_neighbourhood( size_t vertex_index, std::vector< TriangleSet >& connected_components )
{
    // triangles incident to vertex
	TriangleSet triangles_incident_to_vertex = m_surf.m_mesh.m_vertex_to_triangle_map[vertex_index];
	
    // unvisited triangles which are adjacent to some visited ones and incident to vt
    TriangleSet unvisited_triangles, visited_triangles;
    
    while ( triangles_incident_to_vertex.size() > 0 )
    {
        unvisited_triangles.clear();
        visited_triangles.clear();
        unvisited_triangles.push_back( triangles_incident_to_vertex.back() );
        
        while ( unvisited_triangles.size() > 0 )
        {
            // get an unvisited triangle
            size_t curr_tri = unvisited_triangles.back();
            unvisited_triangles.pop_back();
            
            // delete it from triangles_incident_to_vertex
            triangles_incident_to_vertex.erase( find(triangles_incident_to_vertex.begin(), triangles_incident_to_vertex.end(), curr_tri) );
            
            // put it on closed
            visited_triangles.push_back(curr_tri);
            
            // get find a triangle which is incident to vertex and adjacent to curr_tri
            for ( size_t i = 0; i < triangles_incident_to_vertex.size(); ++i )
            {
                size_t incident_triangle_index =  triangles_incident_to_vertex[i];
                
                if ( curr_tri == incident_triangle_index )
                {
                    continue;
                }
                
                if ( m_surf.m_mesh.triangles_are_adjacent( curr_tri, incident_triangle_index ) )
                {
                    // if not in visited_triangles or unvisited_triangles, put them on unvisited_triangles
                    if ( ( find(unvisited_triangles.begin(), unvisited_triangles.end(), incident_triangle_index) == unvisited_triangles.end() ) &&
                        ( find(visited_triangles.begin(), visited_triangles.end(), incident_triangle_index) == visited_triangles.end() ) ) 
                    {
                        unvisited_triangles.push_back( incident_triangle_index );
                    }
                }
            }
        }
        
        // one connected component = visited triangles
        connected_components.push_back(visited_triangles);
    }   
}

// --------------------------------------------------------
///
/// Duplicate a vertex and move the two copies away from each other slightly
///
// --------------------------------------------------------

bool MeshPincher::pull_apart_vertex( size_t vertex_index, const std::vector< TriangleSet >& connected_components )
{
    double dx = 10.0 * m_surf.m_proximity_epsilon;
    
    TriangleSet triangles_to_delete;
    std::vector< Vec3st > triangles_to_add;
    std::vector< size_t > vertices_added;
    
    // for each connected component except the last one, create a duplicate vertex
    for ( int i = 0; i < (int)connected_components.size() - 1; ++i )
    {
        // duplicate the vertex 
        size_t duplicate_vertex_index = m_surf.add_vertex( m_surf.get_position(vertex_index), m_surf.m_masses[vertex_index] );
        
        vertices_added.push_back( duplicate_vertex_index );
        
        Vec3d centroid( 0.0, 0.0, 0.0 );
        
        // map component triangles to the duplicate vertex
        for ( size_t t = 0; t < connected_components[i].size(); ++t ) 
        {
            // create a new triangle with 2 vertices the same, and one vertex set to the new duplicate vertex
            Vec3st new_triangle = m_surf.m_mesh.get_triangle( connected_components[i][t] ); 
            
            for ( unsigned short v = 0; v < 3; ++v ) 
            {
                if ( new_triangle[v] == vertex_index )
                {
                    new_triangle[v] = duplicate_vertex_index;
                }
                else
                {         
                    centroid += m_surf.get_position( new_triangle[v] );
                }
            }
            
            triangles_to_add.push_back( new_triangle );
            triangles_to_delete.push_back( connected_components[i][t] ); 
        }
        
        // compute the centroid    
        centroid /= ( connected_components[i].size() * 2 );
        
        // move the duplicate vertex towards the centroid
        
        Vec3d added_vertex_position = (1.0 - dx) * m_surf.get_position(duplicate_vertex_index) + dx * centroid;
        
        m_surf.set_position( duplicate_vertex_index, added_vertex_position );
        m_surf.set_newposition( duplicate_vertex_index, added_vertex_position );
        
    }
    
    // check new triangles for collision safety
    
    bool collision_occurs = false;
    
    if ( m_surf.m_collision_safety )
    {
        
        for ( size_t i = 0; i < triangles_to_add.size(); ++i ) 
        {
            const Vec3st& current_triangle = triangles_to_add[i];
            Vec3d low, high;
            
            minmax( m_surf.get_position(current_triangle[0]), m_surf.get_position(current_triangle[1]), m_surf.get_position(current_triangle[2]), low, high );
            
            std::vector<size_t> overlapping_triangles;
            m_surf.m_broad_phase->get_potential_triangle_collisions( low, high, true, true, overlapping_triangles );
            
            for ( size_t j=0; j < overlapping_triangles.size(); ++j )
            {        
                const Vec3st& tri_j = m_surf.m_mesh.get_triangle(overlapping_triangles[j]);
                
                assert( tri_j[0] != tri_j[1] );
                
                if ( check_triangle_triangle_intersection( current_triangle, tri_j, m_surf.get_positions() ) )
                {
                    // collision occurs - abort separation
                    collision_occurs = true;
                    break;
                }
            }
        }
        
        // check new triangles vs each other as well
        for ( size_t i = 0; i < triangles_to_add.size(); ++i ) 
        {
            for ( size_t j = i+1; j < triangles_to_add.size(); ++j ) 
            {
                if ( check_triangle_triangle_intersection( triangles_to_add[i], triangles_to_add[j], m_surf.get_positions() ) )
                {
                    // collision occurs - abort separation
                    collision_occurs = true;
                    break;
                }         
            }
        }
    }
    
    // abort separation, remove added vertices and return
    
    if ( collision_occurs )
    {
        for ( size_t i = 0; i < vertices_added.size(); ++i )
        {
            m_surf.remove_vertex( vertices_added[i] );
        }
        return false;
    }
    
    
    PrePinchInfo pre_pinch_info( vertex_index, connected_components, triangles_to_delete, triangles_to_add, vertices_added );
    
                                  
    // all new triangles check out okay for collision safety.  Add them to the data structure.
    
    for ( size_t i = 0; i < triangles_to_add.size(); ++i )
    {
        m_surf.add_triangle( triangles_to_add[i] );
    }
    
    for ( size_t i = 0; i < triangles_to_delete.size(); ++i )
    {
        m_surf.remove_triangle( triangles_to_delete[i] );
    }
    
    
    if ( m_surf.m_collision_safety )
    {
        m_surf.m_collision_pipeline.assert_mesh_is_intersection_free(false);
    }
    
    if ( m_surf.m_verbose ) { std::cout << "pulled apart a vertex" << std::endl; }
    
    
    PostPinchInfo post_pinch_info( pre_pinch_info );
                                  
    
    return true;
}


// --------------------------------------------------------
///
/// Find vertices with disconnected neighbourhoods, and pull them apart
///
// --------------------------------------------------------

void MeshPincher::process_mesh()
{
    
    for ( size_t i = 0; i < m_surf.get_num_vertices(); ++i )
    {
        // Partition the set of triangles adjacent to this vertex into connected components
        std::vector< TriangleSet > connected_components;
        partition_vertex_neighbourhood( i, connected_components );
        
        if ( connected_components.size() > 1 ) 
        {
            pull_apart_vertex( i, connected_components );
        }
    }
}

// --------------------------------------------------------
///
/// 
///
// --------------------------------------------------------

void MeshPincher::add_observer( MeshPinchObserver* observer )
{
    m_observers.push_back(observer);
}


