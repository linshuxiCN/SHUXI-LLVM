//===-- SHUXITargetMachine.h - Define TargetMachine for SHUXI -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the SHUXI specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef SHUXITARGETMACHINE_H
#define SHUXITARGETMACHINE_H

#include "SHUXI.h"
#include "SHUXISubtarget.h"
#include "MCTargetDesc/SHUXIMCTargetDesc.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class SHUXITargetMachine : public LLVMTargetMachine {
  SHUXISubtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  bool Is64Bit;

public:
  SHUXITargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, const TargetOptions &Options,
                     Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                     CodeGenOpt::Level OL, bool JIT, bool is64Bit);

  // Pass Pipeline Configuration
  virtual TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  
  TargetLoweringObjectFile *getObjFileLowering() const override
  {
    return TLOF.get();
  }

  bool Is64BitTarget()
  {
    return Is64Bit;
  }

  virtual const TargetSubtargetInfo *
  getSubtargetImpl(const Function &) const override
  {
    return &Subtarget;
  }
}; // end namespace SHUXITargetMachine

/// SHUXI 32-bit target machine
///
class SHUXI32TargetMachine : public SHUXITargetMachine {
public:
  SHUXI32TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                       StringRef FS, const TargetOptions &Options,
                       Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                       CodeGenOpt::Level OL, bool JIT);
}; // end namespace SHUXI32TargetMachine

/// SHUXI 64-bit target machine
///
class SHUXI64TargetMachine : public SHUXITargetMachine {
public:
  SHUXI64TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                       StringRef FS, const TargetOptions &Options,
                       Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                       CodeGenOpt::Level OL, bool JIT);
}; // end namespace SHUXI64TargetMachine

} // end namespace llvm

#endif
