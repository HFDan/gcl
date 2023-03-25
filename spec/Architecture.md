# Architecture

## Virtual Machine

### Pseudo-Registers

```yaml
ip: addr(u1) # The instruction pointer. Holds the addy of the current function.
sp: addr(u1) # Stack pointer.
```

### Runtime

```mermaid
flowchart LR
    Input --> VM.Constructor
    subgraph VM
    VM.Constructor[Constructor]
        subgraph Initialize VM
            VM.Constructor --> VMInit.AllocStack[Allocate Stack] --> VMInit.InitRSP[Initialize Stack Pointer] --> FileParser.checkMagic
            VMInit.InitRIP[Initialize Instruction Pointer]
            subgraph File Parsing
                FileParser.checkMagic[Check Magic] --> FileParser.isMagicPresent{Is magic present?}
                FileParser.isMagicPresent --> |Yes| FileParser.LoadMetadata[Load Metadata]
                FileParser.isMagicPresent --> |No| FileParser.Die[Stop Parsing]
                FileParser.Die --> ThrowException[Throw Exception] --> VM.CloseGracefully

                FileParser.LoadMetadata --> FileParser.LoadConstantData[Load Constant Data] --> FileParser.LoadFunctionDefinitions[Load Function Definitions] --> FileParser.FindMainFunction[Find Main Function] --> VMInit.InitRIP --> VM.PushFirstFrame
            end
        end

        VM.PushFirstFrame[Push First Stack Frame]

        subgraph Process Instructions
            VM.PushFirstFrame --> ProcInst.ReadOneByte[Read One Byte from IP] --> ProcInst.ExecuteCorrespondingInstruction[Execute Instruction] --> ProcInst.ShouldExit{Should Exit?}
            ProcInst.ShouldExit --> |Yes| VM.CloseGracefully
            ProcInst.ShouldExit --> |No| ProcInst.ReadOneByte
        end

        VM.CloseGracefully[Close Gracefully]
    end
```

## Name mangling

### Layout

```yaml
helloworld: Function name
(s): Parameter list
V: Return type
```

Thus, `helloworld(s)V` would translate to `void helloworld(string);`

### Types

|  Type  |   Identifier   |
|:------:|:--------------:|
|  Void  |       V        |
| Int32  | I  (Capital i) |
| Int16  |       i        |
| Int64  |       L        |
| Float  |       f        |
| Double |       d        |
|  Char  |       c        |
| String |       s        |
|  Array |      []        |
