#include "llvm/ADT/APInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <iostream>

using namespace llvm;

int main(int argc, char *argv[]) {
  LLVMContext TheContext;
  IRBuilder<> Builder(TheContext);
  std::unique_ptr<Module> TheModule;

  TheModule = llvm::make_unique<Module>("my cool jit", TheContext);

  IntegerType *I32 = Type::getInt32Ty(TheContext);
  FunctionType *FT = FunctionType::get(I32, false);

  Function *F = Function::Create(FT, Function::ExternalLinkage, Twine("foo"),
                                 TheModule.get());
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
  Builder.SetInsertPoint(BB);

  Value *RetVal = ConstantInt::get(I32, 10, true);
  Builder.CreateRet(RetVal);

  verifyFunction(*F);

  TheModule->dump();

//   std::unique_ptr<KaleidoscopeJIT> TheJIT;
//   TheJIT = llvm::make_unique<KaleidoscopeJIT>();

//   auto H = TheJIT->addModule(std::move(TheModule));
//   auto ExprSymbol = TheJIT->findSymbol("foo");
//   assert(ExprSymbol && "symbol foo not found");

//   auto foo = reinterpret_cast<int32_t (*)()>(ExprSymbol.getAddress());

//   auto result = foo();

//   std::cout << "result = " << result << std::endl;

  return 0;
}