//===-- SHUXITargetStreamer.h - SHUXI Target Streamer --------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SHUXI_MCTARGETDESC_SHUXITARGETSTREAMER_H
#define LLVM_LIB_TARGET_SHUXI_MCTARGETDESC_SHUXITARGETSTREAMER_H

#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"

namespace llvm {

class DataLayout;
class Function;
class MCELFStreamer;
class MCSymbol;
class MDNode;
class Module;
class Type;

//===----------------------------------------------------------------------===//
// Base target streamer.
//===----------------------------------------------------------------------===//
class SHUXITargetStreamer : public MCTargetStreamer {

protected:
  MCContext &getContext() const { return Streamer.getContext(); }

public:
  SHUXITargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}
};

//===----------------------------------------------------------------------===//
// TargetStreamer for ASM.
//===----------------------------------------------------------------------===//
class SHUXITargetAsmStreamer final : public SHUXITargetStreamer {
  formatted_raw_ostream &OS;

public:
  SHUXITargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);

public:
  void EmitStartOfAsmFile(Module &M);
};

//===----------------------------------------------------------------------===//
// TargetStreamer for ELF.
//===----------------------------------------------------------------------===//
class SHUXITargetELFStreamer final : public SHUXITargetStreamer {
  MCStreamer &Streamer;

public:
  SHUXITargetELFStreamer(MCStreamer &S);

  MCELFStreamer &getStreamer();
};
}
#endif
