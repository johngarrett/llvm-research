/*
    provide mRISCV specific target descriptions
*/

#include "mRISCVMCTargetDesc.h"
#include "mRISCVInstPrinter.h"
#include "mRISCVMCAsmInfo.h"
#include "TargetInfo/mRISCVTargetInfo.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "mRISCVGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "mRISCVGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "mRISCVGenRegisterInfo.inc"

static MCInstrInfo *createmRISCVMCInstrInfo() {
    MCInstrInfo *M = new MCInstrInfo();
    InitmRISCVMCInstrInfo(M);
    return M;
}

static MCRegisterInfo *createmRISCVMCRegisterInfo(const Triple &TT) {
    MCRegisterInfo *R = new MCRegisterInfo();
    return R;
}

static MCSubtargetInfo *createmRISCVMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
    std::string CPUName = CPU;
    if (CPUName.empty()) {
        CPUName = "generic";
    }
    return createmRISCVMCSubtargetInfoImpl(TT, CPUName, FS);
}

static MCInstPrinter *createmRISCVMCInstPrinter(const Triple &T,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
    return new mRISCVInstPrinter(MAI, MII, MRI);
}

static MCAsmInfo *createmRISCVMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT,
                                       const MCTargetOptions &Options) {
    MCAsmInfo *MAI = new mRISCVMCAsmInfo(TT);

    unsigned WP = MRI.getDwarfRegNum(mRISCV::X2, true); // TODO: what's this doing
    MCCFIInstruction Inst = MCCFIInstruction::createDefCfa(nullptr, WP, 0); // TODO: this too
    MAI->addInitialFrameState(Inst);

    return MAI;
}

extern "C" void LLVMInitializemRISCVTargetMC() {
    for (Target *T : {&getThemRISCVTarget()}) {
        // Register the MC asm info.
        TargetRegistry::RegisterMCAsmInfo(*T, createmRISCVMCAsmInfo);

        // Register the MC instruction info.
        TargetRegistry::RegisterMCInstrInfo(*T, createmRISCVMCInstrInfo);

        // Register the MC register info.
        TargetRegistry::RegisterMCRegInfo(*T, createmRISCVMCRegisterInfo);

        // Register the MC subtarget info.
        TargetRegistry::RegisterMCSubtargetInfo(*T, createmRISCVMCSubtargetInfo);

        // Register the MCInstPrinter.
        TargetRegistry::RegisterMCInstPrinter(*T, createmRISCVMCInstPrinter);
    }
}
