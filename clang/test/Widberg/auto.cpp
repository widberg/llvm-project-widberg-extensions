// RUN: %clang_cc1 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK-LABEL %s

// CHECK-LABEL: @is_odd_also = global ptr @_Z6is_oddiRb, align 4

// CHECK-LABEL: define{{.*}} userpurgecc void @_Z6is_oddiRb(i32 noundef %num, ptr noundef nonnull align 1 dereferenceable(1) "widberg_location"="eax" %result)
void __userpurge is_odd(int num, bool &result@<eax>) {
    result = num % 2 == 1;
}

auto is_odd_also = is_odd;
