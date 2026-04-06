; RUN: llc < %s -O=3 -mtriple=i386-pc-windows-msvc | FileCheck %s

%class.A = type { i32 }

define dso_local usercallcc noundef "widberg_location"="ecx" i32 @"?add@A@@SrH_VAAV1@HH@Z"(ptr noundef nonnull align 4 "widberg_location"="eax" %this_, i32 noundef "widberg_location"="ebx" %x, i32 noundef %y) local_unnamed_addr {
entry:
; CHECK-LABEL: "?add@A@@SrH_VAAV1@HH@Z":
; CHECK: movl	%ebx, %ecx
; CHECK: imull	4(%esp), %ecx
; CHECK: addl	(%eax), %ecx
; CHECK: movl	%ecx, (%eax)
; CHECK: retl
  %mul = mul nsw i32 %y, %x
  %0 = load i32, ptr %this_, align 4, !tbaa !0
  %add = add nsw i32 %0, %mul
  store i32 %add, ptr %this_, align 4, !tbaa !0
  ret i32 %add
}

define dso_local usercallcc noundef "widberg_location"="eax" i32 @"?sub@A@@SrH_VAAV1@HH@Z"(ptr noundef nonnull align 4 %this_, i32 noundef "widberg_location"="ecx" %x, i32 noundef %y) local_unnamed_addr {
entry:
; CHECK-LABEL: "?sub@A@@SrH_VAAV1@HH@Z":
; CHECK: movl	4(%esp), %edx
; CHECK: imull	8(%esp), %ecx
; CHECK: movl	(%edx), %eax
; CHECK: subl	%ecx, %eax
; CHECK: movl	%eax, (%edx)
; CHECK: retl
  %mul = mul nsw i32 %y, %x
  %0 = load i32, ptr %this_, align 4, !tbaa !0
  %sub = sub nsw i32 %0, %mul
  store i32 %sub, ptr %this_, align 4, !tbaa !0
  ret i32 %sub
}

define dso_local noundef i32 @main() local_unnamed_addr {
entry:
; CHECK-LABEL: _main:
; CHECK: pushl	%ebx
; CHECK-NEXT: pushl	%esi
; CHECK-NEXT: pushl	%eax
; CHECK-NEXT: movl	$0, (%esp)
; CHECK-NEXT: movl	%esp, %esi
; CHECK-NEXT: movl	%esi, %eax
; CHECK-NEXT: movl	$1, %ebx
; CHECK-NEXT: pushl	$2
; CHECK-NEXT: calll	"?add@A@@SrH_VAAV1@HH@Z"
; CHECK-NEXT: addl	$4, %esp
; CHECK-NEXT: movl	$4, %ecx
; CHECK-NEXT: pushl	$5
; CHECK-NEXT: pushl	%esi
; CHECK-NEXT: calll	"?sub@A@@SrH_VAAV1@HH@Z"
; CHECK-NEXT: addl	$8, %esp
; CHECK-NEXT: movl	(%esp), %eax
; CHECK-NEXT: addl	$4, %esp
; CHECK-NEXT: popl	%esi
; CHECK-NEXT: popl	%ebx
; CHECK-NEXT: retl
  %a = alloca %class.A, align 4
  call void @llvm.lifetime.start.p0(ptr nonnull %a)
  store i32 0, ptr %a, align 4, !tbaa !0
  %call1 = call usercallcc noundef "widberg_location"="ecx" i32 @"?add@A@@SrH_VAAV1@HH@Z"(ptr noundef nonnull align 4 "widberg_location"="eax" %a, i32 noundef "widberg_location"="ebx" 1, i32 noundef 2)
  %call2 = call usercallcc noundef "widberg_location"="eax" i32 @"?sub@A@@SrH_VAAV1@HH@Z"(ptr noundef nonnull align 4 %a, i32 noundef "widberg_location"="ecx" 4, i32 noundef 5)
  %0 = load i32, ptr %a, align 4, !tbaa !0
  call void @llvm.lifetime.end.p0(ptr nonnull %a)
  ret i32 %0
}

declare void @llvm.lifetime.start.p0(ptr captures(none))
declare void @llvm.lifetime.end.p0(ptr captures(none))

!llvm.module.flags = !{!1}

!0 = !{!2, !3, i64 0}
!1 = !{i32 1, !"NumRegisterParameters", i32 0}
!2 = !{!"?AVA@@", !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C++ TBAA"}
