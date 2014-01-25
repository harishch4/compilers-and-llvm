// 15-745 S13 Assignment 2: LocalOpts.cpp
// 
// Based on code from Todd C. Mowry
// Modified by Arthur Peters
// Modified by Ankit Goyal
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/PassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/DebugInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/Instruction.h"
#include "llvm/DebugInfo.h"
#include "llvm/Assembly/AssemblyAnnotationWriter.h"
#include "llvm/IR/Constants.h"

#include <ostream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace llvm;

namespace {


    APInt generalIntZero; //to take care of both 32 and 64 bits.
    APInt generalIntOne;

    class LocalOpts : public FunctionPass{

        public:
            static char ID;
            LocalOpts() : FunctionPass(ID) { } //default constructor
            ~LocalOpts() {  }

            void printFunctionDetails(Function &F) {
                int arg_size = F.arg_size();
                int num_call_sites = F.getNumUses();
                int num_basic_blocks = F.size(); //defined in value class.
                //count the number of instructions.
                int number_of_instructions = 0;
                for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
                    number_of_instructions += 1;

                errs() << F.getName() <<": arguments=" << arg_size << " call sites=" <<  num_call_sites << " basic blocks=" << num_basic_blocks << " instructions=" << number_of_instructions << "\n\n";
            }

            void tryAlgebraicIdentitiesForAdd(Instruction &i){
                errs() << i << "\n";
                Type *tp = i.getType();
                Value *operand1 = i.getOperand(0);
                Value *operand2 = i.getOperand(1);
                const IntegerType * intype ;
                if(intype= dyn_cast<IntegerType>(i.getType()))
                    errs() << "YAHOO! 1 haha" << "\n";
                generalIntZero =  APInt(intype->getBitWidth(), 0);

                ConstantInt *cint;
                errs() << "Name\n" <<  operand1 -> getName();
                if(cint = dyn_cast<ConstantInt>(operand2)){
                    if(cint->getValue().eq(generalIntZero))
                        errs() << "Kill me!" << "\n";
                 }
              //  errs() << "VALUE " << cint->getValue() << "\n";
            }

            void runOnBasicBlock(BasicBlock &blk){
                for (BasicBlock::iterator i = blk.begin(), e = blk.end(); i != e; ++i){
                    Instruction *ii= dyn_cast<Instruction>(i);
                    if(ii->getOpcode() == Instruction::Add)
                        tryAlgebraicIdentitiesForAdd(*ii);
//                        errs() << *ii << "  --- " << ii->getOpcode() << "\n";
                    }
            }

            
            virtual bool runOnFunction(Function &func){
                errs() << "Function: " << func.getName() << "\n";
                for (Function::iterator i = func.begin(), e = func.end(); i != e; ++i){ //iterating over the basic blocks                    
                    runOnBasicBlock(*i);
                    //print_basic_block_info(*i);
                }

                return false;
            }


            // We don't modify the program, so we preserve all analyses
            virtual void getAnalysisUsage(AnalysisUsage &AU) const {
                AU.setPreservesAll();
            }

        private:
            //helper functions
            void print_basic_block_info(BasicBlock &b){
                errs() << "Basic block (name=" << b.getName() << ") has " << b.size() << " instructions.\n";

            }
    };

    char LocalOpts::ID = 0;

    static void registerMyPass(const PassManagerBuilder &,
            PassManagerBase &PM) {
        PM.add(new LocalOpts());
    }
    RegisterStandardPasses
        RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                registerMyPass);

    RegisterPass<LocalOpts> X("my-local-opts", "Local Optimizations");

}
