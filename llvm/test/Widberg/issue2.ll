; RUN: llc < %s -O=3 -mtriple=i386-unknown-unknown | FileCheck %s

%union.Tu = type { i16 }

define dso_local void @c(ptr dead_on_unwind noalias writable sret(%union.Tu) align 2 %agg.result, i16 noundef signext %x.coerce) #0 {
entry:
; CHECK-LABEL: c:
; CHECK: pushl %eax
; CHECK: movl 8(%esp), %eax
; CHECK: movw 12(%esp), %cx
; CHECK: movw %cx, 2(%esp)
; CHECK: movw 2(%esp), %cx
; CHECK: movw %cx, (%eax)
; CHECK: popl %ecx
; CHECK: retl $4
  %result.ptr = alloca ptr, align 4
  %x = alloca %union.Tu, align 2
  store ptr %agg.result, ptr %result.ptr, align 4
  %coerce.dive = getelementptr inbounds nuw %union.Tu, ptr %x, i32 0, i32 0
  store i16 %x.coerce, ptr %coerce.dive, align 2
  call void @llvm.memcpy.p0.p0.i32(ptr align 2 %agg.result, ptr align 2 %x, i32 2, i1 false)
  ret void
}

declare void @llvm.memcpy.p0.p0.i32(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i32, i1 immarg) #1

attributes #0 = { noinline nounwind optnone "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+x87" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{!"clang version 22.0.0git (C:/Users/widberg/AppData/Local/nullable/lappverk/data/work/2fb41ecc39195040e9c250356d238ce798cdb7e5cfe95eacea77795df481518d/worktree/main/clang 17a49c405b63ccb1925a838710547cee1821cf93)"}
