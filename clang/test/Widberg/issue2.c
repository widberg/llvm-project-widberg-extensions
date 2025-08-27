// RUN: %clang_cc1 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK %s

union Tu {
    short b;
} __attribute__((transparent_union));

// CHECK-LABEL: define dso_local void @c(ptr dead_on_unwind noalias writable sret(%union.Tu) align 2 %agg.result, i16 noundef signext %x.coerce)
union Tu c(union Tu x) {
    return x;
}

// CHECK: %result.ptr = alloca ptr, align 4
// CHECK: %x = alloca %union.Tu, align 2
// CHECK: store ptr %agg.result, ptr %result.ptr, align 4
// CHECK: %coerce.dive = getelementptr inbounds nuw %union.Tu, ptr %x, i32 0, i32 0
// CHECK: store i16 %x.coerce, ptr %coerce.dive, align 2
// CHECK: call void @llvm.memcpy.p0.p0.i32(ptr align 2 %agg.result, ptr align 2 %x, i32 2, i1 false)
// CHECK: ret void
