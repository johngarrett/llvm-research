#ifndef LLVM_LIB_TARGET_mRISCV_MCTARGETDESC_mRISCVMCASMINFO_H
#define LLVM_LIB_TARGET_mRISCV_MCTARGETDESC_mRISCVMCASMINFO_H

/*
    From MCAsmInfoELF.cpp:

    > This file defines target asm properties related what form asm statements
    should take in general on ELF-based targets
*/
#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class mRISCVMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit mRISCVMCAsmInfo(const Triple &TheTriple);
};

} // namespace llvm

#endif