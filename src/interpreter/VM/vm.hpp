#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "file.hpp"
#include "opcodes.hpp"

namespace gcl {
    class VM final {
       public:
        using instruction = const Opcodes;

        VM() = delete;
        VM(const VM&) = delete;
        auto operator=(const VM&) -> VM& = delete;
        VM(const VM&&) = delete;
        auto operator=(const VM&&) -> VM& = delete;

        explicit VM(std::size_t stackSize);

        void Load(std::string_view file);
        void Exec();

        ~VM();

       private:
        static constexpr std::string MainFunctionSignature{"main(Is[])I"};

        struct {
            instruction* ip{nullptr};
            uint8_t* sp{nullptr};
        } registers;

        void* mappedFile{nullptr};
        void* functionVectorLocation{nullptr};
        uint8_t* stack{nullptr};

        std::unordered_map<std::string_view, const FunctionInfo*> functionTable;  // Note: Might need to un-const
                                                                                  // FunctionInfo if we want to
                                                                                  // support bytecode patching
        std::vector<const gcl::DataInfo*> constDataPool;

        void LoadConstData();
        void LoadFunctionData();
        
        template <class T>
        auto ResolveNativeFunction(std::string_view functionSignature) -> std::optional<T>;
    };
}  // namespace gcl