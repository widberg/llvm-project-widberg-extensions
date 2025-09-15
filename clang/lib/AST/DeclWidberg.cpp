#include "clang/AST/DeclWidberg.h"

#include "clang/AST/ASTContext.h"

using namespace clang;

WidbergLocation::WidbergLocation(const ASTContext &C, SourceLocation ATLoc,
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

WidbergLocation *WidbergLocation::Create(
    const ASTContext &C, SourceLocation ATLoc, SourceLocation LAngleLoc,
    ArrayRef<IdentifierLoc *> RegisterIdentifiers, SourceLocation RAngleLoc) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<IdentifierLoc *>(RegisterIdentifiers.size()),
                 alignof(WidbergLocation));
  return new (Mem)
      WidbergLocation(C, ATLoc, LAngleLoc, RegisterIdentifiers, RAngleLoc);
}
