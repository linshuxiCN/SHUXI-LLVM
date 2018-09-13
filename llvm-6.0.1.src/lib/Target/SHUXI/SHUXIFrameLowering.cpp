//===-- SHUXIFrameLowering.cpp - SHUXI Frame Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the SHUXI implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "SHUXIFrameLowering.h"
#include "SHUXIInstrInfo.h"
#include "SHUXIMachineFunctionInfo.h"
#include "SHUXISubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

SHUXIFrameLowering::SHUXIFrameLowering(const SHUXISubtarget &STI)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsUp, 
                          /*StackAlignment=*/4,
                          /*LocalAreaOffset=*/0) {}

bool SHUXIFrameLowering::hasFP(const MachineFunction &MF) const { return true; }

void SHUXIFrameLowering::emitPrologue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {}

void SHUXIFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {}

MachineBasicBlock::iterator
SHUXIFrameLowering::eliminateCallFramePseudoInstr(MachineFunction &MF,
												MachineBasicBlock &MBB,
    											MachineBasicBlock::iterator I)
																		 const {
  const SHUXIInstrInfo &TII =
      *static_cast<const SHUXIInstrInfo *>(MF.getSubtarget().getInstrInfo());
  unsigned StackAlign = getStackAlignment();

  if (!hasReservedCallFrame(MF)) {
    // If the stack pointer can be changed after prologue, turn the
    // adjcallstackup instruction into a 'sub SP, <amt>' and the
    // adjcallstackdown instruction into 'add SP, <amt>'
    // TODO: consider using push / pop instead of sub + store / add
    MachineInstr &Old = *I;
    uint64_t Amount = Old.getOperand(0).getImm();
    if (Amount != 0) {
      // We need to keep the stack aligned properly.  To do this, we round the
      // amount of space needed for the outgoing arguments up to the next
      // alignment boundary.
      Amount = (Amount+StackAlign-1)/StackAlign*StackAlign;

      MachineInstr *New = nullptr;
      if (Old.getOpcode() == TII.getCallFrameSetupOpcode()) {
        New =
            BuildMI(MF, Old.getDebugLoc(), TII.get(SHUXI::SUBri), SHUXI::SP)
                .addReg(SHUXI::SP)
                .addImm(Amount);
      } else {
        assert(Old.getOpcode() == TII.getCallFrameDestroyOpcode());
        // factor out the amount the callee already popped.
        uint64_t CalleeAmt = Old.getOperand(1).getImm();
        Amount -= CalleeAmt;
        if (Amount)
          New = BuildMI(MF, Old.getDebugLoc(), TII.get(SHUXI::ADDri),
                        SHUXI::SP)
                    .addReg(SHUXI::SP)
                    .addImm(Amount);
      }

      if (New) {
        // The SRW implicit def is dead.
        New->getOperand(3).setIsDead();

        // Replace the pseudo instruction with a new instruction...
        MBB.insert(I, New);
      }
    }
  } else if (I->getOpcode() == TII.getCallFrameDestroyOpcode()) {
    // If we are performing frame pointer elimination and if the callee pops
    // something off the stack pointer, add it back.
    if (uint64_t CalleeAmt = I->getOperand(1).getImm()) {
      MachineInstr &Old = *I;
      MachineInstr *New =
          BuildMI(MF, Old.getDebugLoc(), TII.get(SHUXI::SUBri), SHUXI::SP)
              .addReg(SHUXI::SP)
              .addImm(CalleeAmt);
      // The SRW implicit def is dead.
      New->getOperand(3).setIsDead();

      MBB.insert(I, New);
    }
  }

  return MBB.erase(I);
}

bool
SHUXIFrameLowering::spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                            MachineBasicBlock::iterator MI,
                                        const std::vector<CalleeSavedInfo> &CSI,
                                        const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  MachineFunction &MF = *MBB.getParent();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  SHUXIMachineFunctionInfo *MFI = MF.getInfo<SHUXIMachineFunctionInfo>();
  MFI->setCalleeSavedFrameSize(CSI.size() * 2);

  for (unsigned i = CSI.size(); i != 0; --i) {
    unsigned Reg = CSI[i-1].getReg();
    // Add the callee-saved register as live-in. It's killed at the spill.
    MBB.addLiveIn(Reg);
    BuildMI(MBB, MI, DL, TII.get(SHUXI::PUSH))
      .addReg(Reg, RegState::Kill);
  }
  return true;
}

bool
SHUXIFrameLowering::restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                                                MachineBasicBlock::iterator MI,
                                                std::vector<CalleeSavedInfo> &CSI,
                                                const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  MachineFunction &MF = *MBB.getParent();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

  for (unsigned i = 0, e = CSI.size(); i != e; ++i)
    BuildMI(MBB, MI, DL, TII.get(SHUXI::POP), CSI[i].getReg());

  return true;
}

bool SHUXIFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  return !MF.getFrameInfo().hasVarSizedObjects();
}

void SHUXIFrameLowering::processFunctionBeforeFrameFinalized(MachineFunction &MF,
                                                         RegScavenger *) const {
  // Create a frame entry for the FP register that must be saved.
  if (hasFP(MF)) {
    int FrameIdx = MF.getFrameInfo().CreateFixedObject(4, -4, true);
    (void)FrameIdx;
    assert(FrameIdx == MF.getFrameInfo().getObjectIndexBegin() &&
           "Slot for FP register must be last in order to be found!");
  }
}
