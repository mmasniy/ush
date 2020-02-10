#include "../../inc/ush.h"

void mx_free_lst(t_tok *lst) {
    lst->next ? lst->next->prev = lst->prev : 0;
    lst->prev ? lst->prev->next = lst->next : 0;
    lst->next = NULL;
    lst->prev = NULL;
    if (lst->token)
        mx_strdel(&lst->token);
    free(lst);
}

void check_file_or_in(t_tok **lst) {
    t_tok *tmp = *lst;;
    char *filename;

    if (!tmp)
        return ;
    while (tmp && tmp->next && tmp->next->next){
        if ((tmp->next->prio == 8 || tmp->next->prio == 4)
            && tmp->next->next){
            // tmp->next->type == 10 ?
            // filename = mx_strdup(mx_run_heredoc(tmp->next->next->content)) : 0;
            tmp->next->prio == 4 ?
            filename = mx_strdup(tmp->next->next->token) : 0;
            mx_free_lst(tmp->next->next);
            mx_strdel(&tmp->next->token);
            tmp->next->token = mx_strdup(filename);
            tmp->next->type = 0;
            tmp->next->prio = 10;
            mx_strdel(&filename);
        }
        tmp = tmp->next;
    }
}

void mx_check_tok(t_tok **tok, char *command, int size, int i) {
    if (tok || command || size || i) {}
        printf("ya na meste\n");
}

void mx_valid_red(t_tok **tok) {
    while ((*tok)) {
        mx_check_tok(tok, (*tok)->token, mx_strlen((*tok)->token), 0);
        (*tok) = (*tok)->next;
    }
}

int mx_work_w_toks(char *line, t_tok **tok) {
    int size = 1;
    int i = 0;
    char *tmp;

    if (!line)
        return 0;
    tmp = mx_strdup(line);
    while (size > 0 && *(tmp + i)) {
        if ((size = mx_get_size_tok(tmp + i)) == -1) {
            mx_strdel(&tmp);
            return 0;
        }
        if (*(tmp + i) != 32)
            mx_add_tok(tok, tmp + i, size);
        i += size;
    }
    // while (*tok && (*tok)->prev)
    //     *tok = (*tok)->prev;
    // // mx_valid_red(tok);
    // // while (*tok && (*tok)->prev)
    // //     *tok = (*tok)->prev;
    // // check_file_or_in(tok);
    // Вывод красивый, чтобы было понятнее
    printf("%slist: %s\n", GRN, RESET);
    printf("%s---------------------------------------------%s\n", MAG, RESET);
    for (t_tok *temp = *tok; temp; temp = temp->next) {
         printf("%s[%s%s%s%s%s]%s ",GRN , RESET, YEL, temp->token, RESET, GRN, RESET);
    }
    printf("\n\n");
    for (t_tok *temp = *tok; temp; temp = temp->next) {
         printf("%s[%s%s%d%s%s]%s ",GRN , RESET, YEL, temp->type, RESET, GRN, RESET);
    }
    printf("\n\n");
    for (t_tok *temp = *tok; temp; temp = temp->next) {
         printf("%s[%s%s%d%s%s]%s ",GRN , RESET, YEL, temp->prio, RESET, GRN, RESET);
    }
    printf("\n%s---------------------------------------------%s\n", MAG, RESET);
    printf("\n");
    mx_strdel(&tmp);
    return 1;
}
