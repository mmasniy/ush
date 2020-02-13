#include "../inc/ush.h"

char *mx_multi_line_enter(t_info *info, char *key_word) {
    char *line = NULL;
    FILE *f = fopen(".\t\t\r\r\n\n", "w+");

    mx_del_and_set(&(info->name), key_word);
    while (strcmp((line = mx_ush_read_line(info)), key_word)) {
        fprintf(f, "%s\n", line);
        mx_strdel(&line);
    }
    mx_del_and_set(&(info->name), USH);
    fflush(f);
    fclose(f);
    line = mx_file_to_str(".\t\t\r\r\n\n");
    remove(".\t\t\r\r\n\n");
    return line;
}