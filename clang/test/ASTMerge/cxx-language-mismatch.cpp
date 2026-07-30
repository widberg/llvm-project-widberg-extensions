// RUN: %clang_cc1 -x c++ -emit-pch -o %t.ast %s
// RUN: not %clang_cc1 -ast-merge %t.ast /dev/null -ast-dump-all=json 2>&1 | FileCheck %s
// RUN: %clang_cc1 -x c++ -ast-merge %t.ast /dev/null -fsyntax-only

struct QPropertyBindingSourceLocation {};
void makePropertyBinding(
    QPropertyBindingSourceLocation = QPropertyBindingSourceLocation());
template <typename> class QBindableInterfaceForProperty {
  int iface{[] { makePropertyBinding; }};
};

// CHECK: error: AST file '{{.*}}.ast' was built with C++ language options but the current translation unit is not C++; pass '-x c++' or use matching language options
