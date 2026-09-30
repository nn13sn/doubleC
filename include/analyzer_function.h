#include <slot_table.h>
struct Analyzer_function {
  Analyzer_function() {};
  Analyzer_function(const uint32_t &id, const uint32_t &params,
                    const uint32_t &mods)
      : id(id), params(params), mods(mods) {};
  uint32_t id;
  uint32_t params;
  uint32_t mods;
};
