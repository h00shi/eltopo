// ---------------------------------------------------------
//
//  broadphasegrid.h
//  Tyson Brochu 2008
//  
//  Broad phase collision detection culling using three regular, volumetric grids.
//
// ---------------------------------------------------------

#ifndef EL_TOPO_BROADPHASEGRID_H
#define EL_TOPO_BROADPHASEGRID_H

// ---------------------------------------------------------
// Nested includes
// ---------------------------------------------------------

#include "broadphase.h"
#include "accelerationgrid.h"

// ---------------------------------------------------------
//  Forwards and typedefs
// ---------------------------------------------------------

class DynamicSurface;

// ---------------------------------------------------------
//  Class definitions
// ---------------------------------------------------------

// --------------------------------------------------------
///
/// Broad phase collision detector using three regular grids: one grid each for vertices, edges and triangles.
///
// --------------------------------------------------------

class BroadPhaseGrid : public BroadPhase
{
public:
    
    BroadPhaseGrid() :
        m_solid_vertex_grid(),
        m_solid_edge_grid(),
        m_solid_triangle_grid(),
        m_dynamic_vertex_grid(),
        m_dynamic_edge_grid(),
        m_dynamic_triangle_grid()
    {}
    
    ~BroadPhaseGrid() 
    {}
    
    /// Rebuild the broad phase
    ///
    void update_broad_phase( const DynamicSurface& surface, bool continuous );
    
    inline void add_vertex( size_t index,
                           const Vec3d& aabb_low,
                           const Vec3d& aabb_high,
                           bool is_solid );
    
    inline void add_edge( size_t index,
                         const Vec3d& aabb_low,
                         const Vec3d& aabb_high,
                         bool is_solid );
    
    inline void add_triangle( size_t index,
                             const Vec3d& aabb_low,
                             const Vec3d& aabb_high,
                             bool is_solid );
    
    inline void update_vertex( size_t index,
                              const Vec3d& aabb_low,
                              const Vec3d& aabb_high,
                              bool is_solid );
    
    inline void update_edge( size_t index,
                            const Vec3d& aabb_low,
                            const Vec3d& aabb_high,
                            bool is_solid );
    
    inline void update_triangle( size_t index,
                                const Vec3d& aabb_low,
                                const Vec3d& aabb_high,
                                bool is_solid );
    
    inline void remove_vertex( size_t index );
    inline void remove_edge( size_t index );
    inline void remove_triangle( size_t index ); 
    
    virtual void get_vertex_aabb( size_t index, bool is_solid, Vec3d& aabb_low, Vec3d& aabb_high );
    virtual void get_edge_aabb( size_t index, bool is_solid, Vec3d& aabb_low, Vec3d& aabb_high );
    virtual void get_triangle_aabb( size_t index, bool is_solid, Vec3d& aabb_low, Vec3d& aabb_high );
    
    /// Get the set of vertices whose bounding volumes overlap the specified bounding volume
    ///
    inline void get_potential_vertex_collisions( const Vec3d& aabb_low, 
                                                const Vec3d& aabb_high,
                                                bool return_solid,
                                                bool return_dynamic,
                                                std::vector<size_t>& overlapping_vertices );
    
    /// Get the set of edges whose bounding volumes overlap the specified bounding volume
    ///
    inline void get_potential_edge_collisions( const Vec3d& aabb_low, 
                                              const Vec3d& aabb_high, 
                                              bool return_solid,
                                              bool return_dynamic,
                                              std::vector<size_t>& overlapping_edges );
    
    /// Get the set of triangles whose bounding volumes overlap the specified bounding volume
    ///
    inline void get_potential_triangle_collisions( const Vec3d& aabb_low, 
                                                  const Vec3d& aabb_high,
                                                  bool return_solid,
                                                  bool return_dynamic,
                                                  std::vector<size_t>& overlapping_triangles );
    
    /// Rebuild one of the grids
    ///
    void build_acceleration_grid( AccelerationGrid& grid, 
                                 std::vector<Vec3d>& xmins, 
                                 std::vector<Vec3d>& xmaxs,
                                 std::vector<size_t>& indices,
                                 double length_scale, 
                                 double grid_padding );
    
    /// Regular grids
    ///
    AccelerationGrid m_solid_vertex_grid;
    AccelerationGrid m_solid_edge_grid;
    AccelerationGrid m_solid_triangle_grid;
    
    AccelerationGrid m_dynamic_vertex_grid;
    AccelerationGrid m_dynamic_edge_grid;
    AccelerationGrid m_dynamic_triangle_grid;
    
};

// ---------------------------------------------------------
//  Inline functions
// ---------------------------------------------------------

// --------------------------------------------------------
///
/// Add a vertex to the broad phase
///
// --------------------------------------------------------

inline void BroadPhaseGrid::add_vertex( size_t index, const Vec3d& aabb_low, const Vec3d& aabb_high, bool is_solid )
{
    if ( is_solid )
    {
        m_solid_vertex_grid.add_element( index, aabb_low, aabb_high );
    }
    else
    {
        m_dynamic_vertex_grid.add_element( index, aabb_low, aabb_high );
    }
}

// --------------------------------------------------------
///
/// Add an edge to the broad phase
///
// --------------------------------------------------------

inline void BroadPhaseGrid::add_edge( size_t index, const Vec3d& aabb_low, const Vec3d& aabb_high, bool is_solid )
{
    if ( is_solid )
    {
        m_solid_edge_grid.add_element( index, aabb_low, aabb_high );
    }
    else
    {
        m_dynamic_edge_grid.add_element( index, aabb_low, aabb_high );
    }
}

// --------------------------------------------------------
///
/// Add a triangle to the broad phase
///
// --------------------------------------------------------

inline void BroadPhaseGrid::add_triangle( size_t index, const Vec3d& aabb_low, const Vec3d& aabb_high, bool is_solid )
{
    if ( is_solid )
    {
        m_solid_triangle_grid.add_element( index, aabb_low, aabb_high );
    }
    else
    {
        m_dynamic_triangle_grid.add_element( index, aabb_low, aabb_high );
    }
}


inline void BroadPhaseGrid::update_vertex( size_t index, const Vec3d& aabb_low, const Vec3d& aabb_high, bool is_solid )
{
    if ( is_solid )
    {
        m_solid_vertex_grid.update_element( index, aabb_low, aabb_high );
    }
    else
    {
        m_dynamic_vertex_grid.update_element( index, aabb_low, aabb_high );
    }
}

inline void BroadPhaseGrid::update_edge( size_t index, const Vec3d& aabb_low, const Vec3d& aabb_high, bool is_solid )
{
    if ( is_solid )
    {
        m_solid_edge_grid.update_element( index, aabb_low, aabb_high );
    }
    else
    {
        m_dynamic_edge_grid.update_element( index, aabb_low, aabb_high );
    }
}

inline void BroadPhaseGrid::update_triangle( size_t index, const Vec3d& aabb_low, const Vec3d& aabb_high, bool is_solid )
{
    if ( is_solid )
    {
        m_solid_triangle_grid.update_element( index, aabb_low, aabb_high );
    }
    else
    {
        m_dynamic_triangle_grid.update_element( index, aabb_low, aabb_high );
    }
}


// --------------------------------------------------------
///
/// Remove a vertex from the broad phase
///
// --------------------------------------------------------

inline void BroadPhaseGrid::remove_vertex( size_t index )
{
    m_solid_vertex_grid.remove_element( index );
    m_dynamic_vertex_grid.remove_element( index );
}

// --------------------------------------------------------
///
/// Remove an edge from the broad phase
///
// --------------------------------------------------------

inline void BroadPhaseGrid::remove_edge( size_t index )
{
    m_solid_edge_grid.remove_element( index );
    m_dynamic_edge_grid.remove_element( index );
}

// --------------------------------------------------------
///
/// Remove a triangle from the broad phase
///
// --------------------------------------------------------

inline void BroadPhaseGrid::remove_triangle( size_t index )
{
    m_solid_triangle_grid.remove_element( index );
    m_dynamic_triangle_grid.remove_element( index );
}

// --------------------------------------------------------
///
/// Query the broad phase to get the set of all vertices overlapping the given AABB
///
// --------------------------------------------------------

inline void BroadPhaseGrid::get_potential_vertex_collisions( const Vec3d& aabb_low,
                                                            const Vec3d& aabb_high,
                                                            bool return_solid,
                                                            bool return_dynamic,
                                                            std::vector<size_t>& overlapping_vertices )
{
    if ( return_solid )
    {
        m_solid_vertex_grid.find_overlapping_elements( aabb_low, aabb_high, overlapping_vertices );
    }
    
    if ( return_dynamic )
    {
        m_dynamic_vertex_grid.find_overlapping_elements( aabb_low, aabb_high, overlapping_vertices );
    }
}

// --------------------------------------------------------
///
/// Query the broad phase to get the set of all edges overlapping the given AABB
///
// --------------------------------------------------------

inline void BroadPhaseGrid::get_potential_edge_collisions( const Vec3d& aabb_low,
                                                          const Vec3d& aabb_high,
                                                          bool return_solid,
                                                          bool return_dynamic,
                                                          std::vector<size_t>& overlapping_edges )
{
    if ( return_solid )
    {
        m_solid_edge_grid.find_overlapping_elements( aabb_low, aabb_high, overlapping_edges );
    }
    
    if ( return_dynamic )
    {
        m_dynamic_edge_grid.find_overlapping_elements( aabb_low, aabb_high, overlapping_edges );
    }
}

// --------------------------------------------------------
///
/// Query the broad phase to get the set of all triangles overlapping the given AABB
///
// --------------------------------------------------------

inline void BroadPhaseGrid::get_potential_triangle_collisions( const Vec3d& aabb_low,
                                                              const Vec3d& aabb_high,
                                                              bool return_solid,
                                                              bool return_dynamic,
                                                              std::vector<size_t>& overlapping_triangles )
{
    if ( return_solid )
    {
        m_solid_triangle_grid.find_overlapping_elements( aabb_low, aabb_high, overlapping_triangles );
    }
    
    if ( return_dynamic )
    {
        m_dynamic_triangle_grid.find_overlapping_elements( aabb_low, aabb_high, overlapping_triangles );
    }
}


// --------------------------------------------------------

inline void BroadPhaseGrid::get_vertex_aabb( size_t index, bool is_solid, Vec3d& aabb_low, Vec3d& aabb_high )
{
    if ( is_solid )
    {
        aabb_low = m_solid_vertex_grid.m_elementxmins[index];
        aabb_high = m_solid_vertex_grid.m_elementxmaxs[index];
    }
    else
    {
        aabb_low = m_dynamic_vertex_grid.m_elementxmins[index];
        aabb_high = m_dynamic_vertex_grid.m_elementxmaxs[index];      
    }
}

// --------------------------------------------------------

inline void BroadPhaseGrid::get_edge_aabb( size_t index, bool is_solid, Vec3d& aabb_low, Vec3d& aabb_high )
{
    if ( is_solid )
    {
        aabb_low = m_solid_edge_grid.m_elementxmins[index];
        aabb_high = m_solid_edge_grid.m_elementxmaxs[index];
    }
    else
    {
        aabb_low = m_dynamic_edge_grid.m_elementxmins[index];
        aabb_high = m_dynamic_edge_grid.m_elementxmaxs[index];      
    }
}

// --------------------------------------------------------

inline void BroadPhaseGrid::get_triangle_aabb( size_t index, bool is_solid, Vec3d& aabb_low, Vec3d& aabb_high )
{
    if ( is_solid )
    {
        aabb_low = m_solid_triangle_grid.m_elementxmins[index];
        aabb_high = m_solid_triangle_grid.m_elementxmaxs[index];
    }
    else
    {
        aabb_low = m_dynamic_triangle_grid.m_elementxmins[index];
        aabb_high = m_dynamic_triangle_grid.m_elementxmaxs[index];      
    }   
}


#endif



