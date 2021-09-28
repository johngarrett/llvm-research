//===----------------------------------------------------------------------===//
//
// Implements the info about mRISCV target spec.
//
//===----------------------------------------------------------------------===//

#include "mRISCVTargetMachine.h"
#include "TargetInfo/mRISCVTargetInfo.h"
#include "mRISCVISelDAGToDAG.h"
#include "mRISCVSubtarget.h"
#include "mRISCVTargetObjectFile.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializemRISCVTarget() {
  // Register the target.
  //- Little endian Target Machine
  RegisterTargetMachine<mRISCVTargetMachine> X(getThemRISCVTarget());
}

static std::string computeDataLayout() {
  // e          Little endian
  // -m:e       ELF Name Mangling 
  // -p:32:32   32 bit pointers; 32 bit aligned
  // -i64:64    64 bit ints; 64 bit aligned
  // -n32       registers are 32 bits (native width)
  // -S128      128 bit stack alignment
  return "e-m:e-p:32:32-i64:64-n32-S128";
}

static Reloc::Model getEffectiveRelocModel(Optional<CodeModel::Model> CM,
                                           Optional<Reloc::Model> RM) {
  return !RM.hasValue() ? Reloc::Static : *RM;
}

mRISCVTargetMachine::mRISCVTargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         Optional<Reloc::Model> RM,
                                         Optional<CodeModel::Model> CM,
                                         CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(), TT, CPU, FS, Options,
                        getEffectiveRelocModel(CM, RM),
                        getEffectiveCodeModel(CM, CodeModel::Medium), OL),
      TLOF(std::make_unique<mRISCVTargetObjectFile>()) {
  initAsmInfo();

  //setMachineOutliner(true);
}

const mRISCVSubtarget *
mRISCVTargetMachine::getSubtargetImpl(const Function &F) const {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");

  std::string CPU = !CPUAttr.hasAttribute(Attribute::None)
                        ? CPUAttr.getValueAsString().str()
                        : TargetCPU;
  std::string FS = !FSAttr.hasAttribute(Attribute::None)
                       ? FSAttr.getValueAsString().str()
                       : TargetFS;

  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    // This needs to be done before we create a new subtarget since any
    // creation will depend on the TM and the code generation flags on the
    // function that reside in TargetOptions.
    resetTargetOptions(F);
    // skip ABI handling stuff
    I = std::make_unique<mRISCVSubtarget>(TargetTriple, CPU, FS, *this);
  }
  return I.get();
}

namespace {
class mRISCVPassConfig : public TargetPassConfig {
public:
  mRISCVPassConfig(mRISCVTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  mRISCVTargetMachine &getmRISCVTargetMachine() const {
    return getTM<mRISCVTargetMachine>();
  }

  bool addInstSelector() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *mRISCVTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new mRISCVPassConfig(*this, PM);
}

// Install an instruction selector pass using
// the ISelDag to generate mRISCV code.
bool mRISCVPassConfig::addInstSelector() {
  addPass(new mRISCVDAGToDAGISel(getmRISCVTargetMachine(), getOptLevel()));

  return false;
}

// Implemented by targets that want to run passes immediately before
// machine code is emitted. return true if -print-machineinstrs should
// print out the code after the passes.
void mRISCVPassConfig::addPreEmitPass() {}