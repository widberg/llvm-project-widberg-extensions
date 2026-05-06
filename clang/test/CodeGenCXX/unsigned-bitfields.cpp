// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -std=c++20 -emit-llvm -fdump-record-layouts -o /dev/null %s 2>&1 | FileCheck %s
// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -std=c++20 -emit-llvm -fdump-record-layouts -fno-signed-bitfields -o /dev/null %s 2>&1 | FileCheck %s

typedef int plain_int_t;

enum SignedEnum {
  SE_Negative = -1,
  SE_Zero = 0,
};

struct PlainInt {
  int value : 3;
} GPlainInt;
// CHECK: Record: CXXRecordDecl{{.*}}PlainInt
// CHECK: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0

struct TypedefInt {
  plain_int_t value : 3;
} GTypedefInt;
// CHECK: Record: CXXRecordDecl{{.*}}TypedefInt
// CHECK: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0

struct ExplicitUnsignedInt {
  unsigned int value : 3;
} GExplicitUnsignedInt;
// CHECK: Record: CXXRecordDecl{{.*}}ExplicitUnsignedInt
// CHECK: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct EnumFieldSigned {
  enum SignedEnum value : 3;
} GEnumFieldSigned;
// CHECK: Record: CXXRecordDecl{{.*}}EnumFieldSigned
// CHECK: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
