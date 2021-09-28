//===----------------------------------------------------------------------===//
//
// This file implements the mRISCV specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "mRISCVSubtarget.h"
#include "mRISCV.h"
#include "mRISCVMachineFunction.h"
#include "mRISCVRegisterInfo.h"
#include "mRISCVTargetMachine.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "mRISCV-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "mRISCVGenSubtargetInfo.inc"

mRISCVSubtarget::mRISCVSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                                 const TargetMachine &TM)
    : mRISCVGenSubtargetInfo(TT, CPU, FS), TSInfo(),
      InstrInfo(initializeSubtargetDependencies(TT, CPU, FS, TM)),
      FrameLowering(*this), TLInfo(TM, *this), RegInfo(*this) {}

mRISCVSubtarget &mRISCVSubtarget::initializeSubtargetDependencies(
    const Triple &TT, StringRef CPU, StringRef FS, const TargetMachine &TM) {
  std::string CPUName = CPU;
  if (CPUName.empty()) {
    CPUName = "generic";
  }

  ParseSubtargetFeatures(CPUName, FS);

  return *this;
}