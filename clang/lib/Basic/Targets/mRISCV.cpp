//===--- mRISCV.cpp - Implement mRISCV target feature support ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements mRISCVTargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "mRISCV.h"
#include "clang/Basic/MacroBuilder.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang;
using namespace clang::targets;

// Integer registers
const char *const mRISCVTargetInfo::GCCRegNames[] = {
    "x0", // zero constant
    "x1", // return address
    "x2", // stack pointer
    "x3", // global pointer
    "x4", // thread pointer
    "x5", "x6",  "x7", // temporaries
    "x8", // saved frame pointer
    "x9", // saved register
    "x10", "x11", // fn args/return values
    "x12", "x13", "x14", "x15", "x16", "x17", // fn args
    "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27",  // saved registers
    "x28", "x29", "x30", "x31", // temporaries
};

const TargetInfo::GCCRegAlias GCCRegAliases[] = {
    {{"zero"}, "x0"}, {{"ra"}, "x1"},   {{"sp"}, "x2"},    {{"gp"}, "x3"},
    {{"tp"}, "x4"},   {{"t0"}, "x5"},   {{"t1"}, "x6"},    {{"t2"}, "x7"},
    {{"s0"}, "x8"},   {{"s1"}, "x9"},   {{"a0"}, "x10"},   {{"a1"}, "x11"},
    {{"a2"}, "x12"},  {{"a3"}, "x13"},  {{"a4"}, "x14"},   {{"a5"}, "x15"},
    {{"a6"}, "x16"},  {{"a7"}, "x17"},  {{"s2"}, "x18"},   {{"s3"}, "x19"},
    {{"s4"}, "x20"},  {{"s5"}, "x21"},  {{"s6"}, "x22"},   {{"s7"}, "x23"},
    {{"s8"}, "x24"},  {{"s9"}, "x25"},  {{"s10"}, "x26"},  {{"s11"}, "x27"},
    {{"t3"}, "x28"},  {{"t4"}, "x29"},  {{"t5"}, "x30"},   {{"t6"}, "x31"}
};

ArrayRef<const char *>mRISCVTargetInfo::getGCCRegNames() const {
    return llvm::makeArrayRef(GCCRegNames);
}

ArrayRef<TargetInfo::GCCRegAlias> mRISCVTargetInfo::getGCCRegAliases() const {
    return llvm::makeArrayRef(GCCRegAliases);
}

void mRISCVTargetInfo::getTargetDefines(const LangOptions &Opts, MacroBuilder &Builder) const {
    // Define this macro, __mRISCV__, when building for this target
    // e.g. #ifdef __mRISCV__ 
    Builder.defineMacro("__mRISCV__");
}