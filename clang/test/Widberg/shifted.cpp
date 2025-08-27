// RUN: %clang_cc1 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK %s

typedef struct vec3f {
    float x;
    float y;
    float z;
} vec3f_t;

typedef struct player {
    char name[16];
    int health;
    int armor;
    int ammo;
    vec3f_t pos;
} player_t;

// CHECK-LABEL: define{{.*}} ptr @_Z40get_player_name_from_shifted_pos_pointerPK5vec3f(ptr noundef %pos)
const char *get_player_name_from_shifted_pos_pointer(const vec3f_t *__shifted(player_t, 0x1C) pos) {
    return ADJ(pos)->name;
}

// CHECK: %pos.addr = alloca ptr, align 4
// CHECK: store ptr %pos, ptr %pos.addr, align 4
// CHECK: %0 = load ptr, ptr %pos.addr, align 4
// CHECK: %add.ptr = getelementptr inbounds i8, ptr %0, i32 -28
// CHECK: %name = getelementptr inbounds nuw %struct.player, ptr %add.ptr, i32 0, i32 0
// CHECK: %arraydecay = getelementptr inbounds [16 x i8], ptr %name, i32 0, i32 0
// CHECK: ret ptr %arraydecay
