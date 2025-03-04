// ---------------------------------------------------------
//
//  broadphase.h
//  Tyson Brochu 2008
//  
//  Interface for abstract broad phase collision detector class.  The main function of a broad phase is to avoid performing 
//  collision detection between all primitives. Abstract so we can try different strategies, however only BroadPhaseGrid is 
//  currently implemented.
//
// ---------------------------------------------------------

#ifndef EL_TOPO_BROADPHASE_H
#define EL_TOPO_BROADPHASE_H

// ---------------------------------------------------------
// Nested includes
// ---------------------------------------------------------

#include "../common/vec.h"

// ---------------------------------------------------------
//  Forwards and typedefs
// ---------------------------------------------------------

class DynamicSurface;

// ---------------------------------------------------------
//  Class definitions
// ---------------------------------------------------------

// --------------------------------------------------------
///
/// Abstract broad phase collision detector
///
// --------------------------------------------------------

class BroadPhase
{   
public:
    
    virtual ~BroadPhase() 
    {}
    
    /// Rebuild the broad phase
    ///
    virtual void update_broad_phase( const DynamicSurface& surface, bool continuous ) = 0;
    
    
    virtual void add_vertex( size_t index,
                            const Vec3d& aabb_low,
                            const Vec3d& aabb_high,
                            bool is_solid ) = 0;
    
    virtual void add_edge( size_t index,
                          const Vec3d& aabb_low,
                          const Vec3d& aabb_high,
                          bool is_solid ) = 0;
    
    virtual void add_triangle( size_t index,
                              const Vec3d& aabb_low,
                              const Vec3d& aabb_high,
                              bool is_solid ) = 0;
    
    virtual void update_vertex( size_t index,
                               const Vec3d& aabb_low,
                               const Vec3d& aabb_high,
                               bool is_solid ) = 0;
    
    virtual void update_edge( size_t index,
                             const Vec3d& aabb_low,
                             const Vec3d& aabb_high,
                             bool is_solid ) = 0;
    
    virtual void update_triangle( size_t index,
                                 const Vec3d& aabb_low,
                                 const Vec3d& aabb_high,
                                 bool is_solid ) = 0;
    
    virtual void remove_vertex( size_t index ) = 0;
    virtual void remove_edge( size_t index ) = 0;
    virtual void remove_triangle( size_t index ) = 0; 
    
    virtual void get_vertex_aabb( size_t index, bool is_solid, Vec3d& aabb_low, Vec3d& aabb_high ) = 0;
    virtual void get_edge_aabb( size_t index, bool is_solid, Vec3d& aabb_low, Vec3d& aabb_high ) = 0;
    virtual void get_triangle_aabb( size_t index, bool is_solid, Vec3d& aabb_low, Vec3d& aabb_high ) = 0;
    
    /// Get the set of vertices whose bounding volumes overlap the specified bounding volume
    ///
    virtual void get_potential_vertex_collisions( const Vec3d& aabb_low, 
                                                 const Vec3d& aabb_high,
                                                 bool return_solid,
                                                 bool return_dynamic,
                                                 std::vector<size_t>& overlapping_vertices ) = 0;
    
    /// Get the set of edges whose bounding volumes overlap the specified bounding volume
    ///
    virtual void get_potential_edge_collisions( const Vec3d& aabb_low, 
                                               const Vec3d& aabb_high, 
                                               bool return_solid,
                                               bool return_dynamic,
                                               std::vector<size_t>& overlapping_edges ) = 0;
    
    /// Get the set of triangles whose bounding volumes overlap the specified bounding volume
    ///
    virtual void get_potential_triangle_collisions( const Vec3d& aabb_low, 
                                                   const Vec3d& aabb_high,
                                                   bool return_solid,
                                                   bool return_dynamic,
                                                   std::vector<size_t>& overlapping_triangles ) = 0;
    
};


#endif

