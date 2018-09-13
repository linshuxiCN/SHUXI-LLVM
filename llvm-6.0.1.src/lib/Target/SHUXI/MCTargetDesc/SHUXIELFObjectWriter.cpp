//===-- SHUXIELFObjectWriter.cpp - SHUXI ELF Writer ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/SHUXIMCTargetDesc.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
  class SHUXIELFObjectWriter : public MCELFObjectTargetWriter {
  public:
    SHUXIELFObjectWriter(uint8_t OSABI);

    virtual ~SHUXIELFObjectWriter();

    unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                          const MCFixup &Fixup, bool IsPCRel) const override;
    };
}

SHUXIELFObjectWriter::SHUXIELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit*/ false, OSABI,
                              /*ELF::EM_SHUXI*/ ELF::EM_SHUXI32,
                              /*HasRelocationAddend*/ false) {}

SHUXIELFObjectWriter::~SHUXIELFObjectWriter() {}

unsigned SHUXIELFObjectWriter::getRelocType(MCContext &Ctx,
                                            const MCValue &Target,
                                            const MCFixup &Fixup,
                                            bool IsPCRel) const {
  llvm_unreachable("SHUXIELFObjectWriter: invalid fixup kind");
}

std::unique_ptr<MCObjectWriter>
llvm::createSHUXIELFObjectWriter(raw_pwrite_stream &OS, bool Is64Bit,
  bool IsLittleEndian, uint8_t OSABI) {
  auto MOTW = llvm::make_unique<SHUXIELFObjectWriter>(OSABI);
  return createELFObjectWriter(std::move(MOTW), OS, IsLittleEndian);
}
