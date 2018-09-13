//===-- SHUXIAsmPrinter.h - Asm printer for SHUXI Target ---------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class implements SHUXI-specific bits of SHUXIAsmPrinter class.
//
//===----------------------------------------------------------------------===//

#ifndef SHUXIASMPRINTER_H
#define SHUXIASMPRINTER_H

#include "SHUXI.h"
#include "InstPrinter/SHUXIInstPrinter.h"
#include "MCTargetDesc/SHUXITargetStreamer.h"
#include "SHUXIInstrInfo.h"
#include "SHUXIMCInstLower.h"
#include "SHUXISubtarget.h"
#include "SHUXITargetMachine.h"
#include "llvm/CodeGen/AsmPrinter.h"

using namespace llvm;

namespace llvm {
  class SHUXIAsmPrinter : public AsmPrinter {
    SHUXIMCInstLower MCInstLowering;

  public:
    explicit SHUXIAsmPrinter(TargetMachine &TM,
      std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)),
      MCInstLowering(OutContext, *this) {}

    virtual StringRef getPassName() const
    {
      return StringRef("SHUXI Assembly Printer");
    }

    SHUXITargetAsmStreamer* getTargetStreamer() const;

    //void EmitBasicBlockStart(const MachineBasicBlock &MBB) const;
    void EmitFunctionEntryLabel();
    void EmitInstruction(const MachineInstr *MI);

    /// This virtual method can be overridden by targets that want to emit
    /// something at the start of their file.
    void EmitStartOfAsmFile(Module &);

    /// This virtual method can be overridden by targets that want to emit
    /// something at the end of their file.
    void EmitEndOfAsmFile(Module &);

    /// Targets can override this to emit stuff before the first basic block in
    /// the function.
    void EmitFunctionBodyStart();

    /// Targets can override this to emit stuff after the last basic block in the
    /// function.
    void EmitFunctionBodyEnd();

    /// Targets can override this to emit stuff at the start of a basic block.
    /// By default, this method prints the label for the specified
    /// MachineBasicBlock, an alignment (if present) and a comment describing it
    /// if appropriate.
    void EmitBasicBlockStart(const MachineBasicBlock &MBB) const;

    /// Targets can override this to emit stuff at the end of a basic block.
    void EmitBasicBlockEnd(const MachineBasicBlock &MBB);
  };
} // end of llvm

#endif
