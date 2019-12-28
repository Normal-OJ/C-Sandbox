#include <seccomp.h>
#include <fcntl.h>
#include <stdbool.h>

#define LOAD_SECCOMP_FAILED 1

int c_cpp_rules(char *target, bool allow_write_file){
    scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_KILL);
    if (!ctx) {
        return LOAD_SECCOMP_FAILED;
    }
    // for execve
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, SCMP_A0(SCMP_CMP_EQ , (scmp_datum_t)target));
    // uname  read_link sysinfo writev lseek clock_gettime
    int syscalls_whitelist[] = {SCMP_SYS(read), SCMP_SYS(fstat),
                                SCMP_SYS(mmap), SCMP_SYS(mprotect),
                                SCMP_SYS(munmap), SCMP_SYS(uname),
                                SCMP_SYS(arch_prctl), SCMP_SYS(brk),
                                SCMP_SYS(access), SCMP_SYS(exit_group),
                                SCMP_SYS(close), SCMP_SYS(readlink),
                                SCMP_SYS(sysinfo), SCMP_SYS(write),
                                SCMP_SYS(writev), SCMP_SYS(lseek),
                                SCMP_SYS(clock_gettime)};

    // add rules
    int syscalls_whitelist_length = sizeof(syscalls_whitelist) / sizeof(int);
    for (int i = 0; i < syscalls_whitelist_length; i++) {
        if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, syscalls_whitelist[i], 0) != 0) {
            return LOAD_SECCOMP_FAILED;
        }
    }

    // file write permission
    if (!allow_write_file) {
        // do not allow "w" and "rw"
        if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0)) != 0) {
            return LOAD_SECCOMP_FAILED;
        }
        if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 1, SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0)) != 0) {
            return LOAD_SECCOMP_FAILED;
        }
    } 
    else {
        // allow "w" and "rw"
        if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 0) != 0) {
            return LOAD_SECCOMP_FAILED;
        }
        if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(dup), 0) != 0) {
            return LOAD_SECCOMP_FAILED;
        }
        if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(dup2), 0) != 0) {
            return LOAD_SECCOMP_FAILED;
        }
        if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(dup3), 0) != 0) {
            return LOAD_SECCOMP_FAILED;
        }
    }

	seccomp_load(ctx);
    return 0;
}

int general(char *target){
    return 0;
}