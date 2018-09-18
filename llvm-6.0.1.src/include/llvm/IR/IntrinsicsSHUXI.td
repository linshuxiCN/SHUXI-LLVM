//===- IntrinsicsSHUXI.td - Defines SHUXI intrinsics ---------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines all of the SHUXI-specific intrinsics for use with NVPTX.
//
//===----------------------------------------------------------------------===//

//
// MISC
//

let TargetPrefix = "shuxi" in {

//
// TEST
//

  def int_shuxi_test_i :
      Intrinsic<[llvm_i32_ty], [llvm_i32_ty, llvm_i32_ty],
        [IntrNoMem]>;

} // let TargetPrefix = "shuxi"
