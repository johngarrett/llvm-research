//===----------------------------------------------------------------------===//
//
// This file contains the mRISCV implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_mRISCV_mRISCVREGISTERINFO_H
#define LLVM_LIB_TARGET_mRISCV_mRISCVREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "mRISCVGenRegisterInfo.inc"

namespace llvm {
class mRISCVSubtarget;

class mRISCVRegisterInfo : public mRISCVGenRegisterInfo {
protected:
  const mRISCVSubtarget &Subtarget;

public:
  mRISCVRegisterInfo(const mRISCVSubtarget &Subtarget);

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool requiresRegisterScavenging(const MachineFunction &MF) const override;
  bool requiresFrameIndexScavenging(const MachineFunction &MF) const override;
  bool requiresFrameIndexReplacementScavenging(
      const MachineFunction &MF) const override;

  bool trackLivenessAfterRegAlloc(const MachineFunction &MF) const override;

  void eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  Register getFrameRegister(const MachineFunction &MF) const override;

  const TargetRegisterClass *intRegClass(unsigned Size) const;
};

} // end namespace llvm

#endif // end LLVM_LIB_TARGET_mRISCV_mRISCVREGISTERINFO_H