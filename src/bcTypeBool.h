#ifndef BC_TYPE_BOOL_H__
#define BC_TYPE_BOOL_H__
namespace bc {
struct BoolBC {
  BoolBC(bool d) noexcept : data(d){};
  bool data;
};
} // namespace bc
#endif
