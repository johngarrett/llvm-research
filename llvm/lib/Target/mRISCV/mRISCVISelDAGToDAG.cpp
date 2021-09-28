//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the mRISCV target.
//
//===----------------------------------------------------------------------===//

#include "mRISCVISelDAGToDAG.h"
#include "mRISCVSubtarget.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

using namespace llvm;

#define DEBUG_TYPE "mRISCV-isel"

bool mRISCVDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
    Subtarget = &MF.getSubtarget<mRISCVSubtarget>();
  return SelectionDAGISel::runOnMachineFunction(MF);
}

void mRISCVDAGToDAGISel::Select(SDNode *Node) {
  // If we have a custom node, we have already selected.
  if (Node->isMachineOpcode()) {
    LLVM_DEBUG(errs() << "== "; Node->dump(CurDAG); errs() << "\n");
    Node->setNodeId(-1);
    return;
  }

  // Instruction Selection not handled by the auto-generated tablegen selection
  // should be handled here.
  unsigned Opcode = Node->getOpcode();
  // we are not dealing with those yet
  switch (Opcode) {
  default:
    break;
  }

  // Select the default instruction
  SelectCode(Node);
}