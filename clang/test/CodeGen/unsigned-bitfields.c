// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -emit-llvm -fdump-record-layouts -o /dev/null %s 2>&1 | FileCheck %s --check-prefix=SIGNED
// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -emit-llvm -fdump-record-layouts -fno-signed-bitfields -o /dev/null %s 2>&1 | FileCheck %s --check-prefix=UNSIGNED

typedef int plain_int_t;
typedef signed int signed_int_t;
typedef unsigned int unsigned_int_t;
typedef char plain_char_t;
typedef short plain_short_t;
typedef long plain_long_t;
typedef long long plain_long_long_t;
typedef __int128 plain_int128_t;

enum SignedEnum {
  SE_Negative = -1,
  SE_Zero = 0,
};

struct PlainInt {
  int value : 3;
} GPlainInt;
// SIGNED: Record: RecordDecl{{.*}}PlainInt
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}PlainInt
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct TypedefInt {
  plain_int_t value : 3;
} GTypedefInt;
// SIGNED: Record: RecordDecl{{.*}}TypedefInt
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}TypedefInt
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct ExplicitSignedInt {
  signed int value : 3;
} GExplicitSignedInt;
// SIGNED: Record: RecordDecl{{.*}}ExplicitSignedInt
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}ExplicitSignedInt
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0

struct ExplicitSignedTypedefInt {
  signed_int_t value : 3;
} GExplicitSignedTypedefInt;
// SIGNED: Record: RecordDecl{{.*}}ExplicitSignedTypedefInt
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}ExplicitSignedTypedefInt
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0

struct ExplicitUnsignedInt {
  unsigned int value : 3;
} GExplicitUnsignedInt;
// SIGNED: Record: RecordDecl{{.*}}ExplicitUnsignedInt
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}ExplicitUnsignedInt
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct ExplicitUnsignedTypedefInt {
  unsigned_int_t value : 3;
} GExplicitUnsignedTypedefInt;
// SIGNED: Record: RecordDecl{{.*}}ExplicitUnsignedTypedefInt
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}ExplicitUnsignedTypedefInt
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct PlainChar {
  char value : 3;
} GPlainChar;
// SIGNED: Record: RecordDecl{{.*}}PlainChar
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}PlainChar
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct TypedefChar {
  plain_char_t value : 3;
} GTypedefChar;
// SIGNED: Record: RecordDecl{{.*}}TypedefChar
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}TypedefChar
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct PlainShort {
  short value : 3;
} GPlainShort;
// SIGNED: Record: RecordDecl{{.*}}PlainShort
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}PlainShort
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct TypedefShort {
  plain_short_t value : 3;
} GTypedefShort;
// SIGNED: Record: RecordDecl{{.*}}TypedefShort
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}TypedefShort
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct PlainLong {
  long value : 3;
} GPlainLong;
// SIGNED: Record: RecordDecl{{.*}}PlainLong
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}PlainLong
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct TypedefLong {
  plain_long_t value : 3;
} GTypedefLong;
// SIGNED: Record: RecordDecl{{.*}}TypedefLong
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}TypedefLong
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct PlainLongLong {
  long long value : 3;
} GPlainLongLong;
// SIGNED: Record: RecordDecl{{.*}}PlainLongLong
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}PlainLongLong
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct TypedefLongLong {
  plain_long_long_t value : 3;
} GTypedefLongLong;
// SIGNED: Record: RecordDecl{{.*}}TypedefLongLong
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}TypedefLongLong
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct PlainInt128 {
  __int128 value : 3;
} GPlainInt128;
// SIGNED: Record: RecordDecl{{.*}}PlainInt128
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}PlainInt128
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct TypedefInt128 {
  plain_int128_t value : 3;
} GTypedefInt128;
// SIGNED: Record: RecordDecl{{.*}}TypedefInt128
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}TypedefInt128
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:0 StorageSize:{{[0-9]+}} StorageOffset:0

struct EnumFieldSigned {
  enum SignedEnum value : 3;
} GEnumFieldSigned;
// SIGNED: Record: RecordDecl{{.*}}EnumFieldSigned
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}EnumFieldSigned
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0

struct BitIntField {
  _BitInt(17) value : 3;
} GBitIntField;
// SIGNED: Record: RecordDecl{{.*}}BitIntField
// SIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
// UNSIGNED: Record: RecordDecl{{.*}}BitIntField
// UNSIGNED: <CGBitFieldInfo Offset:0 Size:3 IsSigned:1 StorageSize:{{[0-9]+}} StorageOffset:0
