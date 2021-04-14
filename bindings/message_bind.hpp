#ifndef INCLUDED_GHEX_FORTRAN_MESSAGE_BIND_HPP
#define INCLUDED_GHEX_FORTRAN_MESSAGE_BIND_HPP

#include "obj_wrapper.hpp"

#define ALLOCATOR_STD             1
#define ALLOCATOR_PERSISTENT_STD  2
#define ALLOCATOR_HOST            3
#define ALLOCATOR_PERSISTENT_HOST 4
#define ALLOCATOR_GPU             5
#define ALLOCATOR_PERSISTENT_GPU  6

using std_allocator_type = std::allocator<unsigned char>;
	
#endif /* INCLUDED_GHEX_FORTRAN_MESSAGE_BIND_HPP */
