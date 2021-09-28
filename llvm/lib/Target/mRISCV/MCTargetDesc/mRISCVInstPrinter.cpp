// Convert mRISCV MCInst to assembly syntax

/*
    https://llvm.org/devmtg/2011-11/Grosbach_Anderson_LLVMMC.pdf

    Prints an mRISCV MCInst to a .s file
*/
#include "mRISCVInstPrinter.h"

#include "mRISCVInstrInfo.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "mRISCV-isel"

#define PRINT_ALIAS_INSTR
#include "mRISCVGenAsmWriter.inc"

mRISCVInstPrinter::mRISCVInstPrinter(const MCAsmInfo &MAI,
                                   const MCInstrInfo &MII,
                                   const MCRegisterInfo &MRI)
    : MCInstPrinter(MAI, MII, MRI) {}

void mRISCVInstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const {
    OS << StringRef(getRegisterName(RegNo)).lower();
}

/*
    Per: https://blog.llvm.org/2010/04/intro-to-llvm-mc-project.html

    >Given a single MCInst, [an instruction printer] formats and emits 
    the textual representation of the instruction to a raw_ostream

    That is what we are doing here
*/
void mRISCVInstPrinter::printInst(const MCInst *MI,
                                 uint64_t Address,
                                 StringRef Annot,
                                 const MCSubtargetInfo &STI,
                                 raw_ostream &O) {
    // Print any aliases first
    if (!printAliasInstr(MI, O)) {
        printInstruction(MI, Address, O);
    }
    printAnnotation(O, Annot);
}

void mRISCVInstPrinter::printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O) {
    const MCOperand &Op = MI->getOperand(OpNo);
    
    if(Op.isReg()) {
        printRegName(O, Op.getReg());
        return;
    }

    // Imm - intermediate?
    if(Op.isImm()) {
        O << Op.getImm();
        return;
    }

    assert(Op.isExpr() && "unknown operand kind in printOperand");
    Op.getExpr()->print(O, &MAI, true);
}