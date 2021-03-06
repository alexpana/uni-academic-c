#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include <stack>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Type.h>
#include <llvm/DerivedTypes.h>
#include <llvm/LLVMContext.h>
#include <llvm/PassManager.h>
#include <llvm/Instructions.h>
#include <llvm/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/Constants.h>

using namespace llvm;

class StatementBlock;

class CodeGenBlock {
public:
    BasicBlock *block;
    std::map<std::string, Value*> locals;
};

class CodeGenContext {
    std::stack<CodeGenBlock *> blocks;

public:
    Module *module;
    Function *printfFunction;
    Function *currentFunction;
    Function *mainFunction;
    CodeGenContext() { module = new Module("main", getGlobalContext()); }
    
    std::map<std::string, Value*> functionArguments;

    std::string generateCode(StatementBlock& root);
    GenericValue runCode();
    std::map<std::string, Value*>& locals() { return blocks.top()->locals; }
    BasicBlock *currentBlock() { return blocks.top()->block; }
    void pushBlock(BasicBlock *block) { blocks.push(new CodeGenBlock()); blocks.top()->block = block; }
    void popBlock() { CodeGenBlock *top = blocks.top(); blocks.pop(); delete top; }
};

#endif // CODEGEN_H
