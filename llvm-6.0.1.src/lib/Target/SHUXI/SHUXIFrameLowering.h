//===-- SHUXIFrameLowering.h - Frame info for SHUXI Target ---------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class implements SHUXI-specific bits of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#ifndef SHUXIFRAMEINFO_H
#define SHUXIFRAMEINFO_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class SHUXISubtarget;

class SHUXIFrameLowering : public TargetFrameLowering {
public:
  SHUXIFrameLowering(const SHUXISubtarget &STI);

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

  bool spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 const std::vector<CalleeSavedInfo> &CSI,
                                 const TargetRegisterInfo *TRI) const override;
  
  bool restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator MI,
                                   std::vector<CalleeSavedInfo> &CSI,
                                   const TargetRegisterInfo *TRI) const override;

  bool hasFP(const MachineFunction &MF) const override;

  bool hasReservedCallFrame(const MachineFunction &MF) const override;
  
  void processFunctionBeforeFrameFinalized(MachineFunction &MF,
                                           RegScavenger *RS = nullptr) const override;

  static int stackSlotSize() { return 4; } // 4 Bytes?
};
}

#endif
