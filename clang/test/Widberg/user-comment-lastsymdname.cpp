// RUN: %clang_cc1 -triple i386-pc-windows-msvc -fms-extensions -emit-llvm -o - %s | FileCheck --check-prefix=CHECK %s

#pragma comment(user, "start=" __LASTSYMDNAME__)
int global_symbol;
#pragma comment(user, "after_global=" __LASTSYMDNAME__)
int free_symbol(int x);
#pragma comment(user, "after_free=" __LASTSYMDNAME__)
struct Ty {
  int member_symbol(int x);
};
#pragma comment(user, "after_member=" __LASTSYMDNAME__)

// CHECK: !llvm.user-comments = !{![[C0:[0-9]+]], ![[C1:[0-9]+]], ![[C2:[0-9]+]], ![[C3:[0-9]+]]}
// CHECK: ![[C0]] = !{!"start="}
// CHECK: ![[C1]] = !{!"after_global={{[^"]*global_symbol[^"]*}}"}
// CHECK: ![[C2]] = !{!"after_free={{[^"]*free_symbol[^"]*}}"}
// CHECK: ![[C3]] = !{!"after_member={{[^"]*member_symbol[^"]*}}"}
