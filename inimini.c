#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "inimini.h"


void ini_parse_file(char *file, inientry **head) {
    FILE *fp = NULL;
    char buf[BUFFER_SIZE];

    // open file, check is it opened
    fp = fopen(file, "r");
    if (fp == NULL) {
        error("can't open INI file");
    }

    // read line by line
    inientry *_head = NULL;
    *head = NULL;
    while (fgets(buf, BUFFER_SIZE, fp)) {
        // remove all spaces
        remove_blank(buf);

        // skip if it is empty
        if (strlen(buf) < 3) {
            continue;
        }
        // skip if it is comment
        if (strncmp(buf, ";;", 2) == 0) {
            continue;
        }

        // allocate memory for inientry
        inientry *em = (inientry *) malloc(sizeof(inientry));
        em->key = NULL;
        em->value = NULL;
        em->next = NULL;

        _ini_parse_line(buf, em);

        if (*head == NULL) {
            *head = em;
            _head = em;
        } else {
            (*head)->next = em;
            *head = (*head)->next;
        }
    }

    // we are done with the file
    fclose(fp);

    // rollback to the head
    *head = _head;
}

void _ini_parse_line(char *line, inientry *e) {
    size_t len = strlen(line);

    e->key = (char *) malloc(sizeof(char) * INIMINI_KEY_LENGTH);
    e->value = (char *) malloc(sizeof(char) * INIMINI_VAL_LENGTH);

    bzero(e->key, INIMINI_KEY_LENGTH);
    bzero(e->value, INIMINI_VAL_LENGTH);

    e->key[0] = e->value[0] = '\0';
    while (*line != '=' && *line != '\0') {
        if (*line == ' ')
            continue;
        if (strncmp(line, INIMINI_COMMENT_PREFIX, 2) == 0) {
            break;
        }

        e->key[strlen(e->key)] = *line;

        line++;
        len--;
    }
    e->key[strlen(e->key)] = '\0';

    line++;
    while (*line != '\n' && *line != '\0') {
        if (*line == ' ')
            continue;
        if (strncmp(line, INIMINI_COMMENT_PREFIX, 2) == 0) {
            break;
        }

        e->value[strlen(e->value)] = *line;

        line++;
        len--;
    }
    e->value[strlen(e->value)] = '\0';
}

void ini_destroy(inientry *head) {
    if (head->next != NULL)
        ini_destroy(head->next);

    head->next = NULL;
    free(head->key);
    free(head->value);
    free(head);
}

void ini_get(inientry *head, char *key, char **value) {
    while (head != NULL) {
        if (strcmp(head->key, key) == 0) {
            if (*value == NULL) {
                size_t len = strlen(head->value) + 1;

                *value = (char *) malloc(sizeof(char) * len);
                bzero(*value, len);
            }

            strcpy(*value, head->value);
            return;
        }

        head = head->next;
    }

    *value = NULL;
}

void remove_blank(char *str) {
    char *i = str;
    char *j = str;
    while (*j != 0) {
        *i = *j++;
        if (*i != ' ')
            i++;
    }
    *i = 0;
}

void error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(errno);
}