//===-- SHUXIInstrInfo.cpp - SHUXI Instruction Information ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the SHUXI implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "SHUXIInstrInfo.h"
#include "SHUXI.h"
#include "SHUXISubtarget.h"
#include "SHUXITargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "SHUXIGenInstrInfo.inc"

using namespace llvm;

// Pin the vtable to this file.
void SHUXIInstrInfo::anchor() {}

SHUXIInstrInfo::SHUXIInstrInfo() : SHUXIGenInstrInfo(), RI() {}

void SHUXIInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator I,
                                  const DebugLoc &DL, unsigned DestReg,
                                  unsigned SrcReg, bool KillSrc) const {

  if (SHUXI::GPRegsRegClass.contains(DestReg, SrcReg)) {
      BuildMI(MBB, I, DL, get(SHUXI::CPYrr), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc));
  }
  else {
    llvm_unreachable("Impossible reg-to-reg copy");
  }
}

void SHUXIInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                       MachineBasicBlock::iterator MI,
                                       unsigned SrcReg, bool isKill,
                                       int FrameIdx,
                                       const TargetRegisterClass *RC,
                                       const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  BuildMI(MBB, MI, DL, get(SHUXI::PUSH))
    .addReg(SrcReg, getKillRegState(isKill));
}

void SHUXIInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator MI,
                                        unsigned DestReg, int FrameIdx,
                                        const TargetRegisterClass *RC,
                                        const TargetRegisterInfo *TRI) const{
  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  BuildMI(MBB, MI, DL, get(SHUXI::POP))
    .addReg(DestReg, getDefRegState(true));
}

bool SHUXIInstrInfo::
reverseBranchCondition(SmallVectorImpl<MachineOperand> &Cond) const {
  assert(Cond.size() == 1 && "Invalid Xbranch condition!");

  SHUXICC::CondCodes CC = static_cast<SHUXICC::CondCodes>(Cond[0].getImm());

  switch (CC) {
  default: llvm_unreachable("Invalid branch condition!");
  
  case SHUXICC::COND_C:
    CC = SHUXICC::COND_NC;
    break;
  case SHUXICC::COND_N:
    CC = SHUXICC::COND_NN;
    break;
  case SHUXICC::COND_V:
    CC = SHUXICC::COND_NV;
    break;
  case SHUXICC::COND_Z:
    CC = SHUXICC::COND_NZ;
    break;
  case SHUXICC::COND_GE:
    CC = SHUXICC::COND_L;
    break;
  case SHUXICC::COND_NC:
    CC = SHUXICC::COND_C;
    break;
  case SHUXICC::COND_NN:
    CC = SHUXICC::COND_N;
    break;
  case SHUXICC::COND_NV:
    CC = SHUXICC::COND_V;
    break;
  case SHUXICC::COND_NZ:
    CC = SHUXICC::COND_Z;
    break;
  case SHUXICC::COND_L:
    CC = SHUXICC::COND_GE;
    break;
  }

  Cond[0].setImm(CC);
  return false;
}

bool SHUXIInstrInfo::isUnpredicatedTerminator(const MachineInstr &MI) const {
  if (!MI.isTerminator())
    return false;

  // Conditional branch is a special case.
  if (MI.isBranch() && !MI.isBarrier())
    return true;
  if (!MI.isPredicable())
    return true;
  return !isPredicated(MI);
}

bool SHUXIInstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                 MachineBasicBlock *&TBB,
                                 MachineBasicBlock *&FBB,
                                 SmallVectorImpl<MachineOperand> &Cond,
                                 bool AllowModify) const {
  // Start from the bottom of the block and work up, examining the
  // terminator instructions.
  MachineBasicBlock::iterator I = MBB.end();
  while (I != MBB.begin()) {
    --I;
    if (I->isDebugValue())
      continue;

    // Working from the bottom, when we see a non-terminator
    // instruction, we're done.
    if (!isUnpredicatedTerminator(*I))
      break;

    // A terminator that isn't a branch can't easily be handled
    // by this analysis.
    if (!I->isBranch())
      return true;

    // Handle unconditional branches.
    if (I->getOpcode() == SHUXI::JMP) {
      if (!AllowModify) {
        TBB = I->getOperand(0).getMBB();
        continue;
      }

      // If the block has any instructions after a JMP, delete them.
      while (std::next(I) != MBB.end())
        std::next(I)->eraseFromParent();
      Cond.clear();
      FBB = nullptr;

      // Delete the JMP if it's equivalent to a fall-through.
      if (MBB.isLayoutSuccessor(I->getOperand(0).getMBB())) {
        TBB = nullptr;
        I->eraseFromParent();
        I = MBB.end();
        continue;
      }

      // TBB is used to indicate the unconditinal destination.
      TBB = I->getOperand(0).getMBB();
      continue;
    }

    // Handle conditional branches.
    assert(I->getOpcode() == SHUXI::JCC && "Invalid conditional branch");
    SHUXICC::CondCodes BranchCode =
      static_cast<SHUXICC::CondCodes>(I->getOperand(1).getImm());
    if (BranchCode == SHUXICC::COND_INVALID)
      return true;  // Can't handle weird stuff.

    // Working from the bottom, handle the first conditional branch.
    if (Cond.empty()) {
      FBB = TBB;
      TBB = I->getOperand(0).getMBB();
      Cond.push_back(MachineOperand::CreateImm(BranchCode));
      continue;
    }

    // Handle subsequent conditional branches. Only handle the case where all
    // conditional branches branch to the same destination.
    assert(Cond.size() == 1);
    assert(TBB);

    // Only handle the case where all conditional branches branch to
    // the same destination.
    if (TBB != I->getOperand(0).getMBB())
      return true;

    SHUXICC::CondCodes OldBranchCode = (SHUXICC::CondCodes)Cond[0].getImm();
    // If the conditions are the same, we can leave them alone.
    if (OldBranchCode == BranchCode)
      continue;

    return true;
  }

  return false;
}

unsigned SHUXIInstrInfo::removeBranch(MachineBasicBlock &MBB,
                                    int *BytesRemoved) const {
  assert(!BytesRemoved && "code size not handled");

  MachineBasicBlock::iterator I = MBB.end();
  unsigned Count = 0;

  while (I != MBB.begin()) {
    --I;
    if (I->isDebugValue())
      continue;
    if (I->getOpcode() != SHUXI::JMP &&
        I->getOpcode() != SHUXI::JCC
        //I->getOpcode() != MSP430::Br &&
        //I->getOpcode() != MSP430::Bm
        )
      break;
    // Remove the branch.
    I->eraseFromParent();
    I = MBB.end();
    ++Count;
  }

  return Count;
}

unsigned SHUXIInstrInfo::insertBranch(MachineBasicBlock &MBB,
                                       MachineBasicBlock *TBB,
                                       MachineBasicBlock *FBB,
                                       ArrayRef<MachineOperand> Cond,
                                       const DebugLoc &DL,
                                       int *BytesAdded) const {
  // Shouldn't be a fall through.
  assert(TBB && "insertBranch must not be told to insert a fallthrough");
  assert((Cond.size() == 1 || Cond.size() == 0) &&
         "SHUXI branch conditions have one component!");
  assert(!BytesAdded && "code size not handled");

  if (Cond.empty()) {
    // Unconditional branch?
    assert(!FBB && "Unconditional branch with multiple successors!");
    BuildMI(&MBB, DL, get(SHUXI::JMP)).addMBB(TBB);
    return 1;
  }

  // Conditional branch.
  unsigned Count = 0;
  BuildMI(&MBB, DL, get(SHUXI::JCC)).addMBB(TBB).addImm(Cond[0].getImm());
  ++Count;

  if (FBB) {
    // Two-way Conditional branch. Insert the second branch.
    BuildMI(&MBB, DL, get(SHUXI::JMP)).addMBB(FBB);
    ++Count;
  }
  return Count;
}
