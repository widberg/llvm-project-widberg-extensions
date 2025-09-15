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

#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/SourceLocation.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/TrailingObjects.h"

namespace clang {

class ASTContext;
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
                  SourceLocation RAngleLoc);
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
                                 SourceLocation RAngleLoc);

  SourceLocation getATLoc() const { return ATLoc; }
  SourceLocation getLAngleLoc() const { return LAngleLoc; }
  SourceLocation getRAngleLoc() const { return RAngleLoc; }

  SourceRange getSourceRange() const LLVM_READONLY {
    return SourceRange(ATLoc, RAngleLoc);
  }

  bool operator==(const WidbergLocation& Other) const {
    if (size() != Other.size())
      return false;

    for (unsigned i = 0; i < size(); ++i) {
      if (getRegister(i)->getIdentifierInfo()->getName() != Other.getRegister(i)->getIdentifierInfo()->getName())
        return false;
    }

    return true;
  }

  bool operator!=(const WidbergLocation& Other) const {
    return !(*this == Other);
  }
};

} // namespace clang

#endif // LLVM_CLANG_AST_DECLWIDBERG_H
