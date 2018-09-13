//===-- SHUXISubtarget.cpp - SHUXI Subtarget Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the SHUXI specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "SHUXI.h"
#include "SHUXIFrameLowering.h"
#include "SHUXISubtarget.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "shuxi-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "SHUXIGenSubtargetInfo.inc"

void SHUXISubtarget::anchor() {}

SHUXISubtarget::SHUXISubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                               SHUXITargetMachine &TM)
    : SHUXIGenSubtargetInfo(TT, CPU, FS), InstrInfo(), FrameLowering(*this),
      TLInfo(TM, *this) {}
