#pragma once

#include <functional>
#include <unordered_map>

#include "opcodes.hpp"

using opcodeHandler = std::function<void(void)>;
extern const std::unordered_map<gcl::Opcodes, opcodeHandler> instructionFunctions;
