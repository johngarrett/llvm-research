//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the RISCV assembly language.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/mRISCVInstPrinter.h"
#include "TargetInfo/mRISCVTargetInfo.h"
#include "mRISCVInstrInfo.h"
#include "mRISCVTargetMachine.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "mRISCV-asm-printer"

namespace llvm {
class mRISCVAsmPrinter : public AsmPrinter {
public:
  explicit mRISCVAsmPrinter(TargetMachine &TM,
                            std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  virtual StringRef getPassName() const override {
    return "mRISCV Assembly Printer";
  }

  void EmitInstruction(const MachineInstr *MI) override;

  void EmitToStreamer(MCStreamer &S, const MCInst &Inst);
  // used by emit to streamer
  bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,
                                   const MachineInstr *MI);

private:
  void LowermRISCVMachineInstrToMCInst(const MachineInstr *MI,
                                       MCInst &OutMI) const;
  MCOperand LowerOperand(const MachineOperand &MO) const;
  MCOperand LowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym) const;
};
} // end namespace llvm

// Simple pseudo-instructions have their lowering (with expansion to real
// instructions) auto-generated.
#include "mRISCVGenMCPseudoLowering.inc"
void mRISCVAsmPrinter::EmitToStreamer(MCStreamer &S, const MCInst &Inst) {
  AsmPrinter::EmitToStreamer(*OutStreamer, Inst);
}

void mRISCVAsmPrinter::EmitInstruction(const MachineInstr *MI) {
  // Do any auto-generated pseudo lowerings.
  if (emitPseudoExpansionLowering(*OutStreamer, MI)) {
    return;
  }

  MCInst TmpInst;
  LowermRISCVMachineInstrToMCInst(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

// lower a machien instrcution
void mRISCVAsmPrinter::LowermRISCVMachineInstrToMCInst(const MachineInstr *MI,
                                                       MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());

  //  for every machine operand, lower it and add to OutMI
  for (const MachineOperand &MO : MI->operands()) {
    MCOperand MCOp = LowerOperand(MO);
    if (MCOp.isValid()) {
      OutMI.addOperand(MCOp);
    }
  }
}

// lower an individual operand
MCOperand mRISCVAsmPrinter::LowerOperand(const MachineOperand &MO) const {
  switch (MO.getType()) {
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands.
    if (MO.isImplicit()) {
      break;
    }
    return MCOperand::createReg(MO.getReg());

  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm());

  case MachineOperand::MO_MachineBasicBlock:
    return LowerSymbolOperand(MO, MO.getMBB()->getSymbol());

  case MachineOperand::MO_GlobalAddress:
    return LowerSymbolOperand(MO, getSymbol(MO.getGlobal()));

  case MachineOperand::MO_BlockAddress:
    return LowerSymbolOperand(MO, GetBlockAddressSymbol(MO.getBlockAddress()));

  case MachineOperand::MO_ExternalSymbol:
    return LowerSymbolOperand(MO, GetExternalSymbolSymbol(MO.getSymbolName()));

  case MachineOperand::MO_ConstantPoolIndex:
    return LowerSymbolOperand(MO, GetCPISymbol(MO.getIndex()));

  case MachineOperand::MO_RegisterMask:
    break;

  default:
    report_fatal_error("[mRISCVASMPrinter::LowerOperand]: unknown operand type");
  }

  return MCOperand();
}

// lower an individual symbol operand
MCOperand mRISCVAsmPrinter::LowerSymbolOperand(const MachineOperand &MO,
                                               MCSymbol *Sym) const {
  MCContext &Ctx = OutContext;

  const MCExpr *Expr =
      MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, Ctx);

  // JTI - JumpTableIndex 
  // MBB - MachineBasicBlock
  // getOffset - returns 0 for ExternalSymbol operands
  if (!MO.isJTI() && !MO.isMBB() && MO.getOffset()) {
    Expr = MCBinaryExpr::createAdd(Expr,
                                   MCConstantExpr::create(MO.getOffset(), Ctx),
                                   Ctx);
  }

  return MCOperand::createExpr(Expr);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializemRISCVAsmPrinter() {
  RegisterAsmPrinter<mRISCVAsmPrinter> X(getThemRISCVTarget());
}