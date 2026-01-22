; RUN: llc < %s -O=3 -mtriple=i386-pc-windows-msvc | FileCheck %s

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local x86_watcallcc noundef i32 @"?test1@@YtHHF_NIHH@Z"(i32 inreg noundef %a, i16 inreg noundef signext %d, i1 inreg noundef zeroext %b, i32 inreg noundef %c, i32 noundef %s4, i32 noundef %s8) local_unnamed_addr #0 {
entry:
  %conv = sext i16 %d to i32
  %mul = mul nsw i32 %a, %conv
  %mul2 = select i1 %b, i32 %c, i32 0
  %add = add i32 %mul2, %mul
  %mul3 = mul nsw i32 %s8, %s4
  %add4 = add i32 %add, %mul3
  ret i32 %add4
; CHECK-LABEL: "?test1@@YtHHF_NIHH@Z":
; CHECK: # %bb.0:                                # %entry
; CHECK: pushl	%esi
; CHECK: movl	12(%esp), %esi
; CHECK: movswl	%dx, %edx
; CHECK: imull	%edx, %eax
; CHECK: xorl	%edx, %edx
; CHECK: testb	%bl, %bl
; CHECK: cmovnel	%ecx, %edx
; CHECK: addl	%edx, %eax
; CHECK: imull	8(%esp), %esi
; CHECK: addl	%esi, %eax
; CHECK: popl	%esi
; CHECK: retl	$8
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local x86_watcallcc noundef i32 @"?test2@@YtHHMF_NI@Z"(i32 inreg noundef %a, float noundef %s, i16 inreg noundef signext %d, i1 inreg noundef zeroext %b, i32 inreg noundef %c) local_unnamed_addr #0 {
entry:
  %conv = sext i16 %d to i32
  %mul = mul nsw i32 %a, %conv
  %conv1 = sitofp i32 %mul to float
  %0 = uitofp i32 %c to float
  %conv4 = select i1 %b, float %0, float 0.000000e+00
  %1 = tail call float @llvm.fmuladd.f32(float %conv4, float %s, float %conv1)
  %conv6 = fptosi float %1 to i32
  ret i32 %conv6
; CHECK-LABEL: "?test2@@YtHHMF_NI@Z":
; CHECK: # %bb.0:                                # %entry
; CHECK: movswl	8(%esp), %ecx
; CHECK: imull	%eax, %ecx
; CHECK: cvtsi2ss	%ecx, %xmm0
; CHECK: cmpb	$0, 12(%esp)
; CHECK: jne	LBB1_1
; CHECK: # %bb.2:                                # %entry
; CHECK: xorps	%xmm1, %xmm1
; CHECK: mulss	4(%esp), %xmm1
; CHECK: addss	%xmm0, %xmm1
; CHECK: cvttss2si	%xmm1, %eax
; CHECK: retl	$16
; CHECK: LBB1_1:
; CHECK: movss	16(%esp), %xmm1                 # xmm1 = mem[0],zero,zero,zero
; CHECK: orpd	__xmm@43300000000000004330000000000000, %xmm1
; CHECK: subsd	__real@4330000000000000, %xmm1
; CHECK: cvtsd2ss	%xmm1, %xmm1
; CHECK: mulss	4(%esp), %xmm1
; CHECK: addss	%xmm0, %xmm1
; CHECK: cvttss2si	%xmm1, %eax
; CHECK: retl	$16
}

; Function Attrs: mustprogress nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.fmuladd.f32(float, float, float) #1

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local x86_watcallcc noundef float @"?test3@@YtMHMF@Z"(i32 inreg noundef %a, float noundef %s, i16 inreg noundef signext %d) local_unnamed_addr #0 {
entry:
  %conv = sext i16 %d to i32
  %mul = mul nsw i32 %a, %conv
  %conv1 = sitofp i32 %mul to float
  %add = fadd float %s, %conv1
  ret float %add
; CHECK-LABEL: "?test3@@YtMHMF@Z":
; CHECK: # %bb.0:                                # %entry
; CHECK: pushl	%eax
; CHECK: movswl	12(%esp), %ecx
; CHECK: imull	%eax, %ecx
; CHECK: cvtsi2ss	%ecx, %xmm0
; CHECK: addss	8(%esp), %xmm0
; CHECK: movss	%xmm0, (%esp)
; CHECK: flds	(%esp)
; CHECK: popl	%eax
; CHECK: retl	$8
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: readwrite)
define dso_local x86_watcallcc void @"?test4@@YtXHAAM@Z"(i32 inreg noundef %a, ptr inreg noundef nonnull align 4 captures(none) dereferenceable(4) %d) local_unnamed_addr #2 {
entry:
  %conv = sitofp i32 %a to float
  %0 = load float, ptr %d, align 4
  %add = fadd float %0, %conv
  store float %add, ptr %d, align 4
  ret void
; CHECK-LABEL: "?test4@@YtXHAAM@Z":
; CHECK: # %bb.0:                                # %entry
; CHECK: cvtsi2ss	%eax, %xmm0
; CHECK: addss	(%edx), %xmm0
; CHECK: movss	%xmm0, (%edx)
; CHECK: retl
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: readwrite)
define dso_local x86_watcallcc void @"?test5@@YtXPAH@Z"(ptr inreg noundef captures(none) %a) local_unnamed_addr #2 {
entry:
  %0 = load i32, ptr %a, align 4
  %mul = shl nsw i32 %0, 1
  store i32 %mul, ptr %a, align 4
  ret void
; CHECK-LABEL: "?test5@@YtXPAH@Z":
; CHECK: # %bb.0:                                # %entry
; CHECK: shll	(%eax)
; CHECK: retl
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local x86_watcallcc noundef i64 @"?test6@@Yt_J_J@Z"(i64 noundef returned %ad) local_unnamed_addr #0 {
entry:
  ret i64 %ad
; CHECK-LABEL: "?test6@@Yt_J_J@Z":
; CHECK: # %bb.0:                                # %entry
; CHECK: retl
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local x86_watcallcc noundef i64 @"?test6@@Yt_JH_JH@Z"(i32 inreg noundef %a, i64 noundef returned %bc, i32 inreg noundef %d) local_unnamed_addr #0 {
entry:
  ret i64 %bc
; CHECK-LABEL: "?test6@@Yt_JH_JH@Z":
; CHECK: # %bb.0:                                # %entry
; CHECK: movl	%ebx, %eax
; CHECK: movl	%ecx, %edx
; CHECK: retl
}
; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local x86_watcallcc noundef float @"?test7@@YtMM_J@Z"(float noundef %s4, i64 noundef %s8) local_unnamed_addr #0 {
entry:
  %conv = sitofp i64 %s8 to float
  %add = fadd float %s4, %conv
  ret float %add
; CHECK-LABEL: "?test7@@YtMM_J@Z":
; CHECK: # %bb.0:                                # %entry
; CHECK: subl	$8, %esp
; CHECK: movss	12(%esp), %xmm0                 # xmm0 = mem[0],zero,zero,zero
; CHECK: fildll	16(%esp)
; CHECK: fstps	4(%esp)
; CHECK: addss	4(%esp), %xmm0
; CHECK: movss	%xmm0, (%esp)
; CHECK: flds	(%esp)
; CHECK: addl	$8, %esp
; CHECK: retl	$12
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local x86_watcallcc noundef i64 @"?test8@@Yt_J_J0@Z"(i64 noundef %ad, i64 noundef %bc) local_unnamed_addr #0 {
entry:
  %add = add nsw i64 %bc, %ad
  ret i64 %add
; CHECK-LABEL: "?test8@@Yt_J_J0@Z":
; CHECK: # %bb.0:                                # %entry
; CHECK: addl	%ebx, %eax
; CHECK: adcl	%ecx, %edx
; CHECK: retl
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: readwrite) "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !1, producer: "clang version 23.0.0git (https://github.com/widberg/llvm-project-widberg-extensions.git ebe2560aa8da57f8b8c0952074325845ddc01a4e)", isOptimized: true, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.cpp", directory: "D:\\programming\\widberg\\llvm-project-widberg-extensions\\build")
!2 = !{i32 1, !"NumRegisterParameters", i32 0}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 2}
!5 = !{i32 1, !"MaxTLSAlign", i32 65536}
!6 = !{!"clang version 23.0.0git (https://github.com/widberg/llvm-project-widberg-extensions.git ebe2560aa8da57f8b8c0952074325845ddc01a4e)"}
