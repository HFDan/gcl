#include "file.hpp"

#include <string_view>
#include <unordered_map>

#include "endian.hpp"

#ifdef __linux__

extern "C" {
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
}

static std::unordered_map<void*, off_t> mapped_files_sizes;

namespace gcl {
    auto mapFile(std::string_view fileName) -> void* {
        const auto file = open(fileName.data(), O_RDONLY | O_NOATIME);
        struct stat statRes;
        fstat(file, &statRes);

        const auto fileSize = statRes.st_size;

        auto* const ptr = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, file, 0);
        if (ptr == MAP_FAILED) {
            return nullptr;
        }

        mapped_files_sizes[ptr] = fileSize;

        return ptr;
    }

    auto unmapFile(void* mappedAt) -> bool {
        if (mapped_files_sizes.contains(mappedAt)) {
            return munmap(mappedAt, mapped_files_sizes.at(mappedAt)) == 0;
        }
        return false;
    }
#endif

    auto ValidateMappedFile(void* mappedAt) -> std::optional<gcl::FileOpenErrors> {
        if (const auto* const header = static_cast<gcl::FileHeader*>(mappedAt);
            BigEndianToHostEndian(header->magic) != 0xf00dbabe) {
            return gcl::FileOpenErrors::bad_magic;
        }

        // TODO: Check version match
        return {};
    }
}  // namespace gcl