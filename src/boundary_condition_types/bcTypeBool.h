#ifndef BC_TYPE_BOOL_H__
#define BC_TYPE_BOOL_H__
namespace bc {
struct BoolBC {
  BoolBC(bool d) noexcept : data(d){};
  operator bool() const noexcept { return data; }
  operator bool &() noexcept { return data; }
  bool data;
};
} // namespace bc
#endif
