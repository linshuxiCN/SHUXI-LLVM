//===--- SHUXI.h - Declare SHUXI target feature support -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares SHUXI TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_SHUXI_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_SHUXI_H
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Compiler.h"
namespace clang {
namespace targets {
class SHUXITargetInfo : public TargetInfo {
public:
  SHUXITargetInfo(const llvm::Triple &Triple, const TargetOptions &);

  void getTargetDefines(const LangOptions &Opts, MacroBuilder &Builder) const override {}

  static const Builtin::Info BuiltinInfo[];
  ArrayRef<Builtin::Info> getTargetBuiltins() const override;

  BuiltinVaListKind getBuiltinVaListKind() const override
  {
    return TargetInfo::VoidPtrBuiltinVaList;
  }

  const char *getClobbers() const override
  {
    return "";
  }

  ArrayRef<const char *> getGCCRegNames() const override
  {
    return None;
  }

  ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override
  {
    return None;
  }

  bool validateAsmConstraint(const char *&Name, TargetInfo::ConstraintInfo &Info) const override
  {
    return false;
  }

  int getEHDataRegisterNumber(unsigned RegNo) const override
  {
    // R0=ExceptionPointerRegister R1=ExceptionSelectorRegister
    return -1;
  }
};// namespace SHUXI targetInfo
} // namespace targets
} // namespace clang
#endif // LLVM_CLANG_LIB_BASIC_TARGETS_SHUXI_H
