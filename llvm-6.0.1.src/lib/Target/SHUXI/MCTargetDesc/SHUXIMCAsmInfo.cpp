//===-- SHUXIMCAsmInfo.cpp - SHUXI asm properties -----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "SHUXIMCAsmInfo.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Triple.h"
using namespace llvm;

void SHUXIMCAsmInfo::anchor() {}

SHUXIMCAsmInfo::SHUXIMCAsmInfo(const Triple &TT) {
  CommentString = "//";
  SupportsDebugInformation = true;
  PrivateLabelPrefix = "";
}
