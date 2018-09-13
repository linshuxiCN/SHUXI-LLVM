//===-- SHUXIMCAsmInfo.h - SHUXI asm properties --------------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the SHUXIMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef SHUXITARGETASMINFO_H
#define SHUXITARGETASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class SHUXIMCAsmInfo : public MCAsmInfoELF {
  virtual void anchor();

public:
  explicit SHUXIMCAsmInfo(const Triple &TT);
};

} // namespace llvm

#endif
