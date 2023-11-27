// RUN: %clang_cc1 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK-LABEL %s

// CHECK-LABEL: define dso_local noundef i64 @_Z6squarex(i64 noundef %num) [[TF:#[0-9]+]] {
long long __spoils<eax,esi>
square(long long num) {
    return num * num;
}

// CHECK-LABEL: attributes [[TF]] = { {{.*}}"spoils"="eax,esi"{{.*}} }
