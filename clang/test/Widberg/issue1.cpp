// RUN: %clang_cc1 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK-LABEL %s

// CHECK-LABEL: define dso_local userpurgecc "widberg_location"="ebx" i32 @foo(ptr noundef "widberg_location"="edi" %a1, i32 noundef %a2)
extern "C" int __userpurge foo@<ebx>(int* a1@<edi>, int a2) {
    return *a1 + a2;
}

// CHECK-LABEL: define dso_local usercallcc "widberg_location"="ecx" float @bar(i32 noundef "widberg_location"="eax" %a1, i32 noundef %a2, i32 noundef %a3, ptr noundef %a4, float noundef %a5)
extern "C" float __usercall bar@<ecx>(int a1@<eax>, int a2, int a3, float* a4, float a5) {
    return a1 + a2 + a3 + *a4 + a5;
}

// CHECK-LABEL: define dso_local usercallcc "widberg_location"="ebx" ptr @test(ptr noundef "widberg_location"="edx" %x)
extern "C" int *__usercall test@<ebx>(int *(__usercall *x)@<eax>(long @<ecx>, int)@<edx>) {
    // CHECK-LABEL: %call = call usercallcc noundef "widberg_location"="eax" ptr %0(i32 noundef "widberg_location"="ecx" 1337, i32 noundef 0)
    return x(1337, 0);
}
