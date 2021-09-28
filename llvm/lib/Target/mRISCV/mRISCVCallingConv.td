//===----------------------------------------------------------------------===//
// This describes the calling conventions for mRISCV architecture.
//===----------------------------------------------------------------------===//

def mRISCV_CCallingConv : CallingConv<[
    // tell LLVM size and aligned requirements for each stack slot
    CCIfByVal<CCPassByVal<4,4>>,

    // Promote to i32
    // "if type is i1,i8,i16, promote to i32"
    CCIfType<[i1, i8, i16], CCPromoteToType<i32>>,

    // all arguments have now been promoted,
    // first 4 arguments are passed through X0-X3
    CCIfType<[i32], CCAssignToReg<[X0, X1, X2, X3]>>,

    // stored with the same stack slot requirements
    CCIfType<[i32], CCAssignToStack<4, 4>>
]>;


// BeyondRISC C Return Convention
def mRISCV_CRetConv : CallingConv<[
    // promote again
    CCIfType<[i1, i8, i16], CCPromoteToType<i32>>,

   // i32 are returned in registers X0, X1
   CCIfType<[i32], CCAssignToReg<[X0, X1]>>,

   // Integer values get stored in stack slots that are 4 bytes in size and
   // 4-byte aligned
   CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

// from RISCVCallingConv.td
// Needed for implementation of RISCVRegisterInfo::getNoPreservedMask()
def CSR_NoRegs : CalleeSavedRegs<(add)>;

/*
    https://riscv.org/wp-content/uploads/2015/01/riscv-calling.pdf: 

    > Twelve integer registers s0–s11 and twelve floating-point registers fs0–fs11 
    are preserved across calls and must be saved by the callee if used

    X8 -> Frame Pointer
    X9 -> Saved Register
*/
def mRISCV_CalleeSavedRegs : CalleeSavedRegs<(add X8, X9)>;