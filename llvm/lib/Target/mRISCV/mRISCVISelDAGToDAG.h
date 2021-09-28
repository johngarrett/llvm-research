//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the mRISCV target.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_mRISCV_mRISCVISELDAGTODAG_H
#define LLVM_LIB_TARGET_mRISCV_mRISCVISELDAGTODAG_H

#include "mRISCVSubtarget.h"
#include "mRISCVTargetMachine.h"
#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {
class mRISCVDAGToDAGISel : public SelectionDAGISel {
public:
  explicit mRISCVDAGToDAGISel(mRISCVTargetMachine &TM, CodeGenOpt::Level OL)
      : SelectionDAGISel(TM, OL), Subtarget(nullptr) {}

  // Pass Name
  StringRef getPassName() const override {
    return "CPU0 DAG->DAG Pattern Instruction Selection";
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  void Select(SDNode *Node) override;

#include "mRISCVGenDAGISel.inc"

private:
  const mRISCVSubtarget *Subtarget;
};
} // namespace llvm

#endif // end LLVM_LIB_TARGET_mRISCV_mRISCVISELDAGTODAG_H