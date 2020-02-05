#include "ush.h"

static void find_shell_functions(t_info *info, char **line, char *craft);
static void check_line(t_info *info, char **line, char **args, int *pos);
static void save_shell_func(t_info *info, char **line, char **args, int *pos);
static bool is_allow(char c, bool end);

void mx_shell_functions(t_info *info, char **line) {
    int pos[5] = {0, 0, 0, 0, 0};
    char **args = NULL;
    char *tmp = NULL;

    find_shell_functions(info, line, *line);
    if ((pos[0] = mx_get_char_index(*line, '(')) >= 0
        && (pos[1] = mx_get_char_index(*line, ')')) >= 0
        && (pos[2] = mx_get_char_index(*line, '{')) >= 0
        && (pos[3] = mx_get_char_index(*line, '}')) >= 0) {
        for (int i = 0; i < pos[0]; i++)
            if (!is_allow((*line)[i], 0))
                return;
        tmp = strndup(*line, pos[0]);
        args = mx_strsplit(tmp, ' ');
        mx_strdel(&tmp);
        // for (int i = 0; args[i]; i++)
            // printf("args[%d] = %s\n", i, args[i]);
        check_line(info, line, args, pos);
    }
    // for (int i = 0; i < 4; i++)
        // printf("pos[%d] = %d\n", i, pos[i]);
    // printf("============================\n");
    // for (t_export *tmp = info->shell_funcs; tmp; tmp = tmp->next)
        // printf("%s : %s\n", tmp->key, tmp->value);
    // printf("============================\n");
}

static void find_shell_functions(t_info *info, char **line, char *craft) {
    char *new = mx_strnew(mx_strlen(*line));
    char *check = NULL;
    int word_size = 0;
    t_export *find = NULL;

    for (int pos = 0; craft[pos]; pos++) {
        if (mx_isspace(craft[pos]) && is_allow(craft[pos], 1))
            strncat(new, &(craft[pos]), 1);
        else {
            while (!mx_isspace(craft[pos + word_size])
                && (is_allow(craft[pos + word_size], 1)
                || craft[pos + word_size] == '{'
                || craft[pos + word_size] == '}')) {
                word_size++;
            }
            check = strndup(&(craft[pos]), word_size);
            if ((find = mx_search_key_in_list(info->shell_funcs, check))) {
                new = realloc(new, malloc_size(new) + strlen(find->value));
                strcat(new, find->value);
            }
            else
                strncat(new, &(craft[pos]), word_size);
            word_size - 1 ? pos += word_size - 1 : 0;
            mx_strdel(&check);
            word_size = 0;
        }
    }
    free(*line);
    *line = mx_strdup(new);
    mx_strdel(&new);
}

static void check_line(t_info *info, char **line, char **args, int *pos) {
    if (pos[0] < pos[1] && pos[1] < pos[2] && pos[2] < pos[3]) {
        for (int i = pos[0] + 1; i < pos[1]; i++)
            if (!mx_isspace((*line)[i]))
                return;
        for (int i = pos[1] + 1; i < pos[2]; i++)
            if (!mx_isspace((*line)[i]))
                return;
        for (int i = pos[2] + 1; i < pos[3]; i++)
            if (!is_allow((*line)[i], 1))
                return;
        for (int i = pos[3] + 1; (*line)[i]; i++) {
            if ((*line)[i] == ';' && (pos[4] = i))
                break;
            if (!mx_isspace((*line)[i]))
                return;
        }
        save_shell_func(info, line, args, pos);
        if (pos[4])
            mx_shell_functions(info, line);
    }
}

static void save_shell_func(t_info *info
    , char **line, char **args, int *pos) {
    t_export *insert = NULL;
    char *value = mx_strndup(&((*line)[pos[2] + 1]), pos[3] - pos[2] - 1);

    for (int i = 0; args[i]; i++)
        if ((insert = mx_search_key_in_list(info->shell_funcs, args[i]))) {
            mx_strdel(&(insert->value));
            insert->value = strdup(value);
        }
        else
            mx_push_export_back(&(info->shell_funcs), args[i], value);
    mx_strdel(&value);
    mx_del_strarr(&args);
    if (pos[4])
        value = mx_strdup(&((*line)[pos[4] + 1]));
    else
        value = mx_strdup(&((*line)[pos[3] + 1]));
    free(*line);
    *line = mx_strdup(value);
    mx_strdel(&value);
}

static bool is_allow(char c, bool end) {
    if (!end) {
        if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95
            || (c >= 48 && c <= 57) || mx_isspace(c))
            return 1;
    }
    else {
        if (mx_isspace(c) || (c >= 33 && c <= 122) || c == 124 || c == 126)
            return 1;
    }
    return 0;
}
