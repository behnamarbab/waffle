/*
   american fuzzy lop - LLVM-mode instrumentation pass
   ---------------------------------------------------

   Written by Laszlo Szekeres <lszekeres@google.com> and
              Michal Zalewski <lcamtuf@google.com>

   LLVM integration design comes from Laszlo Szekeres. C bits copied-and-pasted
   from afl-as.c are Michal's fault.

   Copyright 2015, 2016 Google Inc. All rights reserved.
   Copyright 2019 wcventure Inc. All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at:

     http://www.apache.org/licenses/LICENSE-2.0

   This library is plugged into LLVM when invoking clang through afl-clang-fast.
   It tells the compiler to add code roughly equivalent to the bits discussed
   in ../afl-as.h.

 */

#define AFL_LLVM_PASS

#include "../config.h"
#include "../debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constant.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/InstrTypes.h"

using namespace llvm;

namespace {

  class AFLCoverage : public ModulePass {

    public:

      static char ID;
      AFLCoverage() : ModulePass(ID) { }

      bool runOnModule(Module &M) override;

      // StringRef getPassName() const override {
      //  return "American Fuzzy Lop Instrumentation";
      // }

  };

}

static inline std::string loc_description (const DebugLoc& dd) {
  if(!dd) { return "?"; }
  auto* scope = cast<DIScope>(dd.getScope());
  return scope->getFilename().str() + ":" + std::to_string(dd.getLine()) + ":" + std::to_string(dd.getCol());
}

static inline std::string bb_description(const BasicBlock& bb) {
  return "(" + loc_description(bb.getInstList().begin()->getDebugLoc()) + "-" + loc_description(bb.getTerminator()->getDebugLoc()) + ")";

}

struct CountAllVisitor : public InstVisitor<CountAllVisitor> {
  unsigned Count;
  CountAllVisitor() : Count(0) {}

  // void visitICmpInst(ICmpInst &I) { ++Count; }
  // void visitFCmpInst(FCmpInst &I)                { ++Count;}
  // void visitAllocaInst(AllocaInst &I)            { ++Count;}
  // void visitLoadInst(LoadInst     &I)            { ++Count;}
  void visitStoreInst(StoreInst   &I)            { 
    ++Count;
    // OKF("Storing %u", Count);
  }
  // void visitAtomicCmpXchgInst(AtomicCmpXchgInst &I) { ++Count;}
  // void visitAtomicRMWInst(AtomicRMWInst &I)      { ++Count;}
  // void visitFenceInst(FenceInst   &I)            { ++Count;}
  // void visitGetElementPtrInst(GetElementPtrInst &I){ ++Count;}
  // void visitPHINode(PHINode       &I)            { ++Count;}
  // void visitTruncInst(TruncInst &I)              { ++Count;}
  // void visitZExtInst(ZExtInst &I)                { ++Count;}
  // void visitSExtInst(SExtInst &I)                { ++Count;}
  // void visitFPTruncInst(FPTruncInst &I)          { ++Count;}
  // void visitFPExtInst(FPExtInst &I)              { ++Count;}
  // void visitFPToUIInst(FPToUIInst &I)            { ++Count;}
  // void visitFPToSIInst(FPToSIInst &I)            { ++Count;}
  // void visitUIToFPInst(UIToFPInst &I)            { ++Count;}
  // void visitSIToFPInst(SIToFPInst &I)            { ++Count;}
  // void visitPtrToIntInst(PtrToIntInst &I)        { ++Count;}
  // void visitIntToPtrInst(IntToPtrInst &I)        { ++Count;}
  // void visitBitCastInst(BitCastInst &I)          { ++Count;}
  // void visitAddrSpaceCastInst(AddrSpaceCastInst &I) { ++Count;}
  // void visitSelectInst(SelectInst &I)            { ++Count;}
  // void visitVAArgInst(VAArgInst   &I)            { ++Count;}
  // void visitExtractElementInst(ExtractElementInst &I) { ++Count;}
  // void visitInsertElementInst(InsertElementInst &I) { ++Count;}
  // void visitShuffleVectorInst(ShuffleVectorInst &I) { ++Count;}
  // void visitExtractValueInst(ExtractValueInst &I){ ++Count;}
  // void visitInsertValueInst(InsertValueInst &I)  { ++Count;}
  // void visitLandingPadInst(LandingPadInst &I)    { ++Count;}
  // void visitFuncletPadInst(FuncletPadInst &I) { ++Count;}
  // void visitCleanupPadInst(CleanupPadInst &I) { ++Count;}
  // void visitCatchPadInst(CatchPadInst &I)     { ++Count;}
  // // void visitFreezeInst(FreezeInst &I)         { ++Count;}
  void visitInstruction(Instruction &I) {
    ++Count;
    // OKF("Visiting %u", Count);
  }

  // void visitDbgDeclareInst(DbgDeclareInst &I)    { ++Count;}
  // void visitDbgValueInst(DbgValueInst &I)        { ++Count;}
  // // void visitDbgVariableIntrinsic(DbgVariableIntrinsic &I)  { ++Count;}
  // // void visitDbgLabelInst(DbgLabelInst &I)        { ++Count;}
  // void visitDbgInfoIntrinsic(DbgInfoIntrinsic &I){ ++Count;}
  // void visitMemSetInst(MemSetInst &I)            { ++Count;}
  // void visitMemCpyInst(MemCpyInst &I)            { ++Count;}
  // void visitMemMoveInst(MemMoveInst &I)          { ++Count;}
  // void visitMemTransferInst(MemTransferInst &I)  { ++Count;}
  // void visitMemIntrinsic(MemIntrinsic &I)        { ++Count;}
  // void visitVAStartInst(VAStartInst &I)          { ++Count;}
  // void visitVAEndInst(VAEndInst &I)              { ++Count;}
  // void visitVACopyInst(VACopyInst &I)            { ++Count;}
  // void visitIntrinsicInst(IntrinsicInst &I)      { ++Count;}
  // void visitCallInst(CallInst &I)                { ++Count;}
  // void visitInvokeInst(InvokeInst &I)            { ++Count;}
  // void visitCallBrInst(CallBrInst &I)            { ++Count;}
  // void visitTerminator(Instruction &I) {Count++;}

  // void visitReturnInst(ReturnInst &I) { Count++; }
  // void visitBranchInst(BranchInst &I) { Count++; }
  // void visitSwitchInst(SwitchInst &I) { Count++; }
  // void visitIndirectBrInst(IndirectBrInst &I) { Count++; }
  // void visitResumeInst(ResumeInst &I) { Count++; }
  // void visitUnreachableInst(UnreachableInst &I) { Count++; }
  // void visitCleanupReturnInst(CleanupReturnInst &I) { Count++; }
  // void visitCatchReturnInst(CatchReturnInst &I) { Count++; }
  // void visitCatchSwitchInst(CatchSwitchInst &I) { Count++; }

  // void visitCastInst(CastInst &I)                { Count++;}
  // // void visitUnaryOperator(UnaryOperator &I)      { Count++;}
  // void visitBinaryOperator(BinaryOperator &I)    { Count++;}
  // void visitCmpInst(CmpInst &I)                  { Count++;}
  // void visitUnaryInstruction(UnaryInstruction &I){ Count++;}


};


char AFLCoverage::ID = 0;


bool AFLCoverage::runOnModule(Module &M) {

  LLVMContext &C = M.getContext();

  IntegerType *Int8Ty  = IntegerType::getInt8Ty(C);
  IntegerType *Int32Ty = IntegerType::getInt32Ty(C);
  PointerType *CharPtrTy = PointerType::getUnqual(Int8Ty);
  Type *VoidTy = Type::getVoidTy(C);

  /* Show a banner */

  char be_quiet = 0;

  if (isatty(2) && !getenv("AFL_QUIET")) {

    SAYF(cCYA "MemLock-stack-fuzzer: afl-llvm-pass " cBRI VERSION cRST " by <wcventure@126.com>\n");

  } else be_quiet = 1;

  /* Decide instrumentation ratio */

  char* inst_ratio_str = getenv("AFL_INST_RATIO");
  unsigned int inst_ratio = 100;

  if (inst_ratio_str) {

    if (sscanf(inst_ratio_str, "%u", &inst_ratio) != 1 || !inst_ratio ||
        inst_ratio > 100)
      FATAL("Bad value of AFL_INST_RATIO (must be between 1 and 100)");

  }

  /* Get globals for the SHM region and the previous location. Note that
     __afl_prev_loc is thread-local. */

    /* 添加函数声明，插桩用途 */
  llvm::LLVMContext& context = M.getContext ();
  llvm::IRBuilder<> builder(context); 

  std::vector<Type *> icnt_args(1, Type::getInt32Ty(context));
  llvm::FunctionType *icntIncrement =
      llvm::FunctionType::get(builder.getVoidTy(), icnt_args, false);
  llvm::Function *icnt_Increment =
      llvm::Function::Create(icntIncrement, llvm::Function::ExternalLinkage, "instr_AddInsts", &M);

  GlobalVariable *AFLMapPtr =
      new GlobalVariable(M, PointerType::get(Int8Ty, 0), false,
                         GlobalValue::ExternalLinkage, 0, "__afl_area_ptr");

  GlobalVariable *AFLIcntPtr =
      new GlobalVariable(M, PointerType::get(Int32Ty, 0), false,
                         GlobalValue::ExternalLinkage, 0, "__afl_icnt_ptr");

  GlobalVariable *AFLPrevLoc = new GlobalVariable(
      M, Int32Ty, false, GlobalValue::ExternalLinkage, 0, "__afl_prev_loc",
      0, GlobalVariable::GeneralDynamicTLSModel, 0, false);

  GlobalVariable *AFLPrevLocDesc = new GlobalVariable(
      M, CharPtrTy, false, GlobalValue::ExternalLinkage, 0, "__afl_prev_loc_desc",
      0, GlobalVariable::GeneralDynamicTLSModel, 0, false);

  ConstantInt* ICNTMask = ConstantInt::get(Int32Ty, ICNT_SIZE-1);

  Function* LogLocationsFunc = Function::Create(FunctionType::get(VoidTy, 
      ArrayRef<Type*>({CharPtrTy, CharPtrTy}), true), GlobalVariable::ExternalLinkage,
      "__afl_log_loc", &M);
  

  /* Instrument all the things! */

  int inst_blocks = 0;

  for (auto &F : M) {

    bool functionFlag = true;//第一次进函数时flag为true，插桩后改为false

    for (auto &BB : F) {

      BasicBlock::iterator IP = BB.getFirstInsertionPt();
      IRBuilder<> IRB(&(*IP));

      functionFlag = false;

      /* Make up cur_loc */

      unsigned int cur_loc = AFL_R(MAP_SIZE);

      ConstantInt *CurLoc = ConstantInt::get(Int32Ty, cur_loc);
      
      /* Get current source location information */
      std::string cur_loc_desc = bb_description(BB);
      Value* CurLocDesc = IRB.CreateGlobalStringPtr(cur_loc_desc);

      /* Load prev_loc */

      LoadInst *PrevLoc = IRB.CreateLoad(AFLPrevLoc);
      PrevLoc->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
      Value *PrevLocCasted = IRB.CreateZExt(PrevLoc, IRB.getInt32Ty());

      /* Get edge ID as XOR */
      Value* EdgeId = IRB.CreateXor(PrevLocCasted, CurLoc);

      /* Load SHM pointer */

      LoadInst *MapPtr = IRB.CreateLoad(AFLMapPtr);
      MapPtr->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
      Value *MapPtrIdx =
          IRB.CreateGEP(MapPtr, EdgeId);

      LoadInst *IcntPtr = IRB.CreateLoad(AFLIcntPtr);
      IcntPtr->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
      Value *IcntBranchPtr =
        IRB.CreateGEP(IcntPtr, IRB.CreateAnd(EdgeId, ICNTMask));

      /* Update bitmap */

      LoadInst *Counter = IRB.CreateLoad(MapPtrIdx);
      Counter->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));
      Value *Incr = IRB.CreateAdd(Counter, ConstantInt::get(Int8Ty, 1));
      IRB.CreateStore(Incr, MapPtrIdx)
          ->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));

      /* Set prev_loc to cur_loc >> 1 */

      StoreInst *Store =
          IRB.CreateStore(ConstantInt::get(Int32Ty, cur_loc >> 1), AFLPrevLoc);
      Store->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));

      /* Possibly log location */
      LoadInst* PrevLocDesc = IRB.CreateLoad(AFLPrevLocDesc);
      IRB.CreateCall(LogLocationsFunc, ArrayRef<Value*>({ PrevLocDesc, CurLocDesc }));
      

      /* Set prev_loc_desc to cur_loc_desc */
      IRB.CreateStore(CurLocDesc, AFLPrevLocDesc);

      
      /* Increment instruction counters  */
      CountAllVisitor CAV;
      CAV.visit(BB);
      
      Value *CNT = IRB.getInt32(1);
      // Value *CNT = IRB.getInt32(CAV.Count);
      
      LoadInst *IcntLoad = IRB.CreateLoad(IcntBranchPtr);
      Value *IcntIncr = IRB.CreateAdd(IcntLoad, CNT);

      IRB.CreateStore(IcntIncr, IcntBranchPtr)
          ->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(C, None));

      IRB.CreateCall(icnt_Increment, ArrayRef<Value*>({ CNT }));

      inst_blocks++;

    }

  }

  /* Say something nice. */

  if (!be_quiet) {

    if (!inst_blocks) WARNF("No instrumentation targets found.");
    else OKF("x      Instrumented %u locations (%s mode, ratio %u%%).",
             inst_blocks, getenv("AFL_HARDEN") ? "hardened" :
             ((getenv("AFL_USE_ASAN") || getenv("AFL_USE_MSAN")) ?
              "ASAN/MSAN" : "non-hardened"), inst_ratio);

  }

  return true;

}


static void registerAFLPass(const PassManagerBuilder &,
                            legacy::PassManagerBase &PM) {

  PM.add(new AFLCoverage());

}


static RegisterStandardPasses RegisterAFLPass(
    PassManagerBuilder::EP_ModuleOptimizerEarly, registerAFLPass);

static RegisterStandardPasses RegisterAFLPass0(
    PassManagerBuilder::EP_EnabledOnOptLevel0, registerAFLPass);
