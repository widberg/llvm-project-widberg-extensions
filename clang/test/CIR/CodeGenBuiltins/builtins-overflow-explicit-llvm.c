// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -fclangir -emit-llvm %s -o - | FileCheck %s --check-prefixes=LLVM,LLVM64
// RUN: %clang_cc1 -triple i686-unknown-unknown -fclangir -emit-llvm %s -o - | FileCheck %s --check-prefixes=LLVM,LLVM32
// RUN: %clang_cc1 -triple x86_64-mingw32 -fclangir -emit-llvm %s -o - | FileCheck %s --check-prefixes=LLVM,LLVM32

unsigned test_uadd_overflow(unsigned x, unsigned y) {
  unsigned r;
  __builtin_uadd_overflow(x, y, &r);
  return r;
}

// LLVM-LABEL: @test_uadd_overflow
// LLVM: call { i32, i1 } @llvm.uadd.with.overflow.i32

unsigned test_usub_overflow(unsigned x, unsigned y) {
  unsigned r;
  __builtin_usub_overflow(x, y, &r);
  return r;
}

// LLVM-LABEL: @test_usub_overflow
// LLVM: call { i32, i1 } @llvm.usub.with.overflow.i32

unsigned test_umul_overflow(unsigned x, unsigned y) {
  unsigned r;
  __builtin_umul_overflow(x, y, &r);
  return r;
}

// LLVM-LABEL: @test_umul_overflow
// LLVM: call { i32, i1 } @llvm.umul.with.overflow.i32

int test_sadd_overflow(int x, int y) {
  int r;
  __builtin_sadd_overflow(x, y, &r);
  return r;
}

// LLVM-LABEL: @test_sadd_overflow
// LLVM: call { i32, i1 } @llvm.sadd.with.overflow.i32

int test_ssub_overflow(int x, int y) {
  int r;
  __builtin_ssub_overflow(x, y, &r);
  return r;
}

// LLVM-LABEL: @test_ssub_overflow
// LLVM: call { i32, i1 } @llvm.ssub.with.overflow.i32

int test_smul_overflow(int x, int y) {
  int r;
  __builtin_smul_overflow(x, y, &r);
  return r;
}

// LLVM-LABEL: @test_smul_overflow
// LLVM: call { i32, i1 } @llvm.smul.with.overflow.i32

unsigned long test_uaddl_overflow(unsigned long x, unsigned long y) {
  unsigned long r;
  __builtin_uaddl_overflow(x, y, &r);
  return r;
}

// LLVM-LABEL: @test_uaddl_overflow
// LLVM64: call { i64, i1 } @llvm.uadd.with.overflow.i64
// LLVM32: call { i32, i1 } @llvm.uadd.with.overflow.i32

long test_saddl_overflow(long x, long y) {
  long r;
  __builtin_saddl_overflow(x, y, &r);
  return r;
}

// LLVM-LABEL: @test_saddl_overflow
// LLVM64: call { i64, i1 } @llvm.sadd.with.overflow.i64
// LLVM32: call { i32, i1 } @llvm.sadd.with.overflow.i32
