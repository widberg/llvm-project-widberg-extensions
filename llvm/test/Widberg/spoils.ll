; RUN: llc < %s -O=3 -mtriple=i386-unknown-unknown | FileCheck %s

define dso_local usercallcc void @spoil() local_unnamed_addr "spoils"="edx" {
entry:
; CHECK-LABEL: spoil:
; CHECK: pushl	%esi
; CHECK: pushl	%eax
; CHECK: nop
; CHECK: popl	%eax
; CHECK: popl	%esi
; CHECK: retl
  tail call void asm sideeffect "nop", "~{eax},~{edx},~{esi},~{dirflag},~{fpsr},~{flags}"(), !srcloc !5
  ret void
}

define dso_local usercallcc void @spoil_empty() local_unnamed_addr "spoils" {
entry:
; CHECK-LABEL: spoil_empty:
; CHECK: pushl	%esi
; CHECK: pushl	%edx
; CHECK: pushl	%eax
; CHECK: nop
; CHECK: popl	%eax
; CHECK: popl	%edx
; CHECK: popl	%esi
; CHECK: retl
  tail call void asm sideeffect "nop", "~{eax},~{edx},~{esi},~{dirflag},~{fpsr},~{flags}"(), !srcloc !5
  ret void
}

define dso_local void @spoil_cdecl(i32 %x) local_unnamed_addr "spoils"="edx" {
entry:
; CHECK-LABEL: spoil_cdecl:
; CHECK: pushl	%esi
; CHECK: pushl	%eax
; CHECK: nop
; CHECK: popl	%eax
; CHECK: popl	%esi
; CHECK: retl
  tail call void asm sideeffect "nop", "~{eax},~{edx},~{esi},~{dirflag},~{fpsr},~{flags}"(), !srcloc !5
  ret void
}

define dso_local x86_stdcallcc void @spoil_stdcall(i32 %x) local_unnamed_addr "spoils"="edx" {
entry:
; CHECK-LABEL: spoil_stdcall:
; CHECK: pushl	%esi
; CHECK: pushl	%eax
; CHECK: nop
; CHECK: popl	%eax
; CHECK: popl	%esi
; CHECK: retl	$4
  tail call void asm sideeffect "nop", "~{eax},~{edx},~{esi},~{dirflag},~{fpsr},~{flags}"(), !srcloc !5
  ret void
}

define dso_local x86_stdcallcc void @spoil_stdcall_empty(i32 %x) local_unnamed_addr "spoils" {
entry:
; CHECK-LABEL: spoil_stdcall_empty:
; CHECK: pushl	%esi
; CHECK: pushl	%edx
; CHECK: pushl	%eax
; CHECK: nop
; CHECK: popl	%eax
; CHECK: popl	%edx
; CHECK: popl	%esi
; CHECK: retl	$4
  tail call void asm sideeffect "nop", "~{eax},~{edx},~{esi},~{dirflag},~{fpsr},~{flags}"(), !srcloc !5
  ret void
}

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 16.0.6 (https://github.com/llvm/llvm-project.git 7cbf1a2591520c2491aa35339f227775f4d3adf6)"}
!5 = !{i64 62}
