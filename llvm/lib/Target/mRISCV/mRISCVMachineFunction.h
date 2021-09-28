//===----------------------------------------------------------------------===//
//
// This file declares the mRISCV specific subclass of MachineFunctionInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_mRISCV_mRISCVMACHINEFUNCTION_H
#define LLVM_LIB_TARGET_mRISCV_mRISCVMACHINEFUNCTION_H

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

/// mRISCVFunctionInfo - This class is derived from MachineFunction private
/// mRISCV target-specific information for each MachineFunction.
class mRISCVFunctionInfo : public MachineFunctionInfo {
private:
  MachineFunction &MF;

public:
  mRISCVFunctionInfo(MachineFunction &MF) : MF(MF) {}
};

} // end of namespace llvm

#endif // end LLVM_LIB_TARGET_mRISCV_mRISCVMACHINEFUNCTION_H