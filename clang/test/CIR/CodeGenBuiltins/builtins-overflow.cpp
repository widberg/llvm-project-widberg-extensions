// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -fclangir -emit-cir %s -o %t.cir
// RUN: FileCheck %s --check-prefix=CIR --input-file=%t.cir
// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -fclangir -emit-llvm %s -o %t-cir.ll
// RUN: FileCheck %s --check-prefix=LLVM --input-file=%t-cir.ll
// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -emit-llvm %s -o %t.ll
// RUN: FileCheck %s --check-prefix=OGCG --input-file=%t.ll

bool test_add_overflow_uint_uint_uint(unsigned x, unsigned y, unsigned *res) {
  return __builtin_add_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z32test_add_overflow_uint_uint_uintjjPj
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u32i>>, !cir.ptr<!u32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#LHS]], %[[#RHS]]) : !u32i, (!u32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u32i, !cir.ptr<!u32i>
//      CIR: }

// LLVM: define{{.*}} i1 @_Z32test_add_overflow_uint_uint_uintjjPj(i32{{.*}}, i32{{.*}}, ptr{{.*}})
// LLVM:   call { i32, i1 } @llvm.uadd.with.overflow.i32(i32 %{{.+}}, i32 %{{.+}})

// OGCG: define{{.*}} i1 @_Z32test_add_overflow_uint_uint_uintjjPj(i32{{.*}}, i32{{.*}}, ptr{{.*}})
// OGCG:   call { i32, i1 } @llvm.uadd.with.overflow.i32(i32 %{{.+}}, i32 %{{.+}})

bool test_add_overflow_bool_bool_uint(bool x, bool y, unsigned *res) {
  return __builtin_add_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @{{.*}}test_add_overflow_bool_bool_uint{{.*}}
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.bool>, !cir.bool
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.bool>, !cir.bool
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u32i>>, !cir.ptr<!u32i>
// CIR-NEXT:   %[[#PROM_X:]] = cir.cast bool_to_int %[[#X]] : !cir.bool -> !u32i
// CIR-NEXT:   %[[#PROM_Y:]] = cir.cast bool_to_int %[[#Y]] : !cir.bool -> !u32i
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#PROM_X]], %[[#PROM_Y]]) : !u32i, (!u32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u32i, !cir.ptr<!u32i>
//      CIR: }

bool test_uadd_overflow_bool_bool_uint(bool x, bool y, unsigned *res) {
  return __builtin_uadd_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @{{.*}}test_uadd_overflow_bool_bool_uint{{.*}}
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.bool>, !cir.bool
//      CIR:   %[[#PROM_X:]] = cir.cast bool_to_int %[[#X]] : !cir.bool -> !u32i
//      CIR:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.bool>, !cir.bool
//      CIR:   %[[#PROM_Y:]] = cir.cast bool_to_int %[[#Y]] : !cir.bool -> !u32i
//      CIR:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u32i>>, !cir.ptr<!u32i>
//      CIR:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#PROM_X]], %[[#PROM_Y]]) : !u32i, (!u32i, !cir.bool)
//      CIR:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u32i, !cir.ptr<!u32i>
//      CIR: }

// LLVM: define{{.*}} i1 @{{.*}}test_uadd_overflow_bool_bool_uint{{.*}}
// LLVM:   %[[#XZ:]] = zext i1 %{{.*}} to i32
// LLVM:   %[[#YZ:]] = zext i1 %{{.*}} to i32
// LLVM:   call { i32, i1 } @llvm.uadd.with.overflow.i32(i32 %[[#XZ]], i32 %[[#YZ]])

// OGCG-LABEL: define{{.*}} i1 @{{.*}}test_uadd_overflow_bool_bool_uint{{.*}}
// OGCG-DAG:   zext i1 %{{.*}} to i32
// OGCG-DAG:   zext i1 %{{.*}} to i32
// OGCG:       call { i32, i1 } @llvm.uadd.with.overflow.i32

bool test_add_overflow_uint_uint_bool(unsigned x, unsigned y, bool *res) {
  return __builtin_add_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @{{.*}}test_add_overflow_uint_uint_bool{{.*}}
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.bool>>, !cir.ptr<!cir.bool>
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(add, %[[#X]], %[[#Y]]) : !u32i, (!u32i, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !u32i -> !cir.int<u, 1>
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !cir.int<u, 1> -> !u32i
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !u32i, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   %[[BOOL_RES:.+]] = cir.cast int_to_bool %[[TRUNC]] : !cir.int<u, 1> -> !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[BOOL_RES]], %[[#RES_PTR]] : !cir.bool, !cir.ptr<!cir.bool>
//      CIR: }

bool test_add_overflow_int_int_int(int x, int y, int *res) {
  return __builtin_add_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z29test_add_overflow_int_int_intiiPi
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s32i>>, !cir.ptr<!s32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#LHS]], %[[#RHS]]) : !s32i, (!s32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s32i, !cir.ptr<!s32i>
//      CIR: }

bool test_add_overflow_xint31_xint31_xint31(_BitInt(31) x, _BitInt(31) y, _BitInt(31) *res) {
  return __builtin_add_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z38test_add_overflow_xint31_xint31_xint31DB31_S_PS_
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 31>>, !cir.int<s, 31>
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 31>>, !cir.int<s, 31>
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.int<s, 31>>>, !cir.ptr<!cir.int<s, 31>>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#LHS]], %[[#RHS]]) : !cir.int<s, 31>, (!cir.int<s, 31>, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !cir.int<s, 31>, !cir.ptr<!cir.int<s, 31>>
//      CIR: }

bool test_add_overflow_xint127_xint127_xint127(_BitInt(127) x, _BitInt(127) y, _BitInt(127) *res) {
  return __builtin_add_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z41test_add_overflow_xint127_xint127_xint127DB127_S_PS_
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 127>>, !cir.int<s, 127>
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 127>>, !cir.int<s, 127>
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.int<s, 127>>>, !cir.ptr<!cir.int<s, 127>>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#LHS]], %[[#RHS]]) : !cir.int<s, 127>, (!cir.int<s, 127>, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !cir.int<s, 127>, !cir.ptr<!cir.int<s, 127>>
//      CIR: }

bool test_add_overflow_uxint128_uxint128_uxint128(unsigned _BitInt(128) x, unsigned _BitInt(128) y, unsigned _BitInt(128) *res) {
  return __builtin_add_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z44test_add_overflow_uxint128_uxint128_uxint128DU128_S_PS_
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u128i>, !u128i
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u128i>, !u128i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u128i>>, !cir.ptr<!u128i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#LHS]], %[[#RHS]]) : !u128i, (!u128i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u128i, !cir.ptr<!u128i>
//      CIR: }

bool test_add_overflow_xint127_uxint127_uxint127(_BitInt(127) x, unsigned _BitInt(127) y, unsigned _BitInt(127) *res) {
  return __builtin_add_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z43test_add_overflow_xint127_uxint127_uxint127DB127_DU127_PS0_
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 127>>, !cir.int<s, 127>
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<u, 127>>, !cir.int<u, 127>
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.int<u, 127>>>, !cir.ptr<!cir.int<u, 127>>
// CIR-NEXT:   %[[#PROM_X:]] = cir.cast integral %[[#X]] : !cir.int<s, 127> -> !s128i
// CIR-NEXT:   %[[#PROM_Y:]] = cir.cast integral %[[#Y]] : !cir.int<u, 127> -> !s128i
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(add, %[[#PROM_X]], %[[#PROM_Y]]) : !s128i, (!s128i, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !s128i -> !cir.int<u, 127>
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !cir.int<u, 127> -> !s128i
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !s128i, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[TRUNC]], %[[#RES_PTR]] : !cir.int<u, 127>, !cir.ptr<!cir.int<u, 127>>
//      CIR: }

bool test_sub_overflow_uint_uint_uint(unsigned x, unsigned y, unsigned *res) {
  return __builtin_sub_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z32test_sub_overflow_uint_uint_uintjjPj
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u32i>>, !cir.ptr<!u32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(sub, %[[#LHS]], %[[#RHS]]) : !u32i, (!u32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u32i, !cir.ptr<!u32i>
//      CIR: }

bool test_sub_overflow_uint_uint_bool(unsigned x, unsigned y, bool *res) {
  return __builtin_sub_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @{{.*}}test_sub_overflow_uint_uint_bool{{.*}}
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.bool>>, !cir.ptr<!cir.bool>
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(sub, %[[#X]], %[[#Y]]) : !u32i, (!u32i, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !u32i -> !cir.int<u, 1>
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !cir.int<u, 1> -> !u32i
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !u32i, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   %[[BOOL_RES:.+]] = cir.cast int_to_bool %[[TRUNC]] : !cir.int<u, 1> -> !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[BOOL_RES]], %[[#RES_PTR]] : !cir.bool, !cir.ptr<!cir.bool>
//      CIR: }

bool test_sub_overflow_int_int_int(int x, int y, int *res) {
  return __builtin_sub_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z29test_sub_overflow_int_int_intiiPi
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s32i>>, !cir.ptr<!s32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(sub, %[[#LHS]], %[[#RHS]]) : !s32i, (!s32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s32i, !cir.ptr<!s32i>
//      CIR: }

bool test_sub_overflow_xint31_xint31_xint31(_BitInt(31) x, _BitInt(31) y, _BitInt(31) *res) {
  return __builtin_sub_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z38test_sub_overflow_xint31_xint31_xint31DB31_S_PS_
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 31>>, !cir.int<s, 31>
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 31>>, !cir.int<s, 31>
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.int<s, 31>>>, !cir.ptr<!cir.int<s, 31>>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(sub, %[[#LHS]], %[[#RHS]]) : !cir.int<s, 31>, (!cir.int<s, 31>, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !cir.int<s, 31>, !cir.ptr<!cir.int<s, 31>>
//      CIR: }

bool test_sub_overflow_xint127_xint127_xint127(_BitInt(127) x, _BitInt(127) y, _BitInt(127) *res) {
  return __builtin_sub_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z41test_sub_overflow_xint127_xint127_xint127DB127_S_PS_
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 127>>, !cir.int<s, 127>
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 127>>, !cir.int<s, 127>
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.int<s, 127>>>, !cir.ptr<!cir.int<s, 127>>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(sub, %[[#LHS]], %[[#RHS]]) : !cir.int<s, 127>, (!cir.int<s, 127>, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !cir.int<s, 127>, !cir.ptr<!cir.int<s, 127>>
//      CIR: }

bool test_sub_overflow_uxint128_uxint128_uxint128(unsigned _BitInt(128) x, unsigned _BitInt(128) y, unsigned _BitInt(128) *res) {
  return __builtin_sub_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z44test_sub_overflow_uxint128_uxint128_uxint128DU128_S_PS_
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u128i>, !u128i
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u128i>, !u128i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u128i>>, !cir.ptr<!u128i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(sub, %[[#LHS]], %[[#RHS]]) : !u128i, (!u128i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u128i, !cir.ptr<!u128i>
//      CIR: }

bool test_sub_overflow_xint127_uxint127_uxint127(_BitInt(127) x, unsigned _BitInt(127) y, unsigned _BitInt(127) *res) {
  return __builtin_sub_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z43test_sub_overflow_xint127_uxint127_uxint127DB127_DU127_PS0_
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 127>>, !cir.int<s, 127>
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<u, 127>>, !cir.int<u, 127>
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.int<u, 127>>>, !cir.ptr<!cir.int<u, 127>>
// CIR-NEXT:   %[[#PROM_X:]] = cir.cast integral %[[#X]] : !cir.int<s, 127> -> !s128i
// CIR-NEXT:   %[[#PROM_Y:]] = cir.cast integral %[[#Y]] : !cir.int<u, 127> -> !s128i
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(sub, %[[#PROM_X]], %[[#PROM_Y]]) : !s128i, (!s128i, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !s128i -> !cir.int<u, 127>
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !cir.int<u, 127> -> !s128i
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !s128i, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[TRUNC]], %[[#RES_PTR]] : !cir.int<u, 127>, !cir.ptr<!cir.int<u, 127>>
//      CIR: }

bool test_mul_overflow_uint_uint_uint(unsigned x, unsigned y, unsigned *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z32test_mul_overflow_uint_uint_uintjjPj
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u32i>>, !cir.ptr<!u32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#LHS]], %[[#RHS]]) : !u32i, (!u32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u32i, !cir.ptr<!u32i>
//      CIR: }

bool test_mul_overflow_uint_uint_int(unsigned x, unsigned y, int *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @{{.*}}test_mul_overflow_uint_uint_int{{.*}}
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s32i>>, !cir.ptr<!s32i>
// CIR-NEXT:   %[[#PROM_X:]] = cir.cast integral %[[#X]] : !u32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[#PROM_Y:]] = cir.cast integral %[[#Y]] : !u32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(mul, %[[#PROM_X]], %[[#PROM_Y]]) : !cir.int<s, 33>, (!cir.int<s, 33>, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !cir.int<s, 33> -> !s32i
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !s32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !cir.int<s, 33>, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[TRUNC]], %[[#RES_PTR]] : !s32i, !cir.ptr<!s32i>
//      CIR: }

// LLVM: define{{.*}} i1 @{{.*}}test_mul_overflow_uint_uint_int{{.*}}
// LLVM:   %[[#X:]] = zext i32 %{{.*}} to i33
// LLVM:   %[[#Y:]] = zext i32 %{{.*}} to i33
// LLVM:   %[[#CALL:]] = call { i33, i1 } @llvm.smul.with.overflow.i33(i33 %[[#X]], i33 %[[#Y]])
// LLVM:   %[[#RES:]] = extractvalue { i33, i1 } %[[#CALL]], 0
// LLVM:   %[[#OV:]] = extractvalue { i33, i1 } %[[#CALL]], 1
// LLVM:   %[[#TRUNC:]] = trunc i33 %[[#RES]] to i32
// LLVM:   %[[#TRUNC_EXT:]] = sext i32 %[[#TRUNC]] to i33
// LLVM:   %[[#TRUNC_OV:]] = icmp ne i33 %[[#RES]], %[[#TRUNC_EXT]]
// LLVM:   or i1 %[[#OV]], %[[#TRUNC_OV]]

// OGCG-LABEL: define{{.*}} i1 @{{.*}}test_mul_overflow_uint_uint_int{{.*}}
// OGCG:       %[[#CALL:]] = call { i32, i1 } @llvm.umul.with.overflow.i32(i32 %{{.*}}, i32 %{{.*}})
// OGCG:       %[[#OV:]] = extractvalue { i32, i1 } %[[#CALL]], 1
// OGCG:       %[[#RES:]] = extractvalue { i32, i1 } %[[#CALL]], 0
// OGCG:       %[[#OV2:]] = icmp ugt i32 %[[#RES]], 2147483647
// OGCG:       or i1 %[[#OV]], %[[#OV2]]

bool test_mul_overflow_int_uint_int(int x, unsigned y, int *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @{{.*}}test_mul_overflow_int_uint_int{{.*}}
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s32i>>, !cir.ptr<!s32i>
// CIR-NEXT:   %[[#PROM_X:]] = cir.cast integral %[[#X]] : !s32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[#PROM_Y:]] = cir.cast integral %[[#Y]] : !u32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(mul, %[[#PROM_X]], %[[#PROM_Y]]) : !cir.int<s, 33>, (!cir.int<s, 33>, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !cir.int<s, 33> -> !s32i
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !s32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !cir.int<s, 33>, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[TRUNC]], %[[#RES_PTR]] : !s32i, !cir.ptr<!s32i>
//      CIR: }

// LLVM: define{{.*}} i1 @{{.*}}test_mul_overflow_int_uint_int{{.*}}
// LLVM:   %[[#CALL:]] = call { i32, i1 } @llvm.umul.with.overflow.i32(i32 %{{.*}}, i32 %{{.*}})
// LLVM:   %[[#RES:]] = extractvalue { i32, i1 } %[[#CALL]], 0
// LLVM:   %[[#OV:]] = extractvalue { i32, i1 } %[[#CALL]], 1
// LLVM:   %[[#TRUNC:]] = trunc i32 %[[#RES]] to i1
// LLVM:   %[[#TRUNC_EXT:]] = zext i1 %[[#TRUNC]] to i32
// LLVM:   %[[#TRUNC_OV:]] = icmp ne i32 %[[#RES]], %[[#TRUNC_EXT]]
// LLVM:   or i1 %[[#OV]], %[[#TRUNC_OV]]

// OGCG-LABEL: define{{.*}} i1 @{{.*}}test_mul_overflow_int_uint_int{{.*}}
// OGCG-DAG:   %[[#NEG:]] = icmp slt i32 %{{.*}}, 0
// OGCG-DAG:   %[[#NEG_ZEXT:]] = zext i1 %[[#NEG]] to i32
// OGCG-DAG:   %[[#MAX:]] = add i32 2147483647, %[[#NEG_ZEXT]]
// OGCG:       %[[#CALL:]] = call { i32, i1 } @llvm.umul.with.overflow.i32(i32 %{{.*}}, i32 %{{.*}})
// OGCG:       %[[#OV:]] = extractvalue { i32, i1 } %[[#CALL]], 1
// OGCG:       %[[#RES:]] = extractvalue { i32, i1 } %[[#CALL]], 0
// OGCG:       %[[#OV2:]] = or i1 %[[#OV]], %{{.*}}

bool test_mul_overflow_uint_uint_bool(unsigned x, unsigned y, bool *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @{{.*}}test_mul_overflow_uint_uint_bool{{.*}}
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.bool>>, !cir.ptr<!cir.bool>
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(mul, %[[#X]], %[[#Y]]) : !u32i, (!u32i, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !u32i -> !cir.int<u, 1>
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !cir.int<u, 1> -> !u32i
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !u32i, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   %[[BOOL_RES:.+]] = cir.cast int_to_bool %[[TRUNC]] : !cir.int<u, 1> -> !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[BOOL_RES]], %[[#RES_PTR]] : !cir.bool, !cir.ptr<!cir.bool>
//      CIR: }

bool test_mul_overflow_int_int_int(int x, int y, int *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z29test_mul_overflow_int_int_intiiPi
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s32i>>, !cir.ptr<!s32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#LHS]], %[[#RHS]]) : !s32i, (!s32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s32i, !cir.ptr<!s32i>
//      CIR: }

bool test_mul_overflow_xint31_xint31_xint31(_BitInt(31) x, _BitInt(31) y, _BitInt(31) *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z38test_mul_overflow_xint31_xint31_xint31DB31_S_PS_
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 31>>, !cir.int<s, 31>
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 31>>, !cir.int<s, 31>
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.int<s, 31>>>, !cir.ptr<!cir.int<s, 31>>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#LHS]], %[[#RHS]]) : !cir.int<s, 31>, (!cir.int<s, 31>, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !cir.int<s, 31>, !cir.ptr<!cir.int<s, 31>>
//      CIR: }

bool test_mul_overflow_xint127_xint127_xint127(_BitInt(127) x, _BitInt(127) y, _BitInt(127) *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z41test_mul_overflow_xint127_xint127_xint127DB127_S_PS_
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 127>>, !cir.int<s, 127>
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 127>>, !cir.int<s, 127>
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.int<s, 127>>>, !cir.ptr<!cir.int<s, 127>>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#LHS]], %[[#RHS]]) : !cir.int<s, 127>, (!cir.int<s, 127>, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !cir.int<s, 127>, !cir.ptr<!cir.int<s, 127>>
//      CIR: }

bool test_mul_overflow_uxint128_uxint128_uxint128(unsigned _BitInt(128) x, unsigned _BitInt(128) y, unsigned _BitInt(128) *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z44test_mul_overflow_uxint128_uxint128_uxint128DU128_S_PS_
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u128i>, !u128i
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u128i>, !u128i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u128i>>, !cir.ptr<!u128i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#LHS]], %[[#RHS]]) : !u128i, (!u128i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u128i, !cir.ptr<!u128i>
//      CIR: }

bool test_mul_overflow_xint127_uxint127_uxint127(_BitInt(127) x, unsigned _BitInt(127) y, unsigned _BitInt(127) *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z43test_mul_overflow_xint127_uxint127_uxint127DB127_DU127_PS0_
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<s, 127>>, !cir.int<s, 127>
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.int<u, 127>>, !cir.int<u, 127>
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!cir.int<u, 127>>>, !cir.ptr<!cir.int<u, 127>>
// CIR-NEXT:   %[[#PROM_X:]] = cir.cast integral %[[#X]] : !cir.int<s, 127> -> !s128i
// CIR-NEXT:   %[[#PROM_Y:]] = cir.cast integral %[[#Y]] : !cir.int<u, 127> -> !s128i
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(mul, %[[#PROM_X]], %[[#PROM_Y]]) : !s128i, (!s128i, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !s128i -> !cir.int<u, 127>
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !cir.int<u, 127> -> !s128i
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !s128i, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[TRUNC]], %[[#RES_PTR]] : !cir.int<u, 127>, !cir.ptr<!cir.int<u, 127>>
//      CIR: }

bool test_mul_overflow_ulong_ulong_long(unsigned long x, unsigned long y, unsigned long *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z34test_mul_overflow_ulong_ulong_longmmPm
//      CIR:   %[[#LHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#RHS:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u64i>>, !cir.ptr<!u64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#LHS]], %[[#RHS]]) : !u64i, (!u64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u64i, !cir.ptr<!u64i>
//      CIR: }

bool test_mul_overflow_ulong_ulong_long_signed(unsigned long x, unsigned long y, long *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @{{.*}}test_mul_overflow_ulong_ulong_long_signed{{.*}}
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s64i>>, !cir.ptr<!s64i>
// CIR-NEXT:   %[[#PROM_X:]] = cir.cast integral %[[#X]] : !u64i -> !cir.int<s, 65>
// CIR-NEXT:   %[[#PROM_Y:]] = cir.cast integral %[[#Y]] : !u64i -> !cir.int<s, 65>
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(mul, %[[#PROM_X]], %[[#PROM_Y]]) : !cir.int<s, 65>, (!cir.int<s, 65>, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !cir.int<s, 65> -> !s64i
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !s64i -> !cir.int<s, 65>
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !cir.int<s, 65>, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[TRUNC]], %[[#RES_PTR]] : !s64i, !cir.ptr<!s64i>
//      CIR: }

// LLVM: define{{.*}} i1 @{{.*}}test_mul_overflow_ulong_ulong_long_signed{{.*}}
// LLVM:   %[[#X:]] = zext i64 %{{.*}} to i65
// LLVM:   %[[#Y:]] = zext i64 %{{.*}} to i65
// LLVM:   %[[#CALL:]] = call { i65, i1 } @llvm.smul.with.overflow.i65(i65 %[[#X]], i65 %[[#Y]])
// LLVM:   %[[#RES:]] = extractvalue { i65, i1 } %[[#CALL]], 0
// LLVM:   %[[#OV:]] = extractvalue { i65, i1 } %[[#CALL]], 1
// LLVM:   %[[#TRUNC:]] = trunc i65 %[[#RES]] to i64
// LLVM:   %[[#TRUNC_EXT:]] = sext i64 %[[#TRUNC]] to i65
// LLVM:   %[[#TRUNC_OV:]] = icmp ne i65 %[[#RES]], %[[#TRUNC_EXT]]
// LLVM:   or i1 %[[#OV]], %[[#TRUNC_OV]]

// OGCG-LABEL: define{{.*}} i1 @{{.*}}test_mul_overflow_ulong_ulong_long_signed{{.*}}
// OGCG:       %[[#CALL:]] = call { i64, i1 } @llvm.umul.with.overflow.i64(i64 %{{.*}}, i64 %{{.*}})
// OGCG:       %[[#OV:]] = extractvalue { i64, i1 } %[[#CALL]], 1
// OGCG:       %[[#RES:]] = extractvalue { i64, i1 } %[[#CALL]], 0
// OGCG:       %[[#OV2:]] = icmp ugt i64 %[[#RES]], 9223372036854775807
// OGCG:       or i1 %[[#OV]], %[[#OV2]]

bool test_add_overflow_uint_int_int(unsigned x, int y, int *res) {
  return __builtin_add_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z30test_add_overflow_uint_int_intjiPi
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s32i>>, !cir.ptr<!s32i>
// CIR-NEXT:   %[[#PROM_X:]] = cir.cast integral %[[#X]] : !u32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[#PROM_Y:]] = cir.cast integral %[[#Y]] : !s32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(add, %[[#PROM_X]], %[[#PROM_Y]]) : !cir.int<s, 33>, (!cir.int<s, 33>, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !cir.int<s, 33> -> !s32i
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !s32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !cir.int<s, 33>, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[TRUNC]], %[[#RES_PTR]] : !s32i, !cir.ptr<!s32i>
//      CIR: }

bool test_add_overflow_int_uint_uint(int x, unsigned y, unsigned *res) {
  return __builtin_add_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @{{.*}}test_add_overflow_int_uint_uint{{.*}}
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u32i>>, !cir.ptr<!u32i>
// CIR-NEXT:   %[[#PROM_X:]] = cir.cast integral %[[#X]] : !s32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[#PROM_Y:]] = cir.cast integral %[[#Y]] : !u32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(add, %[[#PROM_X]], %[[#PROM_Y]]) : !cir.int<s, 33>, (!cir.int<s, 33>, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !cir.int<s, 33> -> !u32i
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !u32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !cir.int<s, 33>, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[TRUNC]], %[[#RES_PTR]] : !u32i, !cir.ptr<!u32i>
//      CIR: }

// LLVM: define{{.*}} i1 @{{.*}}test_add_overflow_int_uint_uint{{.*}}
// LLVM:   %[[#X:]] = sext i32 %{{.*}} to i33
// LLVM:   %[[#Y:]] = zext i32 %{{.*}} to i33
// LLVM:   %[[#CALL:]] = call { i33, i1 } @llvm.sadd.with.overflow.i33(i33 %[[#X]], i33 %[[#Y]])
// LLVM:   %[[#RES:]] = extractvalue { i33, i1 } %[[#CALL]], 0
// LLVM:   %[[#OV:]] = extractvalue { i33, i1 } %[[#CALL]], 1
// LLVM:   %[[#TRUNC:]] = trunc i33 %[[#RES]] to i32
// LLVM:   %[[#TRUNC_EXT:]] = zext i32 %[[#TRUNC]] to i33
// LLVM:   %[[#TRUNC_OV:]] = icmp ne i33 %[[#RES]], %[[#TRUNC_EXT]]
// LLVM:   or i1 %[[#OV]], %[[#TRUNC_OV]]

// OGCG-LABEL: define{{.*}} i1 @{{.*}}test_add_overflow_int_uint_uint{{.*}}
// OGCG:       %[[#X:]] = sext i32 %{{.*}} to i33
// OGCG:       %[[#Y:]] = zext i32 %{{.*}} to i33
// OGCG:       %[[#CALL:]] = call { i33, i1 } @llvm.sadd.with.overflow.i33(i33 %[[#X]], i33 %[[#Y]])
// OGCG-DAG:   %[[#OV:]] = extractvalue { i33, i1 } %[[#CALL]], 1
// OGCG-DAG:   %[[#RES:]] = extractvalue { i33, i1 } %[[#CALL]], 0
// OGCG:       %[[#TRUNC:]] = trunc i33 %[[#RES]] to i32
// OGCG:       %[[#TRUNC_EXT:]] = zext i32 %[[#TRUNC]] to i33
// OGCG:       %[[#TRUNC_OV:]] = icmp ne i33 %[[#RES]], %[[#TRUNC_EXT]]
// OGCG:       or i1 %[[#OV]], %[[#TRUNC_OV]]

bool test_sub_overflow_int_uint_uint(int x, unsigned y, unsigned *res) {
  return __builtin_sub_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @{{.*}}test_sub_overflow_int_uint_uint{{.*}}
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u32i>>, !cir.ptr<!u32i>
// CIR-NEXT:   %[[#PROM_X:]] = cir.cast integral %[[#X]] : !s32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[#PROM_Y:]] = cir.cast integral %[[#Y]] : !u32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(sub, %[[#PROM_X]], %[[#PROM_Y]]) : !cir.int<s, 33>, (!cir.int<s, 33>, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !cir.int<s, 33> -> !u32i
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !u32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !cir.int<s, 33>, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[TRUNC]], %[[#RES_PTR]] : !u32i, !cir.ptr<!u32i>
//      CIR: }

// LLVM: define{{.*}} i1 @{{.*}}test_sub_overflow_int_uint_uint{{.*}}
// LLVM:   %[[#X:]] = sext i32 %{{.*}} to i33
// LLVM:   %[[#Y:]] = zext i32 %{{.*}} to i33
// LLVM:   %[[#CALL:]] = call { i33, i1 } @llvm.ssub.with.overflow.i33(i33 %[[#X]], i33 %[[#Y]])
// LLVM:   %[[#RES:]] = extractvalue { i33, i1 } %[[#CALL]], 0
// LLVM:   %[[#OV:]] = extractvalue { i33, i1 } %[[#CALL]], 1
// LLVM:   %[[#TRUNC:]] = trunc i33 %[[#RES]] to i32
// LLVM:   %[[#TRUNC_EXT:]] = zext i32 %[[#TRUNC]] to i33
// LLVM:   %[[#TRUNC_OV:]] = icmp ne i33 %[[#RES]], %[[#TRUNC_EXT]]
// LLVM:   or i1 %[[#OV]], %[[#TRUNC_OV]]

// OGCG-LABEL: define{{.*}} i1 @{{.*}}test_sub_overflow_int_uint_uint{{.*}}
// OGCG:       %[[#X:]] = sext i32 %{{.*}} to i33
// OGCG:       %[[#Y:]] = zext i32 %{{.*}} to i33
// OGCG:       %[[#CALL:]] = call { i33, i1 } @llvm.ssub.with.overflow.i33(i33 %[[#X]], i33 %[[#Y]])
// OGCG-DAG:   %[[#OV:]] = extractvalue { i33, i1 } %[[#CALL]], 1
// OGCG-DAG:   %[[#RES:]] = extractvalue { i33, i1 } %[[#CALL]], 0
// OGCG:       %[[#TRUNC:]] = trunc i33 %[[#RES]] to i32
// OGCG:       %[[#TRUNC_EXT:]] = zext i32 %[[#TRUNC]] to i33
// OGCG:       %[[#TRUNC_OV:]] = icmp ne i33 %[[#RES]], %[[#TRUNC_EXT]]
// OGCG:       or i1 %[[#OV]], %[[#TRUNC_OV]]

bool test_mul_overflow_int_uint_uint(int x, unsigned y, unsigned *res) {
  return __builtin_mul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @{{.*}}test_mul_overflow_int_uint_uint{{.*}}
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u32i>>, !cir.ptr<!u32i>
// CIR-NEXT:   %[[#PROM_X:]] = cir.cast integral %[[#X]] : !s32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[#PROM_Y:]] = cir.cast integral %[[#Y]] : !u32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[RES:.+]], %[[OV:.+]] = cir.binop.overflow(mul, %[[#PROM_X]], %[[#PROM_Y]]) : !cir.int<s, 33>, (!cir.int<s, 33>, !cir.bool)
// CIR-NEXT:   %[[TRUNC:.+]] = cir.cast integral %[[RES]] : !cir.int<s, 33> -> !u32i
// CIR-NEXT:   %[[TRUNC_EXT:.+]] = cir.cast integral %[[TRUNC]] : !u32i -> !cir.int<s, 33>
// CIR-NEXT:   %[[TRUNC_OV:.+]] = cir.cmp(ne, %[[RES]], %[[TRUNC_EXT]]) : !cir.int<s, 33>, !cir.bool
// CIR-NEXT:   %[[OV2:.+]] = cir.binop(or, %[[OV]], %[[TRUNC_OV]]) : !cir.bool
// CIR-NEXT:   cir.store{{.*}} %[[TRUNC]], %[[#RES_PTR]] : !u32i, !cir.ptr<!u32i>
//      CIR: }

// LLVM: define{{.*}} i1 @{{.*}}test_mul_overflow_int_uint_uint{{.*}}
// LLVM:   %[[#X:]] = sext i32 %{{.*}} to i33
// LLVM:   %[[#Y:]] = zext i32 %{{.*}} to i33
// LLVM:   %[[#CALL:]] = call { i33, i1 } @llvm.smul.with.overflow.i33(i33 %[[#X]], i33 %[[#Y]])
// LLVM:   %[[#RES:]] = extractvalue { i33, i1 } %[[#CALL]], 0
// LLVM:   %[[#OV:]] = extractvalue { i33, i1 } %[[#CALL]], 1
// LLVM:   %[[#TRUNC:]] = trunc i33 %[[#RES]] to i32
// LLVM:   %[[#TRUNC_EXT:]] = zext i32 %[[#TRUNC]] to i33
// LLVM:   %[[#TRUNC_OV:]] = icmp ne i33 %[[#RES]], %[[#TRUNC_EXT]]
// LLVM:   or i1 %[[#OV]], %[[#TRUNC_OV]]

// OGCG-LABEL: define{{.*}} i1 @{{.*}}test_mul_overflow_int_uint_uint{{.*}}
// OGCG:       %[[#NEG:]] = icmp slt i32 %{{.*}}, 0
// OGCG:       %[[#NEG_VAL:]] = sub i32 0, %{{.*}}
// OGCG:       %[[#ABS:]] = select i1 %[[#NEG]], i32 %[[#NEG_VAL]], i32 %{{.*}}
// OGCG:       %[[#CALL:]] = call { i32, i1 } @llvm.umul.with.overflow.i32(i32 %[[#ABS]], i32 %{{.*}})
// OGCG:       %[[#OV:]] = extractvalue { i32, i1 } %[[#CALL]], 1
// OGCG:       %[[#RES:]] = extractvalue { i32, i1 } %[[#CALL]], 0
// OGCG:       %[[#NZ:]] = icmp ne i32 %[[#RES]], 0
// OGCG:       %[[#UNDER:]] = and i1 %[[#NEG]], %[[#NZ]]
// OGCG:       %[[#OV2:]] = or i1 %[[#OV]], %[[#UNDER]]
// OGCG:       %[[#NEG_RES:]] = sub i32 0, %[[#RES]]
// OGCG:       %[[#RES2:]] = select i1 %[[#NEG]], i32 %[[#NEG_RES]], i32 %[[#RES]]

bool test_add_overflow_volatile(int x, int y, volatile int *res) {
  return __builtin_add_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z26test_add_overflow_volatileiiPVi
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s32i>>, !cir.ptr<!s32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#X]], %[[#Y]]) : !s32i, (!s32i, !cir.bool)
// CIR-NEXT:   cir.store volatile{{.*}} %[[RES]], %[[#RES_PTR]] : !s32i, !cir.ptr<!s32i>
//      CIR: }

bool test_uadd_overflow(unsigned x, unsigned y, unsigned *res) {
  return __builtin_uadd_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z18test_uadd_overflowjjPj
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u32i>>, !cir.ptr<!u32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#X]], %[[#Y]]) : !u32i, (!u32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u32i, !cir.ptr<!u32i>
//      CIR: }

bool test_uaddl_overflow(unsigned long x, unsigned long y, unsigned long *res) {
  return __builtin_uaddl_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z19test_uaddl_overflowmmPm
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u64i>>, !cir.ptr<!u64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#X]], %[[#Y]]) : !u64i, (!u64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u64i, !cir.ptr<!u64i>
//      CIR: }

bool test_uaddll_overflow(unsigned long long x, unsigned long long y, unsigned long long *res) {
  return __builtin_uaddll_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z20test_uaddll_overflowyyPy
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u64i>>, !cir.ptr<!u64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#X]], %[[#Y]]) : !u64i, (!u64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u64i, !cir.ptr<!u64i>
//      CIR: }

bool test_usub_overflow(unsigned x, unsigned y, unsigned *res) {
  return __builtin_usub_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z18test_usub_overflowjjPj
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u32i>>, !cir.ptr<!u32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(sub, %[[#X]], %[[#Y]]) : !u32i, (!u32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u32i, !cir.ptr<!u32i>
//      CIR: }

bool test_usubl_overflow(unsigned long x, unsigned long y, unsigned long *res) {
  return __builtin_usubl_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z19test_usubl_overflowmmPm
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u64i>>, !cir.ptr<!u64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(sub, %[[#X]], %[[#Y]]) : !u64i, (!u64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u64i, !cir.ptr<!u64i>
//      CIR: }

bool test_usubll_overflow(unsigned long long x, unsigned long long y, unsigned long long *res) {
  return __builtin_usubll_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z20test_usubll_overflowyyPy
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u64i>>, !cir.ptr<!u64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(sub, %[[#X]], %[[#Y]]) : !u64i, (!u64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u64i, !cir.ptr<!u64i>
//      CIR: }

bool test_umul_overflow(unsigned x, unsigned y, unsigned *res) {
  return __builtin_umul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z18test_umul_overflowjjPj
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u32i>, !u32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u32i>>, !cir.ptr<!u32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#X]], %[[#Y]]) : !u32i, (!u32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u32i, !cir.ptr<!u32i>
//      CIR: }

bool test_umull_overflow(unsigned long x, unsigned long y, unsigned long *res) {
  return __builtin_umull_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z19test_umull_overflowmmPm
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u64i>>, !cir.ptr<!u64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#X]], %[[#Y]]) : !u64i, (!u64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u64i, !cir.ptr<!u64i>
//      CIR: }

bool test_umulll_overflow(unsigned long long x, unsigned long long y, unsigned long long *res) {
  return __builtin_umulll_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z20test_umulll_overflowyyPy
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!u64i>, !u64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!u64i>>, !cir.ptr<!u64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#X]], %[[#Y]]) : !u64i, (!u64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !u64i, !cir.ptr<!u64i>
//      CIR: }

bool test_sadd_overflow(int x, int y, int *res) {
  return __builtin_sadd_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z18test_sadd_overflowiiPi
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s32i>>, !cir.ptr<!s32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#X]], %[[#Y]]) : !s32i, (!s32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s32i, !cir.ptr<!s32i>
//      CIR: }

bool test_saddl_overflow(long x, long y, long *res) {
  return __builtin_saddl_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z19test_saddl_overflowllPl
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s64i>>, !cir.ptr<!s64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#X]], %[[#Y]]) : !s64i, (!s64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s64i, !cir.ptr<!s64i>
//      CIR: }

bool test_saddll_overflow(long long x, long long y, long long *res) {
  return __builtin_saddll_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z20test_saddll_overflowxxPx
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s64i>>, !cir.ptr<!s64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(add, %[[#X]], %[[#Y]]) : !s64i, (!s64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s64i, !cir.ptr<!s64i>
//      CIR: }

bool test_ssub_overflow(int x, int y, int *res) {
  return __builtin_ssub_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z18test_ssub_overflowiiPi
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s32i>>, !cir.ptr<!s32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(sub, %[[#X]], %[[#Y]]) : !s32i, (!s32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s32i, !cir.ptr<!s32i>
//      CIR: }

bool test_ssubl_overflow(long x, long y, long *res) {
  return __builtin_ssubl_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z19test_ssubl_overflowllPl
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s64i>>, !cir.ptr<!s64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(sub, %[[#X]], %[[#Y]]) : !s64i, (!s64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s64i, !cir.ptr<!s64i>
//      CIR: }

bool test_ssubll_overflow(long long x, long long y, long long *res) {
  return __builtin_ssubll_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z20test_ssubll_overflowxxPx
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s64i>>, !cir.ptr<!s64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(sub, %[[#X]], %[[#Y]]) : !s64i, (!s64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s64i, !cir.ptr<!s64i>
//      CIR: }

bool test_smul_overflow(int x, int y, int *res) {
  return __builtin_smul_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z18test_smul_overflowiiPi
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s32i>, !s32i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s32i>>, !cir.ptr<!s32i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#X]], %[[#Y]]) : !s32i, (!s32i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s32i, !cir.ptr<!s32i>
//      CIR: }

bool test_smull_overflow(long x, long y, long *res) {
  return __builtin_smull_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z19test_smull_overflowllPl
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s64i>>, !cir.ptr<!s64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#X]], %[[#Y]]) : !s64i, (!s64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s64i, !cir.ptr<!s64i>
//      CIR: }

bool test_smulll_overflow(long long x, long long y, long long *res) {
  return __builtin_smulll_overflow(x, y, res);
}

//      CIR: cir.func {{.*}} @_Z20test_smulll_overflowxxPx
//      CIR:   %[[#X:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#Y:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!s64i>, !s64i
// CIR-NEXT:   %[[#RES_PTR:]] = cir.load{{.*}} %{{.+}} : !cir.ptr<!cir.ptr<!s64i>>, !cir.ptr<!s64i>
// CIR-NEXT:   %[[RES:.+]], %{{.+}} = cir.binop.overflow(mul, %[[#X]], %[[#Y]]) : !s64i, (!s64i, !cir.bool)
// CIR-NEXT:   cir.store{{.*}} %[[RES]], %[[#RES_PTR]] : !s64i, !cir.ptr<!s64i>
//      CIR: }
