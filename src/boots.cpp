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
  Type *params[] = {I32, I32};
  FunctionType *FT = FunctionType::get(I32, params, false);

  Function *F = Function::Create(FT, Function::ExternalLinkage, Twine("foo"),
                                 TheModule.get());
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
  Builder.SetInsertPoint(BB);

  const char *args[] = { "a", "b" };
  int i = 0;
  std::vector<Value*> FctArgs;

  for (auto &Arg: F->args()) {
    FctArgs.push_back(&Arg);
    Arg.setName(args[i++]);
  }

  Value *adder = Builder.CreateAdd(FctArgs[0], FctArgs[1]);
  Builder.CreateRet(adder);

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