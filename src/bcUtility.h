#ifndef BC_UTILITY_H__
#define BC_UTILITY_H__
#include <unordered_set>
#include <vector>

namespace mt {
/*
 * This is here so that we can use the template, template parameters
 * with the defaults for vector and unordered map. This workaround
 * is not needed in c++17 and up.
 */
template <typename T> using BC_VEC_WORKAROUND = std::vector<T>;
template <typename T> using BC_USET_WORKAROUND = std::unordered_set<T>;
} // namespace mt

#endif
