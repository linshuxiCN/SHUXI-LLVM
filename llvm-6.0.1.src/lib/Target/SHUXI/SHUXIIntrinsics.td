//===- SHUXIIntrinsics.td - BI Intrinsics Instructions -------*- tblgen -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//-----------------------------------
// TEST Functions
//-----------------------------------
def : Pat<(int_shuxi_test_i GPRegs:$a, GPRegs:$b),
          (XORrr GPRegs:$a, GPRegs:$b)>;
