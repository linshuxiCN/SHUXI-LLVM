//===-- SHUXISubtarget.h - Define Subtarget for the SHUXI -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the SHUXI specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef SHUXISUBTARGET_H
#define SHUXISUBTARGET_H

#include "SHUXIFrameLowering.h"
#include "SHUXIISelLowering.h"
#include "SHUXIInstrInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "SHUXIGenSubtargetInfo.inc"

namespace llvm {
class StringRef;

class SHUXISubtarget : public SHUXIGenSubtargetInfo {
  virtual void anchor();

  SHUXIInstrInfo InstrInfo;
  SHUXIFrameLowering FrameLowering;
  SHUXITargetLowering TLInfo;

public:
  // Initializes the data members to match that of the specified triple.
  SHUXISubtarget(const Triple &TT, StringRef CPU,
                 StringRef FS, SHUXITargetMachine &TM);

  // Parses features string setting specified subtarget options. The
  // definition of this function is auto-generated by tblgen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

  const SHUXIFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const SHUXIInstrInfo *getInstrInfo() const override {
  	return &InstrInfo;
  }
  const TargetRegisterInfo *getRegisterInfo() const override {
    return &InstrInfo.getRegisterInfo();
  }
  const SHUXITargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
};
} // End llvm namespace

#endif
