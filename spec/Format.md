# Executable file format

The executable file (.gob - GCL Object) is structured in the following way:

All values in the file are **big endian** unless stated otherwise.

## Table of contents
1. [Structure](#Structure)
2. [Other structs](#Other structs)

###### Data types
| Name | Length in bytes | Signed? |
|:----:|:---------------:|:-------:|
|  u1  |        1        |   no    |
|  u2  |        2        |   no    |
|  u4  |        4        |   no    |
|  u8  |        8        |   no    |
|  i1  |        1        |   yes   |
|  i2  |        2        |   yes   |
|  i4  |        4        |   yes   |
|  i8  |        8        |   yes   |

## Structure
```yaml
magic: u4 # 0xf00dbabe

# These are for determining the version of GCL they were compiled with.
major_version: u2
minor_version: u2

data_length: u2
data: DataInfo[] # An array of size 'data_length'. Array is of type DataInfo.

function_count: u2
functions: FunctionInfo[]
```


## Other structs
### DataInfo

#### Generic layout:
```yaml
type: u1
content: u1[]
```

#### Types:

##### UTF-8
```yaml
type: 0x01 (UTF-8)
length: u2
data: u1[length]
```

##### Const_Int || Const_Float
```yaml
type: 0x02 (Const_Int) || 0x03 (Const_Float)
data: u4 # Read as unsigned, the program chooses to treat as signed or not. In the end, it's all bits
```

##### Const_Long || Const_Double
```yaml
type: 0x04 (Const_Long) || 0x05 (Const_Double)
data: u8 # Again, it's just bits.
```

#####

### FunctionInfo
```yaml
name_index: u2 # Points to UTF-8
code_length: u4
code: u1[code_length]
```