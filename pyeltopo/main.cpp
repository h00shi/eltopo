#include <memory>
#include <algorithm>
#include <iterator>
#include <mtao/types.h>
#include <fstream>
#include "eltopo.h"
#include <iostream>

#include <eltopo3d/eltopo.h>

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>

namespace py = pybind11;


PYBIND11_MODULE(pyeltopo, m) {
    py::class_<ElTopoTracker>(m, "ElTopoTracker")
        .def(py::init<const ElTopoTracker::CRefCV3d&, const ElTopoTracker::CRefCV3i&>())
        //.def(py::init<const ElTopoTracker::CRefCV3d&, const ElTopoTracker::CRefCV3i&,bool,bool>())
        .def("get_triangles",&ElTopoTracker::get_triangles)
        .def("get_vertices",&ElTopoTracker::get_vertices)
        .def("integrate",&ElTopoTracker::integrate_py)
        .def("improve",&ElTopoTracker::improve)
        .def("step",&ElTopoTracker::step_py)
        .def("defrag_mesh",&ElTopoTracker::defrag_mesh)
        .def("split_edge",&ElTopoTracker::split_edge)
        .def("split_triangle",&ElTopoTracker::split_triangle);
}


