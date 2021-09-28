//===-- mRISCVTargetObjectFile.cpp - mRISCV Object Files --===//

#include "mRISCVTargetObjectFile.h"
#include "llvm/MC/MCContext.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

void mRISCVTargetObjectFile::Initialize(MCContext &Ctx,
                                        const TargetMachine &TM) {
  TargetLoweringObjectFileELF::Initialize(Ctx, TM);
}