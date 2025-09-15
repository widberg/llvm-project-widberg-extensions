// RUN: %clang_cc1 -O3 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK %s

int __usercall x@<eax>(int a@<eax>);
int __usercall y@<ebx>(int a@<ebx>);

int main() {
    return x(y(0));
}

// CHECK: define dso_local noundef i32 @main() local_unnamed_addr #0 {
// CHECK: entry:
// CHECK:   %call = tail call usercallcc noundef "widberg_location"="ebx" i32 @_Z1yi(i32 noundef "widberg_location"="ebx" 0) #2
// CHECK:   %call1 = tail call usercallcc noundef "widberg_location"="eax" i32 @_Z1xi(i32 noundef "widberg_location"="eax" %call) #2
// CHECK:   ret i32 %call1
// CHECK: }

// CHECK: declare usercallcc noundef "widberg_location"="eax" i32 @_Z1xi(i32 noundef "widberg_location"="eax") local_unnamed_addr #1

// declare usercallcc noundef "widberg_location"="ebx" i32 @_Z1yi(i32 noundef "widberg_location"="ebx") local_unnamed_addr #1
