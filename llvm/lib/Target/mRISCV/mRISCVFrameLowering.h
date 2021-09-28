#ifndef LLVM_LIB_TARGET_mRISCV_mRISCVFRAMELOWERING_H
#define LLVM_LIB_TARGET_mRISCV_mRISCVFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class mRISCVSubtarget;

class mRISCVFrameLowering : public TargetFrameLowering {
protected:
  const mRISCVSubtarget &STI;

public:
  explicit mRISCVFrameLowering(const mRISCVSubtarget &STI)
      : TargetFrameLowering(TargetFrameLowering::StackGrowsDown,
                            /*StackAlignment*/ Align(4),
                            /*LocalAreaOffset*/ 0,
                            /*TransAl*/ Align(4)),
        STI(STI) {}

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasReservedCallFrame(const MachineFunction &MF) const override;
  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS) const override;

  bool hasFP(const MachineFunction &MF) const override;
};
} // namespace llvm

#endif // end LLVM_LIB_TARGET_mRISCV_mRISCVFRAMELOWERING_H