//===----------------------------------------------------------------------===//
//
// Top tablegen file that pulls in code from the LLVM framework along with
// code specific to this target.
//
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Calling Conv, Instruction Descriptions, Register information, etc
//===----------------------------------------------------------------------===//

include "mRISCVSchedule.td"
include "mRISCVRegisterInfo.td"
include "mRISCVCallingConv.td"
include "mRISCVInstrInfo.td"

def mRISCVInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// mRISCV processors supported
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic",  NoSchedModel, []>;

//===----------------------------------------------------------------------===//
// mRISCV Target declaration
//===----------------------------------------------------------------------===//

def mRISCV : Target {
    let InstructionSet = mRISCVInstrInfo;
}