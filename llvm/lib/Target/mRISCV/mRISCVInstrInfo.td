//===----------------------------------------------------------------------===//
 //
 // This file contains the mRISCV implementation of the TargetInstrInfo class.
 //
 //===----------------------------------------------------------------------===//

 //===----------------------------------------------------------------------===//
 // Instruction format superclass
 //===----------------------------------------------------------------------===//

 // Procedure return
 def mRISCV_ret : SDNode<"mRISCVISD::Ret", SDTNone,
                        [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

 // Procedure calling
 def SDT_CallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>,
                                        SDTCisVT<1, i32>]>;
 def SDT_CallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>,
                                     SDTCisVT<1, i32>]>;

 // These are target-independent nodes, but have target-specific formats.
 def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_CallSeqStart,
                            [SDNPHasChain, SDNPOutGlue]>;
 def callseq_end   : SDNode<"ISD::CALLSEQ_END", SDT_CallSeqEnd,
                            [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

 include "mRISCVInstrFormats.td"


 let Defs = [X2], Uses = [X2] in {
    def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                [(callseq_start timm:$amt1, timm:$amt2)]>;
    def ADJCALLSTACKUP   : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                [(callseq_end timm:$amt1, timm:$amt2)]>;
 } // end Defs = [X2], Uses = [X2]

 /* Arithmetic and logic */
 let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in {
    class ALU_rr<bits<7> funct7, bits<3> funct3, string opcodestr>
        : RWInstR<funct7, funct3, OPC_OP, (outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
                opcodestr, "$rd, $rs1, $rs2">;
 }

def ADD  : ALU_rr<0b0000000, 0b000, "add">,
            Sched<[WriteIALU, ReadIALU, ReadIALU]>;

class PatGprGpr<SDPatternOperator OpNode, RWInst Inst>
    : Pat<(OpNode GPR:$rs1, GPR:$rs2), (Inst GPR:$rs1, GPR:$rs2)>;

def : PatGprGpr<add, ADD>;

/* 
    Procedure calling instructions

    Double checked with 'llvm/lib/Target/RISCV/RISCVInstrInfo.td'

*/

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in {
    let isCall = 1 in
    def JALR : RWInstI<0b000, OPC_JALR, (outs GPR:$rd),
                    (ins GPR:$rs1, i32imm:$imm12),
                    "jalr", "$rd, ${imm12}(${rs1})">,
            Sched<[WriteJalr, ReadJalr]>;
} // end hasSideEffects = 0, mayLoad = 0, mayStore = 0


/*
    Per: https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf

    ret from subroutine aliases to jalr x0, x1, 0

*/
let isReturn = 1, isBarrier = 1, isTerminator = 1 in
def PseudoRET : Pseudo<(outs), (ins), [(mRISCV_ret)]>,
                PseudoInstExpansion<(JALR X0, X1, 0)>;

def : InstAlias<"ret", (JALR X0, X1, 0), 4>;


// Necessary because a frameindex can't be matched directly in a pattern.
def AddrFI : ComplexPattern<iPTR, 1, "SelectAddrFI", [frameindex], []>;

/*
    Loads

multiclass LdPat<PatFrag LoadOp, RWInst Inst> {
  def : Pat<(LoadOp GPR:$rs1), (Inst GPR:$rs1, 0)>;
  def : Pat<(LoadOp AddrFI:$rs1), (Inst AddrFI:$rs1, 0)>;
}

def LB  : Load_ri<0b000, "lb">, Sched<[WriteLDB, ReadMemBase]>;
def LH  : Load_ri<0b001, "lh">, Sched<[WriteLDH, ReadMemBase]>;
def LW  : Load_ri<0b010, "lw">, Sched<[WriteLDW, ReadMemBase]>;
def LBU : Load_ri<0b100, "lbu">, Sched<[WriteLDB, ReadMemBase]>;
def LHU : Load_ri<0b101, "lhu">, Sched<[WriteLDH, ReadMemBase]>

defm : LdPat<sextloadi8, LB>;
defm : LdPat<extloadi8, LB>;
defm : LdPat<sextloadi16, LH>;
defm : LdPat<extloadi16, LH>;
defm : LdPat<load, LW>, Requires<[IsRV32]>;
defm : LdPat<zextloadi8, LBU>;
defm : LdPat<zextloadi16, LHU>;


defm : LdPat<sextloadi32, LW>;
defm : LdPat<extloadi32, LW>;
defm : LdPat<zextloadi32, LWU>;
defm : LdPat<load, LD>;

*/