#include <fmt/format.h>

#include "VM/vm.hpp"

auto main(int argc, char** argv) -> int {
    gcl::VM vm(2040);
    vm.Load("lmao.gcl");

    asm volatile("nop");
    vm.Exec();
    return 0;
}