#ifndef LLVM_LIB_TARGET_mRISCV_MCTARGETDESC_mRISCVMCTARGETDESC_H
#define LLVM_LIB_TARGET_mRISCV_MCTARGETDESC_mRISCVMCTARGETDESC_H

/*
    Defines symbolic names for mRISCV registers.

    This allows us to map register names into register numbers
*/
#define GET_REGINFO_ENUM
#include "mRISCVGenRegisterInfo.inc"

/*
    Defines symbol names for mRISCV instructions.

    This allows us to map instruction names -> opcodes 

    TODO: put examples
*/
#define GET_INSTRINFO_ENUM
#include "mRISCVGenInstrInfo.inc"

/*
    TODO: document
*/
#define GET_SUBTARGETINFO_ENUM
#include "mRISCVGenSubtargetInfo.inc"


#endif