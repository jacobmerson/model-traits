#ifndef BC_SIM_WRAPPER_H__
#define BC_SIM_WRAPPER_H__

#include <SimAttDef.h>
#include <SimAttribute.h>
#include <SimError.h>
#include <SimErrorCodes.h>
#include <SimUtil.h>

#include "SimParasolidKrnl.h"

#include <fmt/format.h>
#include <iterator>
#include <type_traits>

namespace mt {
template <typename T> class SimList {
  static_assert(std::is_pointer<T>::value, "Sim list must be pointer type");

public:
  SimList(pPList list) : list_(list) {}
  void Clear() { PList_clear(list_); }

  template <typename T2> bool Contains(T2 *item) {
    return PList_contains(list_, static_cast<void *>(item));
  }

  T operator[](int n) { return static_cast<T>(PList_item(list_, n)); }
  int Size() noexcept { return PList_size(list_); }
  ~SimList() noexcept { PList_delete(list_); }

  class iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    static_assert(std::is_pointer<value_type>::value,
                  "Sim list must be pointer type");
    // default constructible
    iterator() : list_(nullptr), ent_(nullptr) {}
    iterator(pPList list, value_type ent) : list_(list), ent_(ent) {
      // since the ent for begin will be anything that's not nullptr
      // need to advance the iter_, and set the entity
      // the iterator to the next element
      if (ent != nullptr) {
        ent_ = static_cast<value_type>(PList_next(list_, &iter_));
      }
    }

    reference operator*() { return ent_; }
    // pointer operator->() { return &ent_; }

    // prefix increment
    iterator &operator++() {
      ent_ = static_cast<value_type>(PList_next(list_, &iter_));
      return *this;
    }
    // postfix operator
    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    friend bool operator==(const iterator &a, const iterator &b) {
      return a.ent_ == b.ent_;
    }
    friend bool operator!=(const iterator &a, const iterator &b) {
      return a.ent_ != b.ent_;
    }

  private:
    void *iter_{nullptr};
    pPList list_;
    value_type ent_;
  };

  iterator begin() { return iterator(list_, operator[](0)); }
  iterator end() { return iterator(list_, nullptr); }

private:
  pPList list_;
};
/*
 * A thin wrapper class around a simmetrix char* string. This is intended to
 * take ownership over the char* data that is returned from many of the
 * Simmetrix API calls. It should only be used with char* returned from
 * Simmetrix API and not a generic c++ char*. Otherwise the destruction will
 * cause issues.
 */
class SimString {
public:
  // construction should only come from
  // a simmetrix function
  SimString(char *string) noexcept : data_(string) {}
  SimString() noexcept : data_(nullptr) {}
  ~SimString() noexcept { Sim_deleteString(data_); }
  // disable copy because we don't have access to the allocation functions that
  // Simmetrix uses. Therefore, we cannot create a new SimString that will be
  // properly deleted with Sim_deleteString
  SimString &operator=(const SimString &) = delete;
  SimString(const SimString &) = delete;
  SimString(SimString &&other) noexcept : SimString() { swap(*this, other); }
  SimString &operator=(SimString &&other) {
    swap(*this, other);
    return *this;
  }
  operator const char *() const { return data_; }
  friend void swap(SimString &first, SimString &second) {
    // two-step for ADL lookup
    using std::swap;
    swap(first.data_, second.data_);
  }

private:
  char *data_;
};

/*
 * RAII class for simmodeler startup
 */
class SimmodelerStartup {
  using MessageHandlerFP = void (*)(int, const char *);

public:
  SimmodelerStartup() noexcept {
    SimModel_start();
    // set the default message handler
    SetMessageHandler([](int type, const char *msg) {
      switch (type) {
      case Sim_InfoMsg:
        fmt::print("Info: {}\n", msg);
        break;
      case Sim_DebugMsg:
        fmt::print("Debug: {}\n", msg);
        break;
      case Sim_WarningMsg:
        fmt::print("Warning: {}\n", msg);
        break;
      case Sim_ErrorMsg:
        fmt::print("Error: {}\n", msg);
        break;
      }
      return;
    });
  }
  ~SimmodelerStartup() noexcept { SimModel_stop(); }
  void SetMessageHandler(MessageHandlerFP message_handler) {
    Sim_setMessageHandler(message_handler);
  };
};

/*
 * RAII class to register the simmetrix licenses
 */
class SimmodelerLicenses {
public:
  // require we pass in the startup object since startup
  // must happen before getting the licenses
  SimmodelerLicenses(const SimmodelerStartup &) noexcept {
    Sim_readLicenseFile(0);
  }
  // passing a temp simmodeler startup object
  // would cause the simmodeler starutp destructor
  // to be called immediately which is not the expected
  // or desired behavior
  SimmodelerLicenses(SimmodelerStartup &&) = delete;
  ~SimmodelerLicenses() noexcept { Sim_unregisterAllKeys(); }
};
/*
 * RAII class for the simmodeler log
 */
class SimmodelerLog {
public:
  SimmodelerLog(const char *log_name) noexcept { Sim_logOn(log_name); }
  ~SimmodelerLog() noexcept { Sim_logOff(); }
};
/*
 * RAII class for simmodeler parasolid
 */
class SimmodelerParasolid {
public:
  SimmodelerParasolid() noexcept { SimParasolid_start(1); }
  ~SimmodelerParasolid() noexcept { SimParasolid_stop(1); }
};
/*
 * RAII class for simmetrix progress
 */
class SimProgress {
public:
  using ProgressFP = void (*)(const char *, int, int, int, int, void *);
  SimProgress() noexcept {
    progress_ = Progress_new();
    SetHandler([](const char *what, int level, int start_val, int end_val,
                  int current_val, void *) {
      fmt::print(
          "Progress: {}, level: {}, startVal: {}, endVal: {}, currentVal {}",
          what, level, start_val, end_val, current_val);
    });
  }
  ~SimProgress() noexcept { Progress_delete(progress_); }
  void SetHandler(ProgressFP progress_handler) const noexcept {
    Progress_setCallback(progress_, progress_handler);
  }
  operator pProgress() const noexcept { return progress_; }

private:
  pProgress progress_;
};

/*
 * Wrapper for the simmetrix model
 * Assumes ownership of the simmetrix model
 */
class SimGModel {
public:
  SimGModel(pGModel model) noexcept : model_(model) {}
  ~SimGModel() noexcept { GM_release(model_); }
  operator pGModel() { return model_; }

private:
  pGModel model_;
};

} // namespace mt
#endif
