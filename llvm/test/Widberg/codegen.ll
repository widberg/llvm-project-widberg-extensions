; RUN: llc < %s -O=3 -mtriple=i386-unknown-unknown | FileCheck %s

@is_odd_also = local_unnamed_addr global ptr @is_odd, align 4

define dso_local usercallcc "widberg_location"="ebx,ecx" i64 @square(i64 noundef "widberg_location"="eax,edx" %num) local_unnamed_addr "spoils"="eax,esi" {
entry:
; CHECK-LABEL: square:
; CHECK: pushl	%edx
; CHECK: movl	%edx, %ecx
; CHECK: imull	%eax, %ecx
; CHECK: mull	%eax
; CHECK: addl	%ecx, %ecx
; CHECK: addl	%edx, %ecx
; CHECK: movl	%eax, %ebx
; CHECK: popl	%edx
; CHECK: retl
  %mul = mul nsw i64 %num, %num
  ret i64 %mul
}

define dso_local usercallcc zeroext "widberg_location"="al" i1 @is_even(i32 noundef %num) local_unnamed_addr {
entry:
; CHECK-LABEL: is_even:
; CHECK: testb	$1, 4(%esp)
; CHECK: sete	%al
; CHECK: retl	$4
  %0 = and i32 %num, 1
  %cmp = icmp eq i32 %0, 0
  ret i1 %cmp
}

define dso_local userpurgecc void @is_odd(i32 noundef %num, ptr nocapture noundef nonnull writeonly align 1 dereferenceable(1) "widberg_location"="eax" %result) {
entry:
; CHECK-LABEL: is_odd:
; CHECK: movl	$-2147483647, %ecx
; CHECK: andl	4(%esp), %ecx
; CHECK: cmpl	$1, %ecx
; CHECK: sete	(%eax)
; CHECK: retl
  %0 = and i32 %num, -2147483647
  %cmp = icmp eq i32 %0, 1
  %frombool = zext i1 %cmp to i8
  store i8 %frombool, ptr %result, align 1, !tbaa !5
  ret void
}

define dso_local usercallcc "widberg_location"="ebx" ptr @call_fn_ptr(ptr nocapture noundef readonly "widberg_location"="edx" %x) local_unnamed_addr {
entry:
; CHECK-LABEL: call_fn_ptr:
; CHECK: movl	$1337, %ecx
; CHECK: calll	*%edx
; CHECK: movl	%eax, %ebx
; CHECK: retl
  %call = tail call usercallcc noundef "widberg_location"="eax" ptr %x(i32 noundef "widberg_location"="ecx" 1337)
  ret ptr %call
}

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 16.0.6 (https://github.com/llvm/llvm-project.git 7cbf1a2591520c2491aa35339f227775f4d3adf6)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"bool", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C++ TBAA"}
