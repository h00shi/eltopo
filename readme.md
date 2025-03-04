
# El Topo: Robust Topological Operations for Dynamic Explicit Surfaces

El Topo is a public domain C++ package for tracking dynamic surfaces represented as triangle meshes in 3D. It robustly handles topology changes such as merging and pinching off, while adaptively maintaining a tangle-free, high-quality triangulation.

The current release contains source for the El Topo library, as well as Talpa, an executable demonstrating several applications of our method. The code has been tested on OS X and Linux and is freely available for download. Please email me with questions and suggestions, and especially if you find it useful in your work.

This distribution contains the following directories:

* eltopo3d: the El Topo library
* talpa: an example executable using El Topo
* common: source code shared by our research group used by El Topo and Talpa

For information on building El Topo or Talpa, see the readme files in their
respective directories.  I highly recommend seeing at least the El Topo readme before getting started.

## The paper:

Tyson Brochu and Robert Bridson: [Robust Topological Operations for Dynamic Explicit Surfaces.](http://www.cs.ubc.ca/labs/imager/tr/2009/eltopo/sisc2009.pdf) SIAM Journal on Scientific Computing, vol. 31, no. 4 (2009), pp. 2472-2493. 

## Projects using or derived from El Topo:

[Matching Fluid Simulation Elements to Surface Geometry and Topology](http://www.cs.ubc.ca/labs/imager/tr/2010/MatchingSimulationToSurface/BBB2010.html)

[Linear-Time Smoke Animation with Vortex Sheet Meshes](http://www.cs.ubc.ca/~tbrochu/projects/bkb2012.pdf)

[Multimaterial Mesh-Based Surface Tracking](http://www.cs.columbia.edu/cg/multitracker/) ([Source code](https://github.com/fangda-cu/MultiTracker))

[Detailed Water with Coarse Grids Combining Surface Meshes and Adaptive Discontinuous Galerkin](http://www.cs.ubc.ca/~essex/dgwater/)

[Ocean Waves Animation using Boundary Integral Equations and Explicit Mesh Tracking](http://tolk.ca/ocean-waves/)

The [untangle](http://www.alecjacobson.com/weblog/?p=4341) function in [gptoolbox - Geometry Processing Toolbox (MATLAB)](https://github.com/alecjacobson/gptoolbox)



## Patches borrowed from


[Orbots@a731f28](https://github.com/Orbots/eltopo/commit/a731f2883f06a31e6c62b28a5bc1bb2120bb6f3b)
[flgw@49702eb](https://github.com/flgw/eltopo/commit/49702eb33f7b1d52e669947ffe3e88438858cdd4)
[leokollersacht@dd419de](https://github.com/leokollersacht/eltopo/commit/dd419defe9b2533f01c80b712e581aef17952716)
various commits by [alecjacobson](https://github.com/alecjacobson/eltopo)

