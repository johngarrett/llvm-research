//===--- mRISCV.h - Declare mRISCV target feature support ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares mRISCVTargetInfo objects.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_mRISCV_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_mRISCV_H

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Compiler.h"

namespace clang {
namespace targets {

class LLVM_LIBRARY_VISIBILITY mRISCVTargetInfo: public TargetInfo {
    static const char *const GCCRegNames[];

public:
    mRISCVTargetInfo(const llvm::Triple &Triple, const TargetOptions &): TargetInfo(Triple) {
        // Description string has to be kept in sync with backend string at
        // llvm/lib/Target/mRISCV/mRISCVTargetMachine.cpp

        resetDataLayout("e"
                        // ELF name mangling (?) 
                        "-m:e"
                        // 32-bit pointers
                        "-p:32:32"
                        // 64-bit pointers
                        "-i64:64"
                        // 32-bit native integer width (registers are 32-bit)
                        "-n32"
                        // 128-bit stack alignment
                        "-S128"
        );
        SuitableAlign = 128;
        WCharType = SignedInt;
        WIntType = UnsignedInt;
        IntPtrType = SignedInt;
        PtrDiffType = SignedInt;
        SizeType = UnsignedInt;
    }

    void getTargetDefines(const LangOptions &Opts,
                          MacroBuilder &Builder) const override;

    ArrayRef<const char *> getGCCRegNames() const override;

    ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override;

    BuiltinVaListKind getBuiltinVaListKind() const override {
        return TargetInfo::VoidPtrBuiltinVaList;
    }

    ArrayRef<Builtin::Info> getTargetBuiltins() const override {
        return None;
    }
    
    bool validateAsmConstraint(const char *&Name,
                               TargetInfo::ConstraintInfo &info) const override {
        return false;
    }

    /*
    Per llvm's documentation:
    > A clobber does not consume an input operand, nor generate an output.
    */
    const char *getClobbers() const override {
        return "";
    }
 };

} // end namespace targets
} // end namespace clang

#endif