#pragma once

#include "JITframe.h"
/// jitfunction is a template for all compiled functions
using JITfunction = void (*)(JITFrame *frame_ptr);
