/* Copyright (c) 2008-2012, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#include "codegen/x86/block.h"
#include "common.h"

#define CAST1(x) reinterpret_cast<UnaryOperationType>(x)
#define CAST2(x) reinterpret_cast<BinaryOperationType>(x)
#define CAST_BRANCH(x) reinterpret_cast<BranchOperationType>(x)

namespace avian {
namespace codegen {
namespace x86 {

unsigned
padding(AlignmentPadding* p, unsigned index, unsigned offset, AlignmentPadding* limit);

MyBlock::MyBlock(unsigned offset):
  next(0), firstPadding(0), lastPadding(0), offset(offset), start(~0),
  size(0)
{ }

unsigned MyBlock::resolve(unsigned start, Assembler::Block* next) {
  this->start = start;
  this->next = static_cast<MyBlock*>(next);

  return start + size + padding(firstPadding, start, offset, lastPadding);
}

} // namespace x86
} // namespace codegen
} // namespace avian
