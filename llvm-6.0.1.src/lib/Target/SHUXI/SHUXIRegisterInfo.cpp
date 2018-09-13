//===-- SHUXIRegisterInfo.cpp - SHUXI Register Information ----------*- C++ -*-===//
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

#include "SHUXIRegisterInfo.h"
#include "SHUXI.h"
#include "SHUXISubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_REGINFO_TARGET_DESC
#include "SHUXIGenRegisterInfo.inc"

using namespace llvm;

SHUXIRegisterInfo::SHUXIRegisterInfo() : SHUXIGenRegisterInfo(SHUXI::PC) {}

const MCPhysReg *
SHUXIRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const
{
  // from tablegen
  return CC_Save_SaveList;
}

const uint32_t *
SHUXIRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                        CallingConv::ID CC) const
{
  // from tablegen
  return CC_Save_RegMask;
}

BitVector SHUXIRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(SHUXI::SR); // status regsiter
  Reserved.set(SHUXI::PC); // program counter
  Reserved.set(SHUXI::SP); // stack pointer
  
  return Reserved;
}

const TargetRegisterClass *
SHUXIRegisterInfo::getPointerRegClass(const MachineFunction &MF, unsigned Kind)
                                                                         const {
  return &SHUXI::GPRegsRegClass;
}

void SHUXIRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  const MachineFunction &MF = *MBB.getParent();
  const MachineFrameInfo MFI = MF.getFrameInfo();
  MachineOperand &FIOp = MI.getOperand(FIOperandNum);
  unsigned FI = FIOp.getIndex();
    
  // Determine if we can eliminate the index from this kind of instruction.
  unsigned ImmOpIdx = 0;
  switch (MI.getOpcode()) {
  default:
    llvm_unreachable("Instruction not supported");
  case SHUXI::LD:
  case SHUXI::ST:
  case SHUXI::ADDri:
    ImmOpIdx = FIOperandNum + 1;
    break;
  }
    
  // FIXME: check the size of offset.
  MachineOperand &ImmOp = MI.getOperand(ImmOpIdx);
  int Offset = MFI.getObjectOffset(FI) + MFI.getStackSize() + ImmOp.getImm();
  if (Offset % 4) {
    llvm_unreachable("Offset must be aligned to 4 bytes because memory is "
                     "32-bit word addressable only");
  }
  FIOp.ChangeToRegister(SHUXI::SP, false);
  ImmOp.setImm(Offset);
}

unsigned SHUXIRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return SHUXI::SP;
}
