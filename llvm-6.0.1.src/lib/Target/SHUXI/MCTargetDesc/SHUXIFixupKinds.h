//===-- SHUXIFixupKinds.h - SHUXI Specific Fixup Entries ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef SHUXIFIXUPKINDS_H
#define SHUXIFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace SHUXI {

  // This table *must* be in the same order of
  // MCFixupKindInfo Infos[SHUXI::NumTargetFixupKinds]
  // in SHUXIAsmBackend.cpp.
  enum Fixups {
    fixup_SHUXI_NONE = FirstTargetFixupKind,

    // Pure 32 but data fixup
    fixup_SHUXI_32,

    // Marker
    LastTargetFixupKind,
    NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
  };
} // namespace SHUXI
} // namespace llvm


#endif
