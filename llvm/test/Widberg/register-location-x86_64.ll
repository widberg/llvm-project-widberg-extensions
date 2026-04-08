; RUN: llc < %s -O=3 -mtriple=x86_64-pc-windows-msvc | FileCheck %s

; x86_64 Widberg locations should support high GPR/XMM names and 64-bit pieces.

define dso_local usercallcc "widberg_location"="rax" i64 @ret_i64_from_r8(i64 noundef "widberg_location"="r8" %x) local_unnamed_addr {
entry:
; CHECK-LABEL: ret_i64_from_r8:
; CHECK: movq	%r8, %rax
; CHECK: retq
  ret i64 %x
}

define dso_local usercallcc "widberg_location"="rax,rdx" i128 @ret_i128_from_r8_r9(i128 noundef "widberg_location"="r8,r9" %x) local_unnamed_addr {
entry:
; CHECK-LABEL: ret_i128_from_r8_r9:
; CHECK: movq	%r9, %rdx
; CHECK: movq	%r8, %rax
; CHECK: retq
  ret i128 %x
}

define dso_local usercallcc "widberg_location"="xmm8" double @ret_f64_from_xmm9(double noundef "widberg_location"="xmm9" %x) local_unnamed_addr {
entry:
; CHECK-LABEL: ret_f64_from_xmm9:
; CHECK: movaps	%xmm9, %xmm8
; CHECK: retq
  ret double %x
}

define dso_local usercallcc "widberg_location"="rax" ptr @call_ptr_in_r10(ptr noundef "widberg_location"="r10" %fn, i64 noundef "widberg_location"="r9" %arg) local_unnamed_addr {
entry:
; CHECK-LABEL: call_ptr_in_r10:
; CHECK: callq	*%r10
; CHECK: retq
  %call = call usercallcc noundef "widberg_location"="rax" ptr %fn(i64 noundef "widberg_location"="r9" %arg)
  ret ptr %call
}
