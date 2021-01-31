#ifndef BC_INTERFACE_NODE_H__
#define BC_INTERFACE_NODE_H__
#include <string>

namespace bc {
class INode {
public:
  template <typename String,
            std::enable_if_t<std::is_convertible<String, std::string>::value,
                             int> = 0>
  INode(String &&name) : name_(std::forward<String>(name)){};
  virtual ~INode(){};
  virtual bool IsBoundaryCondition() const { return false; }
  const std::string &GetName() const { return name_; }

protected:
  std::string name_;
};
} // namespace bc
#endif
