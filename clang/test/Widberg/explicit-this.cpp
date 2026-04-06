// RUN: %clang_cc1 -triple i386-pc-windows-msvc -O3 -emit-llvm -o - %s | FileCheck --check-prefix=CHECK %s
// RUN: %clang_cc1 -triple i386-pc-windows-msvc -E -dM %s | FileCheck --check-prefix=CHECK-MACROS %s

// CHECK-MACROS: #define __cpp_explicit_this_parameter 202110L
// CHECK-MACROS: #define __widberg__ 1

// CHECK: %class.A = type { i32 }

class A {
public:
    int __usercall add@<ecx>(this A& this_@<eax>, int x@<ebx>, int y);
    int __usercall sub@<eax>(this A& this_, int x@<ecx>, int y);
    int z = 0;
};

__attribute__((noinline))
// CHECK-LABEL: define dso_local usercallcc noundef "widberg_location"="ecx" i32 @"?add@A@@SrH_VAAV1@HH@Z"
// CHECK-SAME: (ptr noundef nonnull align 4 captures(none) dereferenceable(4) "widberg_location"="eax" %this_, i32 noundef "widberg_location"="ebx" %x, i32 noundef %y) local_unnamed_addr #[[ATTR0:[0-9]+]] align 2 {
// CHECK-NEXT:  entry:
// CHECK-NEXT:   %mul = mul nsw i32 %y, %x
// CHECK-NEXT:   %0 = load i32, ptr %this_, align 4, !tbaa ![[TBAA0:[0-9]+]]
// CHECK-NEXT:   %add = add nsw i32 %0, %mul
// CHECK-NEXT:   store i32 %add, ptr %this_, align 4, !tbaa ![[TBAA0]]
// CHECK-NEXT:   ret i32 %add
// CHECK-NEXT: }
int __usercall A::add@<ecx>(this A& this_@<eax>, int x@<ebx>, int y) {
    this_.z += x * y;
    return this_.z;
}

__attribute__((noinline))
// CHECK-LABEL: define dso_local usercallcc noundef "widberg_location"="eax" i32 @"?sub@A@@SrH_VAAV1@HH@Z"
// CHECK-SAME: (ptr noundef nonnull align 4 captures(none) dereferenceable(4) %this_, i32 noundef "widberg_location"="ecx" %x, i32 noundef %y) local_unnamed_addr #[[ATTR0]] align 2 {
// CHECK-NEXT:  entry:
// CHECK-NEXT:   %mul = mul nsw i32 %y, %x
// CHECK-NEXT:   %0 = load i32, ptr %this_, align 4, !tbaa ![[TBAA0]]
// CHECK-NEXT:   %sub = sub nsw i32 %0, %mul
// CHECK-NEXT:   store i32 %sub, ptr %this_, align 4, !tbaa ![[TBAA0]]
// CHECK-NEXT:   ret i32 %sub
// CHECK-NEXT: }
int __usercall A::sub@<eax>(this A& this_, int x@<ecx>, int y) {
    this_.z -= x * y;
    return this_.z;
}

// CHECK-LABEL: define dso_local noundef i32 @main()
// CHECK-SAME: local_unnamed_addr #[[ATTR1:[0-9]+]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:   %a = alloca %class.A, align 4
// CHECK-NEXT:   call void @llvm.lifetime.start.p0(ptr nonnull %a) #[[ATTR3:[0-9]+]]
// CHECK-NEXT:   store i32 0, ptr %a, align 4, !tbaa ![[TBAA0]]
// CHECK-NEXT:   %call1 = call usercallcc noundef "widberg_location"="ecx" i32 @"?add@A@@SrH_VAAV1@HH@Z"(ptr noundef nonnull align 4 dereferenceable(4) "widberg_location"="eax" %a, i32 noundef "widberg_location"="ebx" 1, i32 noundef 2)
// CHECK-NEXT:   %call2 = call usercallcc noundef "widberg_location"="eax" i32 @"?sub@A@@SrH_VAAV1@HH@Z"(ptr noundef nonnull align 4 dereferenceable(4) %a, i32 noundef "widberg_location"="ecx" 4, i32 noundef 5)
// CHECK-NEXT:   %0 = load i32, ptr %a, align 4, !tbaa ![[TBAA0]]
// CHECK-NEXT:   call void @llvm.lifetime.end.p0(ptr nonnull %a) #[[ATTR3]]
// CHECK-NEXT:   ret i32 %0
// CHECK-NEXT: }
// CHECK: declare void @llvm.lifetime.start.p0(ptr captures(none)) #[[ATTR2:[0-9]+]]
// CHECK: declare void @llvm.lifetime.end.p0(ptr captures(none)) #[[ATTR2]]
// CHECK: attributes #[[ATTR0]] = { mustprogress nofree noinline norecurse nosync nounwind willreturn memory(argmem: readwrite) "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+x87" }
// CHECK: attributes #[[ATTR1]] = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+x87" }
// CHECK: attributes #[[ATTR2]] = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
// CHECK: attributes #[[ATTR3]] = { nounwind }
// CHECK: ![[TBAA2:[0-9]+]] = !{!"int", ![[TBAA3:[0-9]+]], i64 0}
// CHECK: ![[TBAA3]] = !{!"omnipotent char", ![[TBAA4:[0-9]+]], i64 0}
// CHECK: ![[TBAA4]] = !{!"Simple C++ TBAA"}
// CHECK: ![[TBAA0]] = !{![[TBAA1:[0-9]+]], ![[TBAA2]], i64 0}
// CHECK: ![[TBAA1]] = !{!"?AVA@@", ![[TBAA2]], i64 0}
int main() {
    A a;
    a.add(1, 2);
    a.sub(4, 5);
    return a.z;
}
