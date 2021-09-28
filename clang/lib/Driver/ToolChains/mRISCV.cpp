// documentation: https://clang.llvm.org/doxygen/classclang_1_1driver_1_1toolchains_1_1MSP430ToolChain.html

#include "mRISCV.h"
#include "CommonArgs.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Options.h"
#include "llvm/Option/ArgList.h"

using namespace clang::driver;
using namespace clang::driver::toolchains;
using namespace clang;
using namespace llvm::opt;

mRISCVToolChain::mRISCVToolChain(const Driver &D, const llvm::Triple &Triple,
                               const ArgList &Args)
    : ToolChain(D, Triple, Args) {

}

// Position Independent code (https://wikipedia.org/wiki/Position-independent_code)
bool mRISCVToolChain::isPICDefault() const { return true; }

bool mRISCVToolChain::isPIEDefault() const { return false; }

bool mRISCVToolChain::isPICDefaultForced() const { return true; }

bool mRISCVToolChain::SupportsProfiling() const { return false; }

bool mRISCVToolChain::hasBlocksRuntime() const { return false; }