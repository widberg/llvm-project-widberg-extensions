; RUN: llc < %s -O=3 -mtriple=i386-unknown-unknown | FileCheck %s

define dso_local userpurgecc "widberg_location"="ebx" i32 @foo(ptr nocapture noundef readonly "widberg_location"="edi" %a1, i32 noundef %frame) local_unnamed_addr #0 {
entry:
; CHECK-LABEL: foo:
; CHECK: movl	(%edi), %ebx
; CHECK: addl	4(%esp), %ebx
; CHECK: retl
  %0 = load i32, ptr %a1, align 4, !tbaa !5
  %add = add nsw i32 %0, %frame
  ret i32 %add
}

define dso_local usercallcc "widberg_location"="ecx" float @bar(i32 noundef "widberg_location"="eax" %a1, i32 noundef %a2, i32 noundef %a3, ptr nocapture noundef readonly %a4, float noundef %a5) local_unnamed_addr #0 {
entry:
; CHECK-LABEL: bar:
; CHECK: addl	4(%esp), %eax
; CHECK: addl	8(%esp), %eax
; CHECK: cvtsi2ss	%eax, %xmm0
; CHECK: movl	12(%esp), %eax
; CHECK: addss	(%eax), %xmm0
; CHECK: addss	16(%esp), %xmm0
; CHECK: movd	%xmm0, %ecx
; CHECK: retl	$16
  %add = add nsw i32 %a2, %a1
  %add1 = add nsw i32 %add, %a3
  %conv = sitofp i32 %add1 to float
  %0 = load float, ptr %a4, align 4, !tbaa !9
  %add2 = fadd float %0, %conv
  %add3 = fadd float %add2, %a5
  ret float %add3
}

define dso_local usercallcc "widberg_location"="ebx" ptr @test(ptr noundef "widberg_location"="edx" %x) #1 {
entry:
; CHECK-LABEL: test:
; CHECK: pushl	%eax
; CHECK: movl	%edx, (%esp)
; CHECK: movl	$1337, %ecx
; CHECK: pushl	$0
; CHECK: calll	*%edx
; CHECK: movl	%eax, %ebx
; CHECK: popl	%eax
; CHECK: retl
  %x.addr = alloca ptr, align 4
  store ptr %x, ptr %x.addr, align 4
  %0 = load ptr, ptr %x.addr, align 4
  %call = call usercallcc noundef "widberg_location"="eax" ptr %0(i32 noundef "widberg_location"="ecx" 1337, i32 noundef 0)
  ret ptr %call
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 16.0.6 (https://github.com/llvm/llvm-project.git 7cbf1a2591520c2491aa35339f227775f4d3adf6)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C++ TBAA"}
!9 = !{!10, !10, i64 0}
!10 = !{!"float", !7, i64 0}
