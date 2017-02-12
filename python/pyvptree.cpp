#include "vptree.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

template<typename NumType>
std::vector<std::vector<double>> makeItems(NumType* buffer, const std::vector<size_t>& shape) {
    auto itemCount = shape[0];
    auto itemSize = shape[1];

    std::vector<std::vector<double>> items(itemCount);
    for (int i = 0; i < itemCount; ++i) {
        items[i] = std::vector<double>(buffer + i * itemSize, buffer + (i + 1) * itemSize);
    }
    return items;
}

PYBIND11_PLUGIN(vptree) {
    py::module m("vptree");

    py::class_<vpt::VpTree>(m, "VpTree")
        .def(py::init<const std::vector<std::vector<double>>&>())
        .def("__init__", [](vpt::VpTree &tree, py::buffer b) {
            /* Request a buffer descriptor from Python */
            py::buffer_info info = b.request();

            if (info.ndim != 2)
                throw std::runtime_error("Expected 2-dimensional array.");

            if (info.format == py::format_descriptor<double>::format()) {
                new (&tree) vpt::VpTree(makeItems(static_cast<double*>(info.ptr), info.shape));
            } else if (info.format == py::format_descriptor<float>::format()) {
                new (&tree) vpt::VpTree(makeItems(static_cast<float*>(info.ptr), info.shape));
            } else {
                throw std::runtime_error("Incompatible format: expected a double or float array!");
            }
        })
        .def("getNearestNeighbors", &vpt::VpTree::getNearestNeighbors<vpt::Vector>)
        .def("getNearestNeighborsBatch", &vpt::VpTree::getNearestNeighborsBatch<std::vector<vpt::Vector>>);


#ifdef VERSION_INFO
    m.attr("__version__") = py::str(VERSION_INFO);
#else
    m.attr("__version__") = py::str("dev");
#endif

    return m.ptr();
}
