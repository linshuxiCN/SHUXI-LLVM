//===-- SHUXIAsmPrinter.cpp - SHUXI LLVM assembly writer ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the SHUXI assembly language.
//
//===----------------------------------------------------------------------===//

#include "SHUXIAsmPrinter.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineJumpTableInfo.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/CodeGen/TargetLoweringObjectFile.h"
#include <algorithm>
#include <cctype>

using namespace llvm;

SHUXITargetAsmStreamer* SHUXIAsmPrinter::getTargetStreamer() const
{
  if (!OutStreamer)
    return nullptr;
  return static_cast<SHUXITargetAsmStreamer*>(OutStreamer->getTargetStreamer());
}

void SHUXIAsmPrinter::EmitFunctionEntryLabel()
{
  OutStreamer->EmitLabel(CurrentFnSym);
}

void SHUXIAsmPrinter::EmitInstruction(const MachineInstr *MI)
{
  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

void SHUXIAsmPrinter::EmitStartOfAsmFile(Module &M)
{
  getTargetStreamer()->EmitStartOfAsmFile(M);
}

void SHUXIAsmPrinter::EmitEndOfAsmFile(Module &)
{
}

void SHUXIAsmPrinter::EmitFunctionBodyStart()
{
}

void SHUXIAsmPrinter::EmitFunctionBodyEnd()
{
}

void SHUXIAsmPrinter::EmitBasicBlockStart(const MachineBasicBlock &MBB) const
{
  AsmPrinter::EmitBasicBlockStart(MBB);
}

void SHUXIAsmPrinter::EmitBasicBlockEnd(const MachineBasicBlock &MBB)
{
  AsmPrinter::EmitBasicBlockEnd(MBB);
}

// Force static initialization.
extern "C" void LLVMInitializeSHUXIAsmPrinter() {
  RegisterAsmPrinter<SHUXIAsmPrinter> X(getTheSHUXI32Target());
}
