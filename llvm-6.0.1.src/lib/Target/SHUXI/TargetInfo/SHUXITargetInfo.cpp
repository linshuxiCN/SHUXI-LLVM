//===-- SHUXITargetInfo.cpp - SHUXI Target Implementation ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/SHUXIMCTargetDesc.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheSHUXI32Target() {
  static Target TheSHUXI32Target;
  return TheSHUXI32Target;
}

extern "C" void LLVMInitializeSHUXITargetInfo() {
  RegisterTarget<Triple::shuxi32> X(getTheSHUXI32Target(), "shuxi32", "Test BE for shuxi", "SHUXI");
}
