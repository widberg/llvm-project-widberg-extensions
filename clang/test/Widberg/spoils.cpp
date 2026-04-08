// RUN: %clang_cc1 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK-LABEL %s

// CHECK-LABEL: define dso_local noundef i64 @_Z6squarex(i64 noundef %num) [[TF:#[0-9]+]] {
long long __spoils<eax,esi>
square(long long num) {
    return num * num;
}

// Empty __spoils list is valid and means nothing is spoiled.
// CHECK-LABEL: define dso_local noundef i64 @_Z9no_spoilsx(i64 noundef %num) [[TF2:#[0-9]+]] {
long long __spoils<>
no_spoils(long long num) {
    return num + 1;
}

// Explicit cdecl should still apply spoils normally.
// CHECK-LABEL: define dso_local noundef i64 @_Z{{[0-9]+}}cdecl_spoilsx(i64 noundef %num) [[TF:#[0-9]+]] {
long long __attribute__((cdecl)) __spoils<eax,esi>
cdecl_spoils(long long num) {
    return num - 1;
}

// stdcall should keep the spoils attribute and set the calling convention.
// CHECK-LABEL: define dso_local x86_stdcallcc noundef i64 @_Z{{[0-9]+}}stdcall_spoilsx(i64 noundef %num) [[TF:#[0-9]+]] {
long long __attribute__((stdcall)) __spoils<eax,esi>
stdcall_spoils(long long num) {
    return num - 1;
}

// Empty __spoils list is also valid for stdcall.
// CHECK-LABEL: define dso_local x86_stdcallcc noundef i64 @_Z{{[0-9]+}}stdcall_no_spoilsx(i64 noundef %num) [[TF2:#[0-9]+]] {
long long __attribute__((stdcall)) __spoils<>
stdcall_no_spoils(long long num) {
    return num + 2;
}

// CHECK-LABEL: attributes [[TF]] = { {{.*}}"spoils"="eax,esi"{{.*}} }
// CHECK-LABEL: attributes [[TF2]] = { {{.*}}"spoils"{{.*}} }
