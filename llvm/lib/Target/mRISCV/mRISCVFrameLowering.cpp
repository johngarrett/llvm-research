#include "mRISCVFrameLowering.h"
#include "mRISCVSubtarget.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/Support/Debug.h"

using namespace llvm;

/*
    This returns true if the specified function should have
    a dedicated frame pointer register.

    This is true if:
        - needs dynamic stack realignment
        - FP elimination is disabled
        - Frame address is taken
        - variable sized objects

    override to false right now
*/
bool mRISCVFrameLowering::hasFP(const MachineFunction &MF) const {
  return false;
}

MachineBasicBlock::iterator mRISCVFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  return MBB.erase(I);
}

void mRISCVFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {}

void mRISCVFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {}

bool mRISCVFrameLowering::hasReservedCallFrame(
    const MachineFunction &MF) const {
  return true;
}

// This method is called immediately before PrologEpilogInserter scans the
// physical registers used to determine what callee saved registers should be
// spilled. This method is optional.
void mRISCVFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                               BitVector &SavedRegs,
                                               RegScavenger *RS) const {
  // use the callee saved registers we defined earlier
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
}