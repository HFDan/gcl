#include "VM/vm.hpp"

#include <fmt/format.h>

#include <type_traits>

#include "VM/Instructions/instructions.hpp"
#include "endian.hpp"

#ifdef __linux__
extern "C" {
#include <dlfcn.h>
}
#endif

namespace gcl {
    VM::VM(std::size_t stackSize) : stack(new uint8_t[stackSize]) { registers.sp = stack; }

    VM::~VM() { delete[] stack; }

    void VM::LoadConstData() {
        if (mappedFile == nullptr) {
            return;  // TODO: Throw
        }

        const uint16_t data_length =
            BigEndianToHostEndian(*(reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(mappedFile) + file_header_size)));
        if (data_length == 0) {
            return;  // TODO: Throw
        }

        constDataPool.resize(data_length);

        auto* offset =
            reinterpret_cast<void*>(static_cast<uint8_t*>(mappedFile) + file_header_size + sizeof(data_length));
        for (uint16_t i = 0; i < data_length; i++) {
            const auto* data = reinterpret_cast<const DataInfo*>(offset);

            switch (static_cast<DataInfoTypes>(BigEndianToHostEndian(data->type))) {
                using enum gcl::DataInfoTypes;
                case UTF_8: {
                    const auto* utf8data = reinterpret_cast<const DataInfoUtf8*>(data);
                    constDataPool[i] = data;

                    offset = reinterpret_cast<uint8_t*>(offset) + sizeof(DataInfoUtf8) +
                             BigEndianToHostEndian(utf8data->length);
                    break;
                }

                case const_int: {
                    constDataPool[i] = data;

                    offset = reinterpret_cast<uint8_t*>(offset) + sizeof(DataInfoConstInt);
                    break;
                }
                case const_float: {
                    constDataPool[i] = data;

                    offset = reinterpret_cast<uint8_t*>(offset) + sizeof(DataInfoConstFloat);
                    break;
                }

                case const_long: {
                    constDataPool[i] = data;

                    offset = reinterpret_cast<uint8_t*>(offset) + sizeof(DataInfoConstLong);
                    break;
                }
                case const_double: {
                    constDataPool[i] = data;

                    offset = reinterpret_cast<uint8_t*>(offset) + sizeof(DataInfoConstDouble);
                    break;
                }
            }
        }

        functionVectorLocation = offset;
    }

    void VM::LoadFunctionData() {
        const uint16_t function_count = BigEndianToHostEndian(*(reinterpret_cast<uint16_t*>(functionVectorLocation)));

        const auto* func = reinterpret_cast<const FunctionInfo*>(
            reinterpret_cast<uint8_t*>(functionVectorLocation) + sizeof(uint16_t));
        for (uint16_t i = 0; i < function_count; i++) {
            const auto funcnameidx = BigEndianToHostEndian(func->name_index);
            const auto* funcnameentry = reinterpret_cast<const DataInfoUtf8*>(constDataPool[funcnameidx]);

            std::string_view name(
                reinterpret_cast<const char*>(&(funcnameentry->data[0])), BigEndianToHostEndian(funcnameentry->length));

            if (functionTable.contains(name)) {
                return;  // TODO: Throw
            }

            functionTable[name] = func;

            func += sizeof(FunctionInfo) + BigEndianToHostEndian(func->code_length);
        }
    }

    void VM::Load(std::string_view file) {
        mappedFile = mapFile(file);

        if (mappedFile == nullptr) {
            return;  // TODO: Throw
        }

        // Validate header
        if (ValidateMappedFile(mappedFile).has_value()) {
            return;  // TODO: Throw
        }

        LoadConstData();
        LoadFunctionData();
    }

    void VM::Exec() {
        if (!functionTable.contains(MainFunctionSignature)) {
            fmt::print(stderr, "Unable to find main function with signature {}\n", MainFunctionSignature);
            return;  // TODO: Throw
        }

        const auto* mainFunctionEntry = functionTable.at(MainFunctionSignature);

        registers.ip = reinterpret_cast<instruction*>(&(mainFunctionEntry->code[0]));

        {
            const auto opcode = *registers.ip;
            registers.ip += sizeof(opcode);

            instructionFunctions.at(opcode)(*this);
        }
    }

#ifdef __linux__
    auto VM::ResolveNativeFunction(std::string_view functionSignature) -> std::optional<void*> {
        std::string funcname;
        for (auto it : functionSignature) {
            funcname += it;
        }

        const auto* const resolved = dlsym(RTLD_DEFAULT, funcname.c_str());

        if (resolved == nullptr) {
            return std::nullopt;
        }

        return const_cast<void*>(resolved);
    }
#endif
}  // namespace gcl