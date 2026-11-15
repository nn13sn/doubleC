#pragma once
#include "JITfunction.h"
#include "runtime_value.h"
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <thread>
#define NOT_COMPILED 0
#define COMPILING 1
#define COMPILED 2

/**
 * a structure used to identify the state of the compiled function and wait if
 * needed (like in compiler)
 */
struct Compilationsync {
  std::mutex mutex;
  std::condition_variable cv;
};

/// holds all the information about the function
struct FunctionInfo {
  size_t numberofparams;
  uint32_t entry;
  size_t slot;
  size_t exit;
  JITfunction compversion = nullptr;
  int32_t mods;
  std::unique_ptr<Compilationsync> sync = std::make_unique<Compilationsync>();
  std::thread compile_thread;
  bool threadjoined = false;
  std::atomic<uint8_t> compiledstate{NOT_COMPILED};

  /// default constructor
  FunctionInfo() = default;

  /// deleting the copy constructors
  FunctionInfo(const FunctionInfo &) = delete;
  FunctionInfo &operator=(const FunctionInfo &) = delete;

  /**
   * some structure fields cannot be copied or even moved, so there is a custom
   * version of copy operator and copy constructor
   */
  FunctionInfo(FunctionInfo &&other) noexcept
      : numberofparams(other.numberofparams), entry(other.entry),
        slot(other.slot), exit(other.exit), compversion(other.compversion),
        mods(other.mods), sync(std::move(other.sync)),
        compile_thread(std::move(other.compile_thread)),
        compiledstate(other.compiledstate.load()) {}

  FunctionInfo &operator=(FunctionInfo &&other) noexcept {
    if (this == &other)
      return *this;
    this->numberofparams = other.numberofparams;
    this->entry = other.entry;
    this->slot = other.slot;
    this->exit = other.exit;
    this->compversion = other.compversion;
    this->mods = other.mods;
    this->sync = std::move(other.sync);
    this->compile_thread = std::move(other.compile_thread);
    this->compiledstate.store(other.compiledstate.load());
    // the reason I have to do that is because std::atomic cannot be copied or
    // moved through std::move, so I basically loaded the old value of
    // std::atomic and assign it to the new
    return *this;
  }
};
