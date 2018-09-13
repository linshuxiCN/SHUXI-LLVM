//===-- SHUXITargetMachine.cpp - Define TargetMachine for SHUXI ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about SHUXI target spec.
//
//===----------------------------------------------------------------------===//

#include "SHUXITargetMachine.h"
#include "MCTargetDesc/SHUXIMCTargetDesc.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options)
{
  // Build the data layout
  // http://llvm.org/docs/LangRef.html#data-layout

  std::string dataLayout = "";

  dataLayout += "e"; // Little-endian
  dataLayout += "-m:e"; // ELF style name mangling
  dataLayout += "-p:32:32"; // Set 32-bit pointer size with 32-bit alignment
  dataLayout += "-i1:8:32"; // Align i1 to a 32-bit word
  dataLayout += "-i8:8:32"; // Align i8 to a 32-bit word
  dataLayout += "-i16:16:32"; // Align i16 to a 32-bit word
  dataLayout += "-i64:32"; // Align i64 to a 32-bit word
  dataLayout += "-f64:32"; // Align f64 to a 32-bit word
  dataLayout += "-a:0:32"; // Align aggregates to a 32-bit word
  dataLayout += "-n32"; // Set native integer width to 32-bits
  
  // "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32-a:0:32-n32"

  return dataLayout;
}

static Reloc::Model getEffectiveRelocModel(const Triple &TT,
                                           Optional<Reloc::Model> RM)
{
  if (!RM.hasValue())
    // Default relocation model on Darwin is PIC.
    return TT.isOSBinFormatMachO() ? Reloc::PIC_ : Reloc::Static;

  // DynamicNoPIC is only used on darwin.
  if (*RM == Reloc::DynamicNoPIC && !TT.isOSDarwin())
    return Reloc::Static;

  return *RM;
}

static CodeModel::Model getEffectiveCodeModel(Optional<CodeModel::Model> CM)
{
  if (CM)
    return *CM;
  return CodeModel::Small;
}

SHUXITargetMachine::SHUXITargetMachine(const Target &T, const Triple &TT,
                                       StringRef CPU, StringRef FS,
                                       const TargetOptions &Options,
                                       Optional<Reloc::Model> RM,
                                       Optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT, bool is64Bit)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options), TT, CPU, FS,
                        Options, getEffectiveRelocModel(TT, RM), getEffectiveCodeModel(CM), OL),
      Subtarget(TT, CPU, FS, *this),
      TLOF(make_unique<TargetLoweringObjectFileELF>()),
      Is64Bit(is64Bit)
{
  initAsmInfo();
}

SHUXI32TargetMachine::SHUXI32TargetMachine(const Target &T, const Triple &TT,
                                           StringRef CPU, StringRef FS,
                                           const TargetOptions &Options,
                                           Optional<Reloc::Model> RM,
                                           Optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT)
    : SHUXITargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT, false)
{
}

SHUXI64TargetMachine::SHUXI64TargetMachine(const Target &T, const Triple &TT,
                                           StringRef CPU, StringRef FS,
                                           const TargetOptions &Options,
                                           Optional<Reloc::Model> RM,
                                           Optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT)
    : SHUXITargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT, true)
{
}


namespace {
class SHUXIPassConfig : public TargetPassConfig {
public:
  SHUXIPassConfig(SHUXITargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  SHUXITargetMachine &getSHUXITargetMachine() const
  {
    return getTM<SHUXITargetMachine>();
  }

  virtual bool addInstSelector() override;
};
}

TargetPassConfig *SHUXITargetMachine::createPassConfig(PassManagerBase &PM)
{
  return new SHUXIPassConfig(*this, PM);
}

bool SHUXIPassConfig::addInstSelector()
{
  addPass(createSHUXIISelDag(getSHUXITargetMachine(), getOptLevel()));
  return false;
}

// Force static initialization.
extern "C" void LLVMInitializeSHUXITarget()
{
  RegisterTargetMachine<SHUXI32TargetMachine> X(getTheSHUXI32Target());
}
