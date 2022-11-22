/*
 * GridTools
 *
 * Copyright (c) 2014-2021, ETH Zurich
 * All rights reserved.
 *
 * Please, refer to the LICENSE file in the root directory.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef INCLUDED_GHEX_PYBIND_CONTEXT_HPP
#define INCLUDED_GHEX_PYBIND_CONTEXT_HPP

#include <sstream>

#ifdef GHEX_ENABLE_MPI4PY
#include <exception>
#include <mpi4py/mpi4py.h>
#endif

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "ghex/transport_layer/context.hpp"
#include "ghex/transport_layer/mpi/context.hpp"

#include "ghex/bindings/python/type_list.hpp"
#include "ghex/bindings/python/utils/demangle.hpp"

namespace py = pybind11;

namespace gridtools {
namespace ghex {
namespace bindings {
namespace python {
namespace types {
namespace transport_layer {

void context_exporter(py::module_& m) {
    using context_type = typename gridtools::ghex::bindings::python::type_list::context_type;
    auto context_name = gridtools::ghex::bindings::python::utils::demangle<context_type>();

    auto context_cls = py::class_<context_type>(m, context_name.c_str())
        .def_property_readonly_static("__cpp_type__", [context_name] (const pybind11::object&) {
            return context_name;
        })
        .def("rank", &context_type::rank)
        .def("size", &context_type::size)
        .def("get_communicator", &context_type::get_communicator);

#ifdef GHEX_ENABLE_MPI4PY
    context_cls.def(py::init([] (py::object& py_comm_obj) {
        import_mpi4py();
        if (!PyObject_TypeCheck(py_comm_obj.ptr(), &PyMPIComm_Type)) {
            std::stringstream ss;
            ss << "Argument must be `mpi4py.MPI.Comm`, not `" << py_comm_obj.get_type() << "`" << std::endl;
            throw pybind11::type_error(ss.str());
        }
        MPI_Comm mpi_comm = *PyMPIComm_Get(py_comm_obj.ptr());

        return gridtools::ghex::tl::context_factory<typename context_type::tag>::create(mpi_comm);
    }));
#else
    context_cls.def(py::init([] (py::object&) {
        throw std::runtime_error("Context construction reqires bindings to be compiled with mpi4py support.");

        return static_cast<context_type*>(nullptr); // just to make pybind11 happy
    }));
#endif
}

}
}
}
}
}
}
#endif