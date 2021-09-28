//===----------------------------------------------------------------------===//
//
// This file contains the mRISCV implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "mRISCVRegisterInfo.h"
#include "mRISCVSubtarget.h"
#include "llvm/Support/Debug.h"

#define GET_REGINFO_TARGET_DESC
#include "mRISCVGenRegisterInfo.inc"

#define DEBUG_TYPE "mRISCV-reginfo"

using namespace llvm;

mRISCVRegisterInfo::mRISCVRegisterInfo(const mRISCVSubtarget &ST)
    : mRISCVGenRegisterInfo(mRISCV::X1, /*DwarfFlavour*/ 0, /*EHFlavor*/ 0,
                            /*PC*/ 0),
      Subtarget(ST) {}

const MCPhysReg *
mRISCVRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  // we don't have to handle any RISCV extensions or ABI changes yet
  // so, just return the default save list
  return mRISCV_CalleeSavedRegs_SaveList;
}

const TargetRegisterClass *
mRISCVRegisterInfo::intRegClass(unsigned Size) const {
  return &mRISCV::GPRRegClass;
}

const uint32_t *
mRISCVRegisterInfo::getCallPreservedMask(const MachineFunction &MF, CallingConv::ID) const {
  return mRISCV_CalleeSavedRegs_RegMask;
}

BitVector mRISCVRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  markSuperRegs(Reserved, mRISCV::X0); // zero register
  markSuperRegs(Reserved, mRISCV::X2); // stack pointer
  markSuperRegs(Reserved, mRISCV::X3); // global pointer
  markSuperRegs(Reserved, mRISCV::X4); // thread pointer

  return Reserved;
}

void mRISCVRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                             int SPAdj, unsigned FIOperandNum,
                                             RegScavenger *RS) const {
  llvm_unreachable("Unsupported eliminateFrameIndex");
}

bool mRISCVRegisterInfo::requiresRegisterScavenging(const MachineFunction &MF) const {
  return true;
}

bool mRISCVRegisterInfo::requiresFrameIndexScavenging(const MachineFunction &MF) const {
  return true;
}

bool mRISCVRegisterInfo::requiresFrameIndexReplacementScavenging(const MachineFunction &MF) const {
  return true;
}

bool mRISCVRegisterInfo::trackLivenessAfterRegAlloc(const MachineFunction &MF) const {
  return true;
}

Register mRISCVRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  llvm_unreachable("Unsupported getFrameRegister");
}