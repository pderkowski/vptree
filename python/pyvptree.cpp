#include "vptree.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_PLUGIN(vptree) {
    py::module m("vptree");

    py::class_<vpt::VpTree>(m, "VpTree")
        .def(py::init<const std::vector<std::vector<double>>&>())
        .def("getNearestNeighbors", &vpt::VpTree::getNearestNeighbors<vpt::VpTree::Vector>);


#ifdef VERSION_INFO
    m.attr("__version__") = py::str(VERSION_INFO);
#else
    m.attr("__version__") = py::str("dev");
#endif

    return m.ptr();
}
