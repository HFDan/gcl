#include "VM/Instructions/instructions.hpp"

#include <string_view>

#include "endian.hpp"

extern const std::unordered_map<gcl::Opcodes, opcodeHandler> instructionFunctions = {
    {gcl::Opcodes::NCALL,
     [](gcl::VM& interpreter) {
         auto& regs = interpreter.registers;

         auto funcnameidx = BigEndianToHostEndian(*reinterpret_cast<const uint16_t*>(regs.ip));
         const auto* funcnamestruct =
             reinterpret_cast<const gcl::DataInfoUtf8*>(interpreter.constDataPool[funcnameidx]);
         auto funcname =
             std::string_view(reinterpret_cast<const char*>(&(funcnamestruct->data[0])), BigEndianToHostEndian(funcnamestruct->length));

         auto res = interpreter.ResolveNativeFunction(funcname);
         if (res.has_value()) {
             reinterpret_cast<void(*)(const char*)>(res.value())("aaaa");
         }
     }},
    {gcl::Opcodes::PUSHS, [](gcl::VM& interpreter) {
        auto& regs = interpreter.registers;
        
        regs.sp += sizeof(uint16_t);
        
        const auto* valToPush = reinterpret_cast<const uint16_t*>(regs.ip); 

        regs.ip += sizeof(uint16_t);
        *regs.sp = BigEndianToHostEndian(*valToPush);
     }}};
