#ifndef BC_TYPE_STRING_H__
#define BC_TYPE_STRING_H__
#include <string>
namespace bc {
struct StringBC {
  // not noexcept because copying d in the parameter can throw
  StringBC(std::string d) : data(std::move(d)){};
  StringBC(std::string &&d) noexcept : data(std::move(d)){};
  std::string data;
};
} // namespace bc
#endif
