//===-- SHUXIMCTargetDesc.cpp - SHUXI Target Descriptions ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides SHUXI specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "SHUXIMCTargetDesc.h"
#include "SHUXIMCAsmInfo.h"
#include "InstPrinter/SHUXIInstPrinter.h"
#include "SHUXITargetStreamer.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "SHUXIGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "SHUXIGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "SHUXIGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createSHUXIMCInstrInfo()
{
  MCInstrInfo *X = new MCInstrInfo();
  InitSHUXIMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createSHUXIMCRegisterInfo(const Triple &TT)
{
  MCRegisterInfo *X = new MCRegisterInfo();
  InitSHUXIMCRegisterInfo(X, SHUXI::PC);
  return X;
}

static MCSubtargetInfo *
createSHUXIMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS)
{
  return createSHUXIMCSubtargetInfoImpl(TT, CPU, FS);
}

static MCAsmInfo *createSHUXIMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT)
{ 
  return new SHUXIMCAsmInfo(TT);
}

static MCInstPrinter *
createSHUXIMCInstPrinter(const Triple &TT, unsigned SyntaxVariant,
                         const MCAsmInfo &MAI, const MCInstrInfo &MII,
                         const MCRegisterInfo &MRI)
{
  return new SHUXIInstPrinter(MAI, MII, MRI);
}

static MCTargetStreamer *
createSHUXITargetAsmStreamer(MCStreamer &S,
                             formatted_raw_ostream &OS,
                             MCInstPrinter *InstPrint, bool isVerboseAsm)
{
  return new SHUXITargetAsmStreamer(S, OS);
}

extern "C" void LLVMInitializeSHUXITargetMC() {
  Target & T = getTheSHUXI32Target();

  // Register the MC asm info.
  RegisterMCAsmInfoFn X(T, createSHUXIMCAsmInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(T, createSHUXIMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(T, createSHUXIMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(T, createSHUXIMCSubtargetInfo);

  // Register the asm backend.
  TargetRegistry::RegisterMCAsmBackend(T, createSHUXIAsmBackend);

  // Register the MC Code Emitter
  TargetRegistry::RegisterMCCodeEmitter(T, createSHUXIMCCodeEmitter);

  // Register the asm target streamer.
  TargetRegistry::RegisterAsmTargetStreamer(T, createSHUXITargetAsmStreamer);

  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(T, createSHUXIMCInstPrinter);
}
