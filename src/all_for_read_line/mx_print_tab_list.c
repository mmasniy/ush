#include "../../inc/ush.h"

static void basic_tab_print(t_info *info, int arg_len, int max_len) {
    int max_tabs = max_len / 8 + 1;
    int tab_len = arg_len / 8;

    if (tab_len < max_tabs) {
        write(1, "\t", 1);
        tab_len++;
    }
    while (max_tabs > tab_len) {
        write(1, "\t", 1);
        tab_len++;
    }
}

static int mx_num_of_cols(t_info *info, int *max_len) {
    int cols = 0;
    int lines = 0;

    mx_winsize(info);
    for (t_history *tmp = info->tab_list; tmp; tmp = tmp->next)
        if (*max_len < mx_strlen(tmp->data))
            *max_len = mx_strlen(tmp->data);
    cols = (info->winsize / ((8 - (*max_len % 8)) + *max_len));
    lines = info->num_of_elems_in_tab / cols;
    if (lines == 0 || ((info->num_of_elems_in_tab % cols) != 0))
        lines++;
    return lines;
}

static bool mx_count_num_of_elems_in_tab(t_info *info) {
    if (info->tab_list && info->tab_list->next) {
        int num = 0;

        for (t_history *tmp = info->tab_list; tmp; tmp = tmp->next) {
            num++;
        }
        info->num_of_elems_in_tab = num;
        return 1;
    }
    else
        return 0;
}

void mx_print_tab_list(t_info *info) {
    if (mx_count_num_of_elems_in_tab(info)) {
        int j = 0;
        int sub_r;
        int max_len = 0;
        int num_of_lines = mx_num_of_cols(info, &max_len);

        write(1, "\n\r", 2);
        for (int i = 0; i < num_of_lines; i++, j = 0) {
            sub_r = 0;
            for (t_history *tmp = info->tab_list; tmp; tmp = tmp->next, j++) {
                if ((j + num_of_lines - i) % num_of_lines == 0) {
                    mx_printstr(tmp->data);
                    if (sub_r + num_of_lines < info->num_of_elems_in_tab)
                        basic_tab_print(info, mx_strlen(tmp->data), max_len);
                }
                ++sub_r;
            }
            write(1, "\n\r", 2);
        }
    }
}
