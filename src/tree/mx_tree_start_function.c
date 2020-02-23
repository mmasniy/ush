#include "../../inc/ush.h"

int mx_start_function(t_ast *t, t_info *i, char **tree) {
    if (i->file_not_f != 1) {
        i->args = tree;
        if (i->file == 1) {
            mx_exec_for_file(t, i);
        }
        else {
            if (mx_check_buildin(i, 1) == -1){
                // mx_add_and_check_alias(t, i, 0);
                // mx_print_lias_alias(i->alias);
                mx_execute_binary_file(t, i);
            }
        }
    }
    return 0;
}

static void execute_binary_file(t_ast *t, t_info *i, pid_t pid) {
    char *path;

    if (mx_redirection(t->type)) {
        i->fd_r = mx_create_file(t, i);
        mx_run_redirection(t, i, pid);
    }
    else {
        path = mx_find_in_PATH(i->paths, t->command[0], 1);
        if (execv(path, t->command) == -1){
            mx_print_error(MX_ER, t->command[0]);
        }
        dup_2(i, 1);
        exit(EXIT_FAILURE);
    }
}

void mx_execute_binary_file(t_ast *t, t_info *i) {
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        dup_2(i, 0);
        execute_binary_file(t, i, pid);
    }
    else if (pid < 0) {
        mx_print_error(MX_ER, t->command[0]);
    }
    else {
        int status;
        pid_t wpid = waitpid(pid, &status, WUNTRACED); 

        while (!WIFEXITED(status)
            && !WIFSIGNALED(status))
            wpid = waitpid(pid, &status, WUNTRACED);
    }
}
