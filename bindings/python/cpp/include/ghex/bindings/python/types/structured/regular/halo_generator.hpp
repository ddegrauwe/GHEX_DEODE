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
#ifndef INCLUDED_GHEX_PYBIND_HALO_GENERATOR_HPP
#define INCLUDED_GHEX_PYBIND_HALO_GENERATOR_HPP

#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "boost/mp11/bind.hpp"

#include "gridtools/meta.hpp"

#include "ghex/bindings/python/type_list.hpp"
#include "ghex/bindings/python/utils/demangle.hpp"

namespace py = pybind11;

namespace gridtools {
namespace ghex {
namespace bindings {
namespace python {
namespace types {
namespace structured {
namespace regular {

void export_halo_generator (py::module_& m) {
    using type_list = gridtools::ghex::bindings::python::type_list;
    using derived_type_list = gridtools::ghex::bindings::python::derived_type_list<
        boost::mp11::_1, boost::mp11::_2, boost::mp11::_3>;

    using dim_type = typename type_list::dim_type;
    using domain_id_type = typename type_list::domain_id_type;

    static_assert(std::is_same<domain_id_type, int>::value, "Not implemented. Only integer domain types allowed for now.");

    using halo_generator_type = typename derived_type_list::halo_generator_type;
    auto halo_generator_name = gridtools::ghex::bindings::python::utils::demangle<
        halo_generator_type>();

    using dim_array_t = std::array<int, dim_type::value>;
    using halo_array_t = std::array<int, 2 * dim_type::value>;
    using periodic_array_t = std::array<bool, dim_type::value>;

    // halo_generator
    py::class_<halo_generator_type>(m, halo_generator_name.c_str())
        .def_property_readonly_static("__cpp_type__", [halo_generator_name] (const pybind11::object&) {
            return halo_generator_name;
        })
        .def(py::init<dim_array_t, dim_array_t, halo_array_t, periodic_array_t>())
        .def("__call__", &halo_generator_type::operator()); // todo

    // box2
    py::class_<typename halo_generator_type::box2>(m, "Box2")
        .def_property_readonly("local", py::overload_cast<>(&halo_generator_type::box2::local, py::const_))
        .def_property_readonly("global_", py::overload_cast<>(&halo_generator_type::box2::global, py::const_));
    py::class_<typename halo_generator_type::box>(m, "Box")
        .def_property_readonly("first", [] (const typename halo_generator_type::box& b) {
            auto first = b.first();
            return static_cast<typename decltype(first)::array_type>(first);
        })
        .def_property_readonly("last", [] (const typename halo_generator_type::box& b) {
            auto last = b.last();
            return static_cast<typename decltype(last)::array_type>(last);
        });
}

}
}
}
}
}
}
}
#endif