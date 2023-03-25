#pragma once
#include <cstdint>
#include <limits>
#include <optional>
#include <string>
#include <string_view>

namespace gcl {
    enum class FileOpenErrors {
        bad_magic,
    };

    enum class DataInfoTypes : uint8_t {
        UTF_8 = 0x01,
        const_int = 0x02,
        const_float = 0x03,
        const_long = 0x04,
        const_double = 0x05
    };

#pragma pack(push, 1)
    struct FileHeader {
        FileHeader() = delete;
        uint32_t magic;
        uint16_t major_ver;
        uint16_t minor_ver;
    };

    constexpr auto file_header_size = sizeof(FileHeader);

    // Data types
    struct DataInfo {  // This is the only DataInfo that can be constructed. The rest must be casted to.
        uint8_t type;
        uint8_t content[];
    };

    struct DataInfoUtf8 {
        DataInfoUtf8() = delete;
        uint8_t type;
        uint16_t length;
        uint8_t data[];
    };

    struct DataInfoConstInt {
        DataInfoConstInt() = delete;
        uint8_t type;
        uint32_t data;
    };

    struct DataInfoConstFloat {
        DataInfoConstFloat() = delete;
        // Check if float is 4 bytes
        static_assert(std::numeric_limits<float>::is_iec559, "Target does not have iec559 float support");
        static_assert(sizeof(float) == 4, "Target float is not 32 bits");
        uint8_t type;
        float data;
    };

    struct DataInfoConstLong {
        DataInfoConstLong() = delete;
        uint8_t type;
        uint64_t data;
    };

    struct DataInfoConstDouble {
        DataInfoConstDouble() = delete;
        static_assert(sizeof(double) == 8, "Size of double is not 64 bits");
        uint8_t type;
        double data;
    };

    struct FunctionInfo {
        FunctionInfo() = delete;
        uint16_t name_index;
        uint32_t code_length;
        uint8_t code[];
    };
#pragma pack(pop)

    [[nodiscard]] auto mapFile(std::string_view fileName) -> void*;
    [[nodiscard]] auto unmapFile(void* mappedAt) -> bool;

    // TODO: Maybe move this into the VM
    [[nodiscard]] auto ValidateMappedFile(void* mappedAt) -> std::optional<FileOpenErrors>;
}  // namespace gcl