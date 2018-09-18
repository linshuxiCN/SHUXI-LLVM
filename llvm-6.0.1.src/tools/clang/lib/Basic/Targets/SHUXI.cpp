//===--- SHUXI.cpp - Implement SHUXI target feature support ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements SHUXI TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "SHUXI.h"
#include "Targets.h"
#include "clang/Basic/Builtins.h"
#include "clang/Basic/MacroBuilder.h"
#include "clang/Basic/TargetBuiltins.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang;
using namespace clang::targets;

const Builtin::Info SHUXITargetInfo::BuiltinInfo[] = {
#define BUILTIN(ID, TYPE, ATTRS)                                               \
  {#ID, TYPE, ATTRS, nullptr, ALL_LANGUAGES, nullptr},
#define LIBBUILTIN(ID, TYPE, ATTRS, HEADER)                                    \
  {#ID, TYPE, ATTRS, HEADER, ALL_LANGUAGES, nullptr},
#define TARGET_BUILTIN(ID, TYPE, ATTRS, FEATURE)                               \
  {#ID, TYPE, ATTRS, nullptr, ALL_LANGUAGES, FEATURE},
#include "clang/Basic/BuiltinsSHUXI.def"
};

SHUXITargetInfo::SHUXITargetInfo(const llvm::Triple &Triple, const TargetOptions &) :
  TargetInfo(Triple)
{
  BigEndian = false;
  NoAsmVariants = true;
  LongLongAlign = 32;
  SuitableAlign = 32;
  DoubleAlign = LongDoubleAlign = 32;
  SizeType = UnsignedInt;
  PtrDiffType = SignedInt;
  IntPtrType = SignedInt;
  WCharType = UnsignedChar;
  WIntType = UnsignedInt;
  UseZeroLengthBitfieldAlignment = true;
  resetDataLayout("e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32"
    "-f64:32-a:0:32-n32");
}

ArrayRef<Builtin::Info> SHUXITargetInfo::getTargetBuiltins() const {
  return llvm::makeArrayRef(BuiltinInfo,
                            clang::SHUXI::LastTSBuiltin - Builtin::FirstTSBuiltin);
}
