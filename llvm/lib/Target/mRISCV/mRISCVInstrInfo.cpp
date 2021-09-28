//===----------------------------------------------------------------------===//
//
// This file contains the mRISCV implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "mRISCVInstrInfo.h"

#include "mRISCVMachineFunction.h"
#include "mRISCVTargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "mRISCV-instrinfo"

#define GET_INSTRINFO_CTOR_DTOR
#include "mRISCVGenInstrInfo.inc"

mRISCVInstrInfo::mRISCVInstrInfo(const mRISCVSubtarget &STI)
    : mRISCVGenInstrInfo(mRISCV::ADJCALLSTACKDOWN, mRISCV::ADJCALLSTACKUP),
      Subtarget(STI) {}