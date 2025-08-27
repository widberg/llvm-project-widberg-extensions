//===- DeclWidberg.h - Classes for representing Widberg --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file
/// Defines the Widberg declaration subclasses.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_AST_DECLWIDBERG_H
#define LLVM_CLANG_AST_DECLWIDBERG_H

#include "clang/AST/ASTConcept.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclarationName.h"
#include "clang/AST/Redeclarable.h"
#include "clang/AST/TemplateBase.h"
#include "clang/AST/Type.h"
#include "clang/Basic/LLVM.h"
#include "clang/Sema/ParsedAttr.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/Specifiers.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/ADT/PointerIntPair.h"
#include "llvm/ADT/PointerUnion.h"
#include "llvm/ADT/iterator.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/TrailingObjects.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <utility>

namespace clang {

class WidbergLocation final
    : private llvm::TrailingObjects<WidbergLocation, IdentifierLoc *> {
  /// The location of the '@' punctuation.
  SourceLocation ATLoc;

  /// The locations of the '<' and '>' angle brackets.
  SourceLocation LAngleLoc, RAngleLoc;

  unsigned NumRegisters;

protected:
  WidbergLocation(const ASTContext &C, SourceLocation ATLoc,
                  SourceLocation LAngleLoc,
                  ArrayRef<IdentifierLoc *> RegisterIdentifiers,
                  SourceLocation RAngleLoc)
      : ATLoc(ATLoc), LAngleLoc(LAngleLoc), RAngleLoc(RAngleLoc),
        NumRegisters(RegisterIdentifiers.size()) {
    for (unsigned Idx = 0; Idx < RegisterIdentifiers.size(); ++Idx) {
      IdentifierLoc *P = RegisterIdentifiers[Idx];
      begin()[Idx] = P;
    }
  }
public:
  /// Iterates through the template parameters in this list.
  using iterator = IdentifierLoc **;

  /// Iterates through the template parameters in this list.
  using const_iterator = IdentifierLoc *const *;

  iterator begin() { return getTrailingObjects(); }
  const_iterator begin() const { return getTrailingObjects(); }
  iterator end() { return begin() + NumRegisters; }
  const_iterator end() const { return begin() + NumRegisters; }

  unsigned size() const { return NumRegisters; }

  ArrayRef<IdentifierLoc *> asArray() {
    return llvm::ArrayRef(begin(), end());
  }
  ArrayRef<const IdentifierLoc *> asArray() const {
    return llvm::ArrayRef(begin(), size());
  }

  IdentifierLoc *getRegister(unsigned Idx) {
    assert(Idx < size() && "Template parameter index out-of-range");
    return begin()[Idx];
  }
  const IdentifierLoc *getRegister(unsigned Idx) const {
    assert(Idx < size() && "Template parameter index out-of-range");
    return begin()[Idx];
  }

public:
  friend TrailingObjects;

  static WidbergLocation *Create(const ASTContext &C, SourceLocation ATLoc,
                                 SourceLocation LAngleLoc,
                                 ArrayRef<IdentifierLoc *> RegisterIdentifiers,
                                 SourceLocation RAngleLoc) {
    void *Mem = C.Allocate(
        totalSizeToAlloc<IdentifierLoc *>(RegisterIdentifiers.size()),
        alignof(WidbergLocation));
    return new (Mem)
        WidbergLocation(C, ATLoc, LAngleLoc, RegisterIdentifiers, RAngleLoc);
  }

  SourceLocation getATLoc() const { return ATLoc; }
  SourceLocation getLAngleLoc() const { return LAngleLoc; }
  SourceLocation getRAngleLoc() const { return RAngleLoc; }

  SourceRange getSourceRange() const LLVM_READONLY {
    return SourceRange(ATLoc, RAngleLoc);
  }
};

} // namespace clang

#endif // LLVM_CLANG_AST_DECLWIDBERG_H
