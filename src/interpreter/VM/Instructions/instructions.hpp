#pragma once

#include <functional>
#include <unordered_map>

#include "opcodes.hpp"
#include "VM/vm.hpp"

using opcodeHandler = std::function<void(gcl::VM&)>;
extern const std::unordered_map<gcl::Opcodes, opcodeHandler> instructionFunctions;
