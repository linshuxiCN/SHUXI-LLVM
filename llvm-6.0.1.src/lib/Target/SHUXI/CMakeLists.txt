set(LLVM_TARGET_DEFINITIONS SHUXI.td)

tablegen(LLVM SHUXIGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM SHUXIGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM SHUXIGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM SHUXIGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM SHUXIGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM SHUXIGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM SHUXIGenCallingConv.inc -gen-callingconv)
tablegen(LLVM SHUXIGenDAGISel.inc -gen-dag-isel)

add_public_tablegen_target(SHUXICommonTableGen)

add_llvm_target(SHUXICodeGen
  SHUXIAsmPrinter.cpp
  SHUXIFrameLowering.cpp
  SHUXIInstrInfo.cpp
  SHUXIISelDAGToDAG.cpp
  SHUXIISelLowering.cpp
  SHUXIMCInstLower.cpp
  SHUXIRegisterInfo.cpp
  SHUXISubtarget.cpp
  SHUXITargetMachine.cpp
  SHUXIMachineFunctionInfo.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
