#include <slot_table.h>

/**
 * analyzer function - a function info but for analyzer, only contains the
 * information that is essential for catching the errors
 */
struct Analyzer_function {
  /// default constructor
  Analyzer_function() {};
  /// main constructor
  Analyzer_function(const uint32_t &id, const uint32_t &params,
                    const uint32_t &mods)
      : id(id), params(params), mods(mods) {};
  uint32_t id;
  uint32_t params;
  uint32_t mods;
};
