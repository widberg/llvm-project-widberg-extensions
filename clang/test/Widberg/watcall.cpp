// RUN: %clang_cc1 -triple i386-pc-windows-msvc -emit-llvm -O3 -o - %s | FileCheck --check-prefix=CHECK-LABEL %s

// CHECK-LABEL: define dso_local x86_watcallcc noundef i32 @"?test1@@YtHHF_NIHH@Z"(i32 inreg noundef %a, i16 inreg noundef signext %d, i1 inreg noundef zeroext %b, i32 inreg noundef %c, i32 noundef %s4, i32 noundef %s8)
int __watcall test1(int a, short d, bool b, unsigned c, int s4, int s8) {
    return a * d + b * c + s4 * s8;
}

// CHECK-LABEL: define dso_local x86_watcallcc noundef i32 @"?test2@@YtHHMF_NI@Z"(i32 inreg noundef %a, float noundef %s, i16 inreg noundef signext %d, i1 inreg noundef zeroext %b, i32 inreg noundef %c)
int __watcall test2(int a, float s, short d, bool b, unsigned c) {
    return a * d + b * c * s;
}

// CHECK-LABEL: define dso_local x86_watcallcc noundef float @"?test3@@YtMHMF@Z"(i32 inreg noundef %a, float noundef %s, i16 inreg noundef signext %d)
float __watcall test3(int a, float s, short d) {
    return a * d + s;
}

// CHECK-LABEL: define dso_local x86_watcallcc void @"?test4@@YtXHAAM@Z"(i32 inreg noundef %a, ptr inreg noundef nonnull align 4 captures(none) dereferenceable(4) %d)
void __watcall test4(int a, float &d) {
    d += a;
}

// CHECK-LABEL: define dso_local x86_watcallcc void @"?test5@@YtXPAH@Z"(ptr inreg noundef captures(none) %a)
void __watcall test5(int *a) {
    *a *= 2;
}

// CHECK-LABEL: define dso_local x86_watcallcc noundef i64 @"?test6@@Yt_J_J@Z"(i64 noundef returned %ad)
long long __watcall test6(long long ad) {
    return ad;
}

// CHECK-LABEL: define dso_local x86_watcallcc noundef i64 @"?test6@@Yt_JH_JH@Z"(i32 inreg noundef %a, i64 noundef returned %bc, i32 inreg noundef %d)
long long __watcall test6(int a, long long bc, int d) {
    return bc;
}

// CHECK-LABEL: define dso_local x86_watcallcc noundef float @"?test7@@YtMM_J@Z"(float noundef %s4, i64 noundef %s8)
float __watcall test7(float s4, long long s8) {
    return s4 + s8;
}

// CHECK-LABEL: define dso_local x86_watcallcc noundef i64 @"?test8@@Yt_J_J0@Z"(i64 noundef %ad, i64 noundef %bc)
long long __watcall test8(long long ad, long long bc) {
    return ad + bc;
}
