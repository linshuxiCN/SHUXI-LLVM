//===-- SHUXIISelDAGToDAG.cpp - A dag to dag inst selector for SHUXI ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the SHUXI target.
//
//===----------------------------------------------------------------------===//

#include "SHUXI.h"
#include "SHUXITargetMachine.h"
#include "MCTargetDesc/SHUXIMCTargetDesc.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/IR/Function.h" // To access function attributes.
#include "llvm/Support/Debug.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "shuxi-isel"

// SHUXI-specific code to select SHUXI machine instructions for
// SelectionDAG operations.
namespace {
class SHUXIDAGToDAGISel : public SelectionDAGISel {

public:
  explicit SHUXIDAGToDAGISel(SHUXITargetMachine &TM, CodeGenOpt::Level OptLevel)
      : SelectionDAGISel(TM, OptLevel) {}

  StringRef getPassName() const override {
    return "SHUXI DAG->DAG Pattern Instruction Selection";
  }

  void Select(SDNode *Node) override;

  bool SelectAddr(SDValue Addr, SDValue &BaseAddr, SDValue &Offset);

// Include the pieces autogenerated from the target description.
#include "SHUXIGenDAGISel.inc"
};
} // end anonymous namespace

void SHUXIDAGToDAGISel::Select(SDNode *Node) {
  // Dump information about the Node being selected.
  DEBUG(errs() << "Selecting: "; Node->dump(CurDAG); errs() << "\n");

  // If we have a custom node, we have already selected
  if (Node->isMachineOpcode()) {
    DEBUG(errs() << "== "; Node->dump(CurDAG); errs() << "\n");
    Node->setNodeId(-1);
    return;
  }

  SDLoc dl(Node);

  // Few custom selection stuff.
  switch (Node->getOpcode()) {
    default: break;
    case ISD::FrameIndex: {
      assert(Node->getValueType(0) == MVT::i32);
      int FI = cast<FrameIndexSDNode>(Node)->getIndex();
      SDValue TFI = CurDAG->getTargetFrameIndex(FI, MVT::i32);
      if (Node->hasOneUse()) {
        CurDAG->SelectNodeTo(Node, SHUXI::ADDri, MVT::i32, TFI,
                             CurDAG->getTargetConstant(0, dl, MVT::i32));
        return;
      } // else
      ReplaceNode(Node, CurDAG->getMachineNode(
                  SHUXI::ADDri, dl, MVT::i32, TFI,
                  CurDAG->getTargetConstant(0, dl, MVT::i32)));
      return;
    }
  }

  // Select the default instruction.
  SelectCode(Node);
}

/// SelectAddr - returns true if it is able pattern match an addressing mode.
/// It returns the operands which make up the maximal addressing mode it can
/// match by reference.
bool SHUXIDAGToDAGISel::SelectAddr(SDValue Addr,
                                 SDValue &BaseAddr, SDValue &Offset) {
  if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    EVT PtrVT = getTargetLowering()->getPointerTy(CurDAG->getDataLayout());
    BaseAddr = CurDAG->getTargetFrameIndex(FIN->getIndex(), PtrVT);
    Offset = CurDAG->getTargetConstant(0, Addr, MVT::i32);
    return true;
  }
  else {
    BaseAddr = Addr;
  }
  
  if (Addr.getOpcode() == ISD::TargetExternalSymbol ||
      Addr.getOpcode() == ISD::TargetGlobalAddress ||
      Addr.getOpcode() == ISD::TargetGlobalTLSAddress) {
    return false; // direct calls.
  }

  if (CurDAG->isBaseWithConstantOffset(Addr)) {
    ConstantSDNode *CN = dyn_cast<ConstantSDNode>(Addr.getOperand(1));
    Offset = CurDAG->getTargetConstant(CN->getZExtValue(), Addr, MVT::i32);
  }
  else {
    Offset = CurDAG->getTargetConstant(0, Addr, MVT::i32);
  }
  return true;
}

/// createSHUXIISelDag - This pass converts a legalized DAG into a
/// SHUXI-specific DAG, ready for instruction scheduling.
///
FunctionPass *llvm::createSHUXIISelDag(SHUXITargetMachine &TM,
                                     CodeGenOpt::Level OptLevel) {
  return new SHUXIDAGToDAGISel(TM, OptLevel);
}
