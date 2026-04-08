// RUN: %clang_cc1 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK %s

extern "C" double __usercall _0x00445500@<fp0>(float *a1@<eax>, int a2@<ecx>);
extern "C" double __usercall fp1_name@<fp1>(float *a1@<eax>, int a2@<ecx>);
extern "C" double __usercall fp3_name@<fp3>(float *a1@<eax>, int a2@<ecx>);
extern "C" double __usercall fp5_name@<fp5>(float *a1@<eax>, int a2@<ecx>);
extern "C" double __usercall fp7_name@<fp7>(float *a1@<eax>, int a2@<ecx>);

// CHECK-LABEL: define{{.*}} userpurgecc{{.*}} "widberg_location"="al" i8 @_0x005281C0(ptr noundef "widberg_location"="esi" %a1, i32 noundef "widberg_location"="edi" %edi0, ptr noundef %a2)
extern "C" char __userpurge _0x005281C0@<al>(float *a1@<esi>, int edi0@<edi>, float *a2)
{
    float x = 0;
    return _0x00445500(&x, 0) == 0 && fp1_name(&x, 0) == 0 &&
           fp3_name(&x, 0) == 0 && fp5_name(&x, 0) == 0 &&
           fp7_name(&x, 0) == 0;
}

// CHECK: declare usercallcc "widberg_location"="fp0" double @_0x00445500(ptr noundef "widberg_location"="eax", i32 noundef "widberg_location"="ecx")
// CHECK: declare usercallcc "widberg_location"="fp1" double @fp1_name(ptr noundef "widberg_location"="eax", i32 noundef "widberg_location"="ecx")
// CHECK: declare usercallcc "widberg_location"="fp3" double @fp3_name(ptr noundef "widberg_location"="eax", i32 noundef "widberg_location"="ecx")
// CHECK: declare usercallcc "widberg_location"="fp5" double @fp5_name(ptr noundef "widberg_location"="eax", i32 noundef "widberg_location"="ecx")
// CHECK: declare usercallcc "widberg_location"="fp7" double @fp7_name(ptr noundef "widberg_location"="eax", i32 noundef "widberg_location"="ecx")
