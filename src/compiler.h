/* Copyright (c) 2008-2009, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#ifndef COMPILER_H
#define COMPILER_H

#include "system.h"
#include "zone.h"
#include "assembler.h"

namespace vm {

class TraceHandler {
 public:
  virtual void handleTrace(Promise* address, unsigned argumentIndex) = 0;
};

class Compiler {
 public:
  class Client {
   public:
    virtual intptr_t getThunk(UnaryOperation op, unsigned size) = 0;
    virtual intptr_t getThunk(BinaryOperation op, unsigned size, unsigned resultSize) = 0;
    virtual intptr_t getThunk(TernaryOperation op, unsigned size, unsigned resultSize) = 0;
  };
  
  static const unsigned Aligned  = 1 << 0;
  static const unsigned NoReturn = 1 << 1;
  static const unsigned TailJump = 1 << 2;

  class Operand { };
  class State { };
  class Subroutine { };

  virtual State* saveState() = 0;
  virtual void restoreState(State* state) = 0;

  virtual Subroutine* startSubroutine() = 0;
  virtual void endSubroutine(Subroutine* subroutine) = 0;
  virtual void linkSubroutine(Subroutine* subroutine) = 0;

  virtual void init(unsigned logicalCodeSize, unsigned parameterFootprint,
                    unsigned localFootprint, unsigned alignedFrameSize) = 0;

  virtual void visitLogicalIp(unsigned logicalIp) = 0;
  virtual void startLogicalIp(unsigned logicalIp) = 0;

  virtual Promise* machineIp(unsigned logicalIp) = 0;

  virtual Promise* poolAppend(intptr_t value) = 0;
  virtual Promise* poolAppendPromise(Promise* value) = 0;

  virtual Operand* constant(int64_t value, unsigned code) = 0;
  virtual Operand* promiseConstant(Promise* value, unsigned code) = 0;
  virtual Operand* address(Promise* address) = 0;
  virtual Operand* memory(Operand* base,
                          unsigned code,
                          int displacement = 0,
                          Operand* index = 0,
                          unsigned scale = 1) = 0;

  virtual Operand* register_(int number) = 0;

  virtual void push(unsigned footprint) = 0;
  virtual void push(unsigned footprint, Operand* value) = 0;
  virtual void save(unsigned footprint, Operand* value) = 0;
  virtual Operand* pop(unsigned footprint) = 0;
  virtual void pushed() = 0;
  virtual void popped(unsigned footprint) = 0;
  virtual unsigned topOfStack() = 0;
  virtual Operand* peek(unsigned footprint, unsigned index) = 0;

  virtual Operand* call(Operand* address,
                        unsigned flags,
                        TraceHandler* traceHandler,
                        unsigned resultSize,
                        unsigned resultCode,
                        unsigned argumentCount,
                        ...) = 0;

  virtual Operand* stackCall(Operand* address,
                             unsigned flags,
                             TraceHandler* traceHandler,
                             unsigned resultSize,
                             unsigned resultCode,
                             unsigned argumentFootprint) = 0;

  virtual void return_(unsigned size, Operand* value) = 0;

  virtual void initLocal(unsigned size, unsigned index, unsigned code) = 0;
  virtual void initLocalsFromLogicalIp(unsigned logicalIp) = 0;
  virtual void storeLocal(unsigned footprint, Operand* src,
                          unsigned index) = 0;
  virtual Operand* loadLocal(unsigned footprint, unsigned index) = 0;
  virtual void saveLocals() = 0;

  virtual void checkBounds(Operand* object, unsigned lengthOffset,
                           Operand* index, intptr_t handler) = 0;

  virtual void store(unsigned srcSize, Operand* src, unsigned dstSize,
                     Operand* dst) = 0;
  virtual Operand* load(unsigned srcSize, unsigned srcSelectSize, Operand* src,
                        unsigned dstSize) = 0;
  virtual Operand* loadz(unsigned size, unsigned srcSelectSize, Operand* src,
                         unsigned dstSize) = 0;
  virtual Operand* lcmp(Operand* a, Operand* b) = 0;
  virtual void cmp(unsigned size, Operand* a, Operand* b) = 0;
  virtual void fcmp(unsigned size, Operand* a, Operand* b) = 0;
  virtual void jl(Operand* address) = 0;
  virtual void jg(Operand* address) = 0;
  virtual void jle(Operand* address) = 0;
  virtual void jge(Operand* address) = 0;
  virtual void je(Operand* address) = 0;
  virtual void jne(Operand* address) = 0;
  virtual void fjl(Operand* address) = 0;
  virtual void fjg(Operand* address) = 0;
  virtual void fjle(Operand* address) = 0;
  virtual void fjge(Operand* address) = 0;
  virtual void fje(Operand* address) = 0;
  virtual void fjne(Operand* address) = 0;
  virtual void fjuo(Operand* address) = 0;
  virtual void jmp(Operand* address) = 0;
  virtual void exit(Operand* address) = 0;
  virtual Operand* add(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* sub(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* mul(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* div(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* rem(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* fadd(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* fsub(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* fmul(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* fdiv(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* frem(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* shl(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* shr(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* ushr(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* and_(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* or_(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* xor_(unsigned size, Operand* a, Operand* b) = 0;
  virtual Operand* neg(unsigned size, Operand* a) = 0;
  virtual Operand* fneg(unsigned size, Operand* a) = 0;
  virtual Operand* operation(BinaryOperation op, unsigned aSize, unsigned resSize, unsigned resCode, Operand* a) = 0;
  virtual Operand* operation(TernaryOperation op, unsigned aSize, unsigned bSize, unsigned resSize, unsigned resCode, Operand* a, Operand* b) = 0;
  virtual Operand* f2f(unsigned aSize, unsigned resSize, Operand* a) = 0;
  virtual Operand* f2i(unsigned aSize, unsigned resSize, Operand* a) = 0;
  virtual Operand* i2f(unsigned aSize, unsigned resSize, Operand* a) = 0;

  virtual void loadBarrier() = 0;
  virtual void storeStoreBarrier() = 0;
  virtual void storeLoadBarrier() = 0;

  virtual unsigned compile() = 0;
  virtual unsigned poolSize() = 0;
  virtual void writeTo(uint8_t* dst) = 0;

  virtual void dispose() = 0;
};

Compiler*
makeCompiler(System* system, Assembler* assembler, Zone* zone,
             Compiler::Client* client);

} // namespace vm

#endif//COMPILER_H
