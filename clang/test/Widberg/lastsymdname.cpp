// RUN: %clang_cc1 -std=c++20 -triple i386-pc-win32 -fms-compatibility -fms-extensions -fsyntax-only -verify %s
// expected-no-diagnostics

static_assert(__LASTSYMDNAME__[0] == '\0', "");

constexpr bool str_eq(const char *A, const char *B) {
  for (; *A || *B; ++A, ++B) {
    if (*A != *B)
      return false;
  }
  return true;
}

constexpr bool str_contains(const char *Haystack, const char *Needle) {
  if (!*Needle)
    return true;
  for (; *Haystack; ++Haystack) {
    const char *H = Haystack;
    const char *N = Needle;
    while (*H && *N && *H == *N) {
      ++H;
      ++N;
    }
    if (!*N)
      return true;
  }
  return false;
}

int global_symbol;
constexpr const char *AfterGlobal = __LASTSYMDNAME__;
static_assert(str_contains(AfterGlobal, "global_symbol"), "");

extern "C" int extern_c_global;
constexpr const char *AfterExternCGlobal = __LASTSYMDNAME__;
static_assert(str_eq(AfterExternCGlobal, "extern_c_global"), "");

extern "C" int __stdcall extern_c_stdcall(int, int);
constexpr const char *AfterExternCStdcall = __LASTSYMDNAME__;
static_assert(str_eq(AfterExternCStdcall, "_extern_c_stdcall@8"), "");

int free_symbol(int);
constexpr const char *AfterFree = __LASTSYMDNAME__;
static_assert(str_contains(AfterFree, "free_symbol"), "");

struct Ty {
  int member_symbol(int);
};
constexpr const char *AfterMember = __LASTSYMDNAME__;
static_assert(str_contains(AfterMember, "member_symbol"), "");

int verify_current_function(int x) {
  static_assert(str_eq(__LASTSYMDNAME__, __FUNCDNAME__), "");
  return x;
}
