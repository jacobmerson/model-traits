#include "bcModelTraits.h"

namespace bc {
// ModelTraits::ModelTraits(const std::string & modelName, MT* model) {
//
//}
ModelTraits::ModelTraits(const std::string &name) : name_(name){};
INode *ModelTraits::AddCase(const std::string &case_name) {
  const auto it = cases_.find(case_name);
  if (it != cases_.end()) {
    return it->second.get();
  } else {
    auto res = cases_.insert(
        it, {case_name, std::make_unique<CategoryNode>(case_name)});
    return res->second.get();
  }
}
bool ModelTraits::RemoveCase(const std::string &name) {
  const auto it = cases_.find(name);
  // we can't remove an item that doesn't exist in the map
  if (it == cases_.end()) {
    return false;
  }
  cases_.erase(it);
  return true;
}
} // namespace bc
