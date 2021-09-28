
#ifndef LLVM_LIB_TARGET_mRISCV_mRISCVTARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_mRISCV_mRISCVTARGETOBJECTFILE_H

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {
class mRISCVTargetObjectFile : public TargetLoweringObjectFileELF {
public:
  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;
};
} // end namespace llvm

#endif // end LLVM_LIB_TARGET_mRISCV_mRISCVTARGETOBJECTFILE_H