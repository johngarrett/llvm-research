## steps
1. Create a subclass of the `TargetMachine` class that describes characteristics of your target machine
2. Describe the register set of the target
    -  Use TableGen to generate code for register definition, register aliases, and register classes from a target-specific RegisterInfo.td input file.
    - You should also write additional code for a subclass of the TargetRegisterInfo class that represents the class register file data used for register allocation and also describes the interactions between registers
3. Describe the instruction set of the target
    - Use TableGen to generate code for target-specific instructions from target-specific versions of TargetInstrFormats.td and TargetInstrInfo.td
    - You should write additional code for a subclass of the TargetInstrInfo class to represent machine instructions supported by the target machine
4. Describe the selection and conversion of the LLVM IR from a Directed Acyclic Graph (DAG) representation of instructions to native target-specific instructions. 
    - Use TableGen to generate code that matches patterns and selects instructions based on additional information in a target-specific version of TargetInstrInfo.td
    - Write code for XXXISelDAGToDAG.cpp, where XXX identifies the specific target, to perform pattern matching and DAG-to-DAG instruction selection
    - Also write code in XXXISelLowering.cpp to replace or remove operations and data types that are not supported natively in a SelectionDAG.
5. Write code for an assembly printer that converts LLVM IR to a GAS format for your target machine
    - You should add assembly strings to the instructions defined in your target-specific version of TargetInstrInfo.td
    - You should also write code for a subclass of AsmPrinter that performs the LLVM-to-assembly conversion and a trivial subclass of TargetAsmInfo.


