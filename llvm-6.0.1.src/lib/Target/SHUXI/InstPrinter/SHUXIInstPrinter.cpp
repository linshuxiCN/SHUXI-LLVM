//===-- SHUXIInstPrinter.cpp - Convert SHUXI MCInst to asm syntax -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an SHUXI MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "SHUXI.h"
#include "SHUXIInstPrinter.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#include "SHUXIGenAsmWriter.inc"

void SHUXIInstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                                 StringRef Annot, const MCSubtargetInfo &STI) {
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void SHUXIInstPrinter::printRegName(raw_ostream &O, unsigned RegNo) const {
  O << StringRef(getRegisterName(RegNo));
}

void SHUXIInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                    raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);

  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  if (Op.isExpr()) {
    Op.getExpr()->print(O, &MAI);
    return;
  }

  llvm_unreachable("Unknown operand");
}

// Print a memsrc (defined in SHUXIInstrInfo.td)
// This is an operand which defines a location for loading or storing which
// is a register offset by an immediate value
void SHUXIInstPrinter::printMemSrcOperand(const MCInst *MI, unsigned OpNo,
                                          raw_ostream &O) {
  const MCOperand &BaseAddr = MI->getOperand(OpNo);
  const MCOperand &Offset = MI->getOperand(OpNo + 1);
  
  assert(Offset.isImm() && "Expected immediate in displacement field");

  O << "M[";
  printRegName(O, BaseAddr.getReg());
  unsigned OffsetVal = Offset.getImm();
  if (OffsetVal) {
    O << "+" << Offset.getImm();
  }
  O << "]";
}

void SHUXIInstPrinter::printPCRelImmOperand(const MCInst *MI, unsigned OpNo,
                                            raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isImm())
    O << Op.getImm();
  else {
    assert(Op.isExpr() && "unknown pcrel immediate operand");
    Op.getExpr()->print(O, &MAI);
  }
}

void SHUXIInstPrinter::printCCOperand(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &O) {
  unsigned CC = MI->getOperand(OpNo).getImm();

  switch (CC) {
  default:
   llvm_unreachable("Unsupported CC code");
  case SHUXICC::COND_C:
   O << "c";
   break;
  case SHUXICC::COND_N:
   O << "n";
   break;
  case SHUXICC::COND_V:
   O << "v";
   break;
  case SHUXICC::COND_Z:
   O << "eq";
   break;
  case SHUXICC::COND_NC:
   O << "nc";
   break;
  case SHUXICC::COND_NN:
   O << "nn";
   break;
  case SHUXICC::COND_NV:
   O << "nv";
   break;
  case SHUXICC::COND_NZ:
   O << "ne";
   break;
  case SHUXICC::COND_GE:
   O << "ge";
   break;
  case SHUXICC::COND_L:
   O << "l";
   break;
  }
}
