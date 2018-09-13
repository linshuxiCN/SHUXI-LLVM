//===-- SHUXIRegisterInfo.h - SHUXI Register Information Impl ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the SHUXI implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef SHUXIREGISTERINFO_H
#define SHUXIREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "SHUXIGenRegisterInfo.inc"

namespace llvm {

class TargetInstrInfo;

struct SHUXIRegisterInfo : public SHUXIGenRegisterInfo {

  SHUXIRegisterInfo();

  const uint16_t *getCalleeSavedRegs(const MachineFunction *MF = 0) const override;

  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID CC) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  const TargetRegisterClass*
  getPointerRegClass(const MachineFunction &MF,
                     unsigned Kind = 0) const override;

  void eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  unsigned getFrameRegister(const MachineFunction &MF) const override;
};
}

#endif
