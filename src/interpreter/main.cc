#include <fmt/format.h>

#include "VM/vm.hpp"

auto main(int argc, char** argv) -> int {
    fmt::print("Test print\n");
    fmt::print(stderr, "Test error\n");

    gcl::VM vm(2048);
    vm.Load("lmao.gcl");

    asm volatile("nop");
    vm.Exec();
    return 0;
}