#include "TargetInfo/mRISCVTargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

Target &llvm::getThemRISCVTarget() {
    static Target ThemRISCVTarget;
    return ThemRISCVTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializemRISCVTargetInfo() {
    RegisterTarget<Triple::mRISCV> X(getThemRISCVTarget(), "mRISCV", "32-bit RISC-V", "mRISCV");
}