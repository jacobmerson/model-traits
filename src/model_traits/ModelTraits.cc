#include "model_traits/ModelTraits.h"
#include <algorithm>
#include <fmt/format.h>

namespace mt {

using std::find_if;
using std::remove_if;

ModelTraits::ModelTraits(const std::string &name) : name_(name) {}
ModelTraits::ModelTraits(std::string &&name) : name_(std::move(name)) {}
CategoryNode *ModelTraits::AddCase(const std::string &name) {
  auto *nd = FindCase(name);
  if (nd == nullptr) {
    cases_.emplace_back(name);
    return &cases_.back();
  }
  throw std::runtime_error(fmt::format(
      "Cannot add multiple cases with the same name ({})", nd->GetName()));
}
void ModelTraits::RemoveCase(const std::string &name) {
  cases_.erase(remove_if(begin(cases_), end(cases_),
                         [&name](const CategoryNode &nd) {
                           return nd.GetName() == name;
                         }),
               end(cases_));
}
const std::vector<CategoryNode> &ModelTraits::GetCases() const {
  return cases_;
}
int ModelTraits::NumCases() { return cases_.size(); }
const std::string &ModelTraits::GetName() const noexcept { return name_; }

const CategoryNode *
ModelTraits::FindCase(const std::string &name) const noexcept {
  auto it =
      find_if(begin(cases_), end(cases_),
              [&name](const CategoryNode &nd) { return nd.GetName() == name; });
  if (it == end(cases_)) {
    return nullptr;
  }
  return &(*it);
}
CategoryNode *ModelTraits::FindCase(const std::string &name) noexcept {
  return const_cast<CategoryNode *>(
      static_cast<const ModelTraits &>(*this).FindCase(name));
}
} // namespace mt