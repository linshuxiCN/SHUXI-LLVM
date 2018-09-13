//===-- SHUXIRegisterInfo.td - SHUXI Register defs -------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the SHUXI register file 
//===----------------------------------------------------------------------===//

// General register definition
class SHUXIReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "SHUXI";
}

// Define all CPU registers

// Special purpose registers
def SR : SHUXIReg<0, "r0">;
def PC : SHUXIReg<1, "r1">;
def SP : SHUXIReg<2, "r2">;

// General purpose registers
foreach i = 3-31 in {
  def R#i : SHUXIReg< #i, "r"##i>;
}

def StatusReg : RegisterClass<"SHUXI", [i32], 32, (add
	SR // R0
)>;

def ProgramCounterReg : RegisterClass<"SHUXI", [i32], 32, (add
	PC //R1
)>;

// General purpose registers class
def GPRegs : RegisterClass<"SHUXI", [i32], 32, (add
	(sequence "R%u", 3, 31), SP
)>;
