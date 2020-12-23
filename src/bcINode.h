#ifndef BC_INTERFACE_NODE_H__
#define BC_INTERFACE_NODE_H__
#include <string>

namespace bc {
class INode {
public:
  INode(const std::string &name) : name_(name){};
  virtual ~INode(){};
  virtual bool IsBoundaryCondition() const { return false; }
  const std::string &GetName() const { return name_; }

protected:
  std::string name_;
};
} // namespace bc
#endif
