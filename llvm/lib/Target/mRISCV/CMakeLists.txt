set(LLVM_TARGET_DEFINITIONS mRISCV.td)

tablegen(LLVM mRISCVGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM mRISCVGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM mRISCVGenCodeEmitter.inc -gen-emitter)
tablegen(LLVM mRISCVGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM mRISCVGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM mRISCVGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM mRISCVGenCallingConv.inc -gen-callingconv)
tablegen(LLVM mRISCVGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM mRISCVGenMCPseudoLowering.inc -gen-pseudo-lowering)

# mRISCVCommonTableGen must be defined
add_public_tablegen_target(mRISCVCommonTableGen)

# mRISCVCodeGen should match with LLVMBuild.txt mRISCVCodeGen
add_llvm_target(mRISCVCodeGen
    mRISCVAsmPrinter.cpp
    mRISCVFrameLowering.cpp
    mRISCVInstrInfo.cpp
    mRISCVISelLowering.cpp
    mRISCVRegisterInfo.cpp
    mRISCVSubtarget.cpp
    mRISCVTargetObjectFile.cpp
    mRISCVISelDAGToDAG.cpp
    mRISCVTargetMachine.cpp
)

# Should match with "subdirectories =  MCTargetDesc TargetInfo" in LLVMBuild.txt
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)