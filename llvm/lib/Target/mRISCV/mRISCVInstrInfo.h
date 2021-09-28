//===----------------------------------------------------------------------===//
//
// This file contains the mRISCV implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_mRISCV_mRISCVINSTRINFO_H
#define LLVM_LIB_TARGET_mRISCV_mRISCVINSTRINFO_H

#include "mRISCV.h"
#include "mRISCVRegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "mRISCVGenInstrInfo.inc"

namespace llvm {

class mRISCVInstrInfo : public mRISCVGenInstrInfo {
public:
  explicit mRISCVInstrInfo(const mRISCVSubtarget &STI);

protected:
  const mRISCVSubtarget &Subtarget;
};
} // namespace llvm

#endif // end LLVM_LIB_TARGET_mRISCV_mRISCVINSTRINFO_H