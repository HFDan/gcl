# VM

The VM is the main context in which GCL is interpreted

## Definition

```c++
class gcl::VM final {
private:
    Registers regs;

    gcl::FileInfo LoadFile(std::string_view file) const;

    void Run(uint8_t* code);

    gcl::Linker linker;

public:

    VM() = delete;

    explicit VM(size_t stackSize = 2048);

    ~VM();

    void DoFile(std::string_view file);
};
```

## Members

### Public

#### Functions

`VM(size_t stackSize)` - Constructs a VM with a given stack size in bytes
`void DoFile(std::string_view file)` - Opens a file by name, and executes it
