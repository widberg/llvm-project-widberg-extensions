// RUN: %clang_cc1 -triple i386-pc-windows-msvc -fms-extensions -emit-llvm -o - %s | FileCheck --check-prefix=CHECK %s

#pragma comment(user, "my comment")
__pragma(comment(user, "another comment"));

// CHECK: !llvm.user-comments = !{![[my_comment:[0-9]+]], ![[another_comment:[0-9]+]]
// CHECK: ![[my_comment]] = !{!"my comment"}
// CHECK: ![[another_comment]] = !{!"another comment"}
