//===----------------------------------------------------------------------===//
//
// This file declares the mRISCV specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_mRISCV_mRISCVTARGETMACHINE_H
#define LLVM_LIB_TARGET_mRISCV_mRISCVTARGETMACHINE_H

#include "mRISCVSubtarget.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class mRISCVTargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  mutable StringMap<std::unique_ptr<mRISCVSubtarget>> SubtargetMap;

public:
  mRISCVTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                      CodeGenOpt::Level OL, bool JIT);

  const mRISCVSubtarget *getSubtargetImpl(const Function &F) const override;
  const mRISCVSubtarget *getSubtargetImpl() const = delete;

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};
} // namespace llvm

#endif // end LLVM_LIB_TARGET_mRISCV_mRISCVTARGETMACHINE_H