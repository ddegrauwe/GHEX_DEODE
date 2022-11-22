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
#ifndef INCLUDED_GHEX_PYBIND_COMMUNICATION_OBJECT_HPP
#define INCLUDED_GHEX_PYBIND_COMMUNICATION_OBJECT_HPP

#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "ghex/buffer_info.hpp"
#include "ghex/communication_object_2.hpp"
#include "ghex/pattern.hpp"

#include "ghex/bindings/python/type_list.hpp"
#include "ghex/bindings/python/utils/demangle.hpp"

namespace py = pybind11;

namespace gridtools {
namespace ghex {
namespace bindings {
namespace python {
namespace types {

void communication_object_exporter(py::module_& m) {
    using architecture_type = typename gridtools::ghex::bindings::python::type_list::architecture_type;
    using data_type = typename gridtools::ghex::bindings::python::type_list::data_type;
    using dim_type = typename gridtools::ghex::bindings::python::type_list::dim_type;
    using domain_id_type = typename gridtools::ghex::bindings::python::type_list::domain_id_type;
    using grid_type = typename gridtools::ghex::bindings::python::type_list::grid_type;
    using layout_map_type = typename gridtools::ghex::bindings::python::type_list::layout_map_type;
    using transport_type = typename gridtools::ghex::bindings::python::type_list::transport_type;

    using domain_descriptor_type = gridtools::ghex::structured::regular::domain_descriptor<
        domain_id_type, dim_type>;
    using context_type = typename gridtools::ghex::tl::context_factory<transport_type>::context_type;
    using communicator_type = typename context_type::communicator_type;
    using grid_impl_type = typename grid_type::template type<domain_descriptor_type>;

    // TODO: could be multiple in the future when the halo generator is parameterized
    using domain_range_type = typename gridtools::ghex::bindings::python::type_list::domain_range_type<domain_descriptor_type>;
    using halo_generator_type = gridtools::ghex::structured::regular::halo_generator<domain_id_type, dim_type>;
    using pattern_container_type = decltype(gridtools::ghex::make_pattern<
        gridtools::ghex::bindings::python::type_list::grid_type>(
            std::declval<typename gridtools::ghex::bindings::python::type_list::context_type&>(),
            std::declval<halo_generator_type&>(),
            std::declval<domain_range_type&>()
        )
    );
    using field_descriptor_type = gridtools::ghex::structured::regular::field_descriptor<
        data_type, architecture_type, domain_descriptor_type, layout_map_type>;
    using buffer_info_type = gridtools::ghex::buffer_info<
        typename pattern_container_type::value_type, architecture_type, field_descriptor_type>;

    using communication_object_type = gridtools::ghex::communication_object<
        communicator_type, grid_impl_type, domain_id_type>;
    auto communication_object_name = gridtools::ghex::bindings::python::utils::demangle<communication_object_type>();

    py::class_<communication_object_type>(m, communication_object_name.c_str())
        .def_property_readonly_static("__cpp_type__", [communication_object_name] (const pybind11::object&) {
            return communication_object_name;
        })
        .def(py::init<communicator_type>())
        .def("exchange", [] (communication_object_type& co,
                             buffer_info_type& b) {
            return co.exchange(b);
        })
        .def("exchange", [] (communication_object_type& co,
                             buffer_info_type& b1,
                             buffer_info_type& b2) {
            return co.exchange(b1, b2);
        })
        .def("exchange", [] (communication_object_type& co,
                             buffer_info_type& b1,
                             buffer_info_type& b2,
                             buffer_info_type& b3) {
            return co.exchange(b1, b2, b3);
        });

    using communication_handle_type = typename communication_object_type::handle_type;
    auto communication_handle_name = gridtools::ghex::bindings::python::utils::demangle<communication_handle_type>();

    py::class_<communication_handle_type>(m, communication_handle_name.c_str())
        .def_property_readonly_static("__cpp_type__", [communication_handle_name] (const pybind11::object&) {
            return communication_handle_name;
        })
        .def("wait", &communication_handle_type::wait);
}


}
}
}
}
}
#endif