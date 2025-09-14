; RUN: llc < %s -O=3 -mtriple=i386-unknown-unknown | FileCheck %s

define dso_local userpurgecc signext "widberg_location"="al" i8 @_0x005281C0(ptr noundef "widberg_location"="esi" %a1, i32 noundef "widberg_location"="edi" %edi0, ptr noundef %a2) #0 {
; CHECK-LABEL: _0x005281C0:
entry:
  %a1.addr = alloca ptr, align 4
  %edi0.addr = alloca i32, align 4
  %a2.addr = alloca ptr, align 4
  %x = alloca float, align 4
  store ptr %a1, ptr %a1.addr, align 4
  store i32 %edi0, ptr %edi0.addr, align 4
  store ptr %a2, ptr %a2.addr, align 4
  store float 0.000000e+00, ptr %x, align 4
  %call = call usercallcc "widberg_location"="fp0" double @_0x00445500(ptr noundef "widberg_location"="eax" %x, i32 noundef "widberg_location"="ecx" 0)
  %cmp = fcmp oeq double %call, 0.000000e+00
  %conv = zext i1 %cmp to i8
  ret i8 %conv
}

declare usercallcc "widberg_location"="fp0" double @_0x00445500(ptr noundef "widberg_location"="eax", i32 noundef "widberg_location"="ecx") #1

attributes #0 = { mustprogress noinline nounwind optnone "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+x87" }
attributes #1 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+x87" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{!"clang version 22.0.0git (https://github.com/widberg/llvm-project-widberg-extensions.git 2f5bae9f65035657418c80ab0d2ea619dffa9b34)"}
