//===-- SHUXIMCTargetDesc.h - SHUXI Target Descriptions -------------*- C++ -*-===//
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

#ifndef SHUXIMCTARGETDESC_H
#define SHUXIMCTARGETDESC_H

#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {
class Target;
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectWriter;
class MCRegisterInfo;
class MCSubtargetInfo;

class StringRef;
class raw_ostream;
class raw_pwrite_stream;
class Triple;

Target &getTheSHUXI32Target();

MCCodeEmitter *createSHUXIMCCodeEmitter(const MCInstrInfo &MCII,
                                        const MCRegisterInfo &MRI,
                                        MCContext &Ctx);
MCAsmBackend *createSHUXIAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);
std::unique_ptr<MCObjectWriter>
createSHUXIELFObjectWriter(raw_pwrite_stream &OS, bool Is64Bit,
                           bool IsLIttleEndian, uint8_t OSABI);
} // End llvm namespace

// Defines symbolic names for SHUXI registers.
#define GET_REGINFO_ENUM
#include "SHUXIGenRegisterInfo.inc"

// Defines symbolic names for SHUXI instructions.
#define GET_INSTRINFO_ENUM
#include "SHUXIGenInstrInfo.inc"

#endif
