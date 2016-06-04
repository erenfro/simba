/**
 * @file main.c
 * @version 0.6.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

int test_text(struct harness_t *harness_p)
{
    char re[8];

    BTASSERT(re_compile(re, "foo", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "foo", 3, NULL, NULL) == 3);
    BTASSERT(re_match(re, "FoO", 3, NULL, NULL) == -1);

    BTASSERT(re_compile(re, "foo", RE_IGNORECASE, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "foo", 3, NULL, NULL) == 3);
    BTASSERT(re_match(re, "FoO", 3, NULL, NULL) == 3);
    BTASSERT(re_match(re, "BAR", 3, NULL, NULL) == -1);

    BTASSERT(re_compile(re, "\n", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "\n", 1, NULL, NULL) == 1);

    return (0);
}

int test_newline(struct harness_t *harness_p)
{
    char re[32];

    /* RE_DOTALL. */
    BTASSERT(re_compile(re, "a.b.c", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "a\nb\nc", 5, NULL, NULL) == -1);

    BTASSERT(re_compile(re, "a.b.c", RE_DOTALL, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "a\nb\nc", 5, NULL, NULL) == 5);

    /* RE_MULTILINE. */
    /* BTASSERT(re_compile(re, "^a\nb\nc$", 0, sizeof(re)) != NULL); */
    /* BTASSERT(re_match(re, "a\nb\nc", 5, NULL, NULL) == 5); */
    /* BTASSERT(re_match("^a\nb\nc$", "a\nb\nc", NULL, NULL, RE_MULTILINE) == 3); */
    /* BTASSERT(re_match("^a\nb$\n^c$", "a\nb\nc", NULL, NULL, RE_MULTILINE) == 3) */
    /* BTASSERT(re_match("^a\nb$\n^$c", "a\nb\nc", NULL, NULL, RE_MULTILINE) == -1) */

    return (0);
}

int test_special_escaped(struct harness_t *harness_p)
{
    char re[32];

    BTASSERT(re_compile(re, "\\s+", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, " \t\r\n\f\v", 6, NULL, NULL) == 6);
    BTASSERT(re_match(re, "non-space", 9, NULL, NULL) == -1);

    BTASSERT(re_compile(re, "\\d+", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "0123456789", 10, NULL, NULL) == 10);
    BTASSERT(re_match(re, "non-decimal-digits", 18, NULL, NULL) == -1);

    BTASSERT(re_compile(re, "\\w+", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "amzAMZ059_", 10, NULL, NULL) == 10);
    BTASSERT(re_match(re, "-.,\t", 4, NULL, NULL) == -1);

    return (0);
}

int test_special(struct harness_t *harness_p)
{
    char re[32];
    /* struct re_group_t groups[2]; */
    /* size_t number_of_groups; */

    BTASSERT(re_compile(re, ".", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "a", 1, NULL, NULL) == 1);

    /* BTASSERT(re_compile(re, "^$", 0, sizeof(re)) != NULL); */
    /* BTASSERT(re_match(re, "", 0, NULL, NULL) == 0); */

    BTASSERT(re_compile(re, "a?b", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "b", 1, NULL, NULL) == 1);
    BTASSERT(re_match(re, "c", 1, NULL, NULL) == -1);

    BTASSERT(re_compile(re, ".*", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "", 0, NULL, NULL) == 0);
    BTASSERT(re_match(re, "aaa", 3, NULL, NULL) == 3);

    BTASSERT(re_compile(re, "a*", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "aa", 2, NULL, NULL) == 2);

    BTASSERT(re_compile(re, "a+", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "aa", 2, NULL, NULL) == 2);

    BTASSERT(re_compile(re, ".+", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "", 0, NULL, NULL) == -1);

    BTASSERT(re_compile(re, "a*b+", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "aabb", 4, NULL, NULL) == 4);
    BTASSERT(re_match(re, "b", 1, NULL, NULL) == 1);

    BTASSERT(re_compile(re, ".*b+", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "aabb", 4, NULL, NULL) == 4);
    BTASSERT(re_match(re, "aabbcd", 6, NULL, NULL) == 4);

    BTASSERT(re_compile(re, "\\(escape\\)", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "(escape)", 8, NULL, NULL) == 8);
    BTASSERT(re_match(re, "(escape", 7, NULL, NULL) == -1);

    BTASSERT(re_compile(re, "\\(*\\)+", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "((()))", 6, NULL, NULL) == 6);
    BTASSERT(re_match(re, "()", 2, NULL, NULL) == 2);
    BTASSERT(re_match(re, ")", 1, NULL, NULL) == 1);
    BTASSERT(re_match(re, "(", 1, NULL, NULL) == -1);

    BTASSERT(re_compile(re, ".{2}", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "ab", 2, NULL, NULL) == 2);
    BTASSERT(re_match(re, "a", 1, NULL, NULL) == -1);

    BTASSERT(re_compile(re, "a{1}b{2}", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "abb", 3, NULL, NULL) == 3);
    BTASSERT(re_match(re, "abc", 3, NULL, NULL) == -1);

    /* BTASSERT(re_compile(re, "a|b", 0, sizeof(re)) != NULL); */
    /* BTASSERT(re_match(re, "b", 1, NULL, NULL) == 1); */

    /* BTASSERT(re_compile(re, "[ab]", 0, sizeof(re)) != NULL); */
    /* BTASSERT(re_match(re, "b", 1, NULL, NULL) == 1); */

    /* BTASSERT(re_compile(re, "(.)", 0, sizeof(re)) != NULL); */
    /* BTASSERT(re_match(re, "a", 1, groups, &number_of_groups) == 1); */

    /* BTASSERT(re_compile(re, "(.)", 0, sizeof(re)) != NULL); */

    /* number_of_groups = membersof(groups); */
    /* BTASSERT(re_match(re, "a", 1, groups, &number_of_groups) == 1); */

    /* BTASSERT(number_of_groups == 1); */
    /* BTASSERT(groups[0].buf_p != NULL); */
    /* BTASSERT(groups[0].size == 1); */
    /* BTASSERT(groups[0].buf_p[0] == 'a'); */

    return (0);
}

/* int test_special_character_set(struct harness_t *harness_p) */
/* { */
/*     BTASSERT(re_match("[ \t\r\n\f\v]+", " \t\r\n\f\v", NULL, NULL, 0) == 6); */
/*     BTASSERT(re_match("[0-9]+", "0123456789", NULL, NULL, 0) == 10); */
/*     BTASSERT(re_match("[a-zA-Z0-9_]+", "azAZ09_", NULL, NULL, 0) == 7); */

/*     return (0); */
/* } */

int test_greed(struct harness_t *harness_p)
{
    char re[32];

    /* Greedy, match as much as possible. */
    BTASSERT(re_compile(re, "<.\?>", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "<p>foo</p>", 10, NULL, NULL) == 3);

    /* Greedy, match as much as possible. */
    BTASSERT(re_compile(re, "<.*>", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "<p>foo</p>", 10, NULL, NULL) == 10);

    /* Greedy, match as much as possible, at least one. */
    BTASSERT(re_compile(re, "<.+>", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "<p>foo</p>", 10, NULL, NULL) == 10);

    /* Non-greedy, match as little as possible. */
    BTASSERT(re_compile(re, "<.\?\?>", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "<p>foo</p>", 10, NULL, NULL) == 3);
    BTASSERT(re_match(re, "<>>", 3, NULL, NULL) == 2);

    BTASSERT(re_compile(re, "<.\?\?>", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "<", 1, NULL, NULL) == -1);

    /* Non-greedy, match as little as possible. */
    BTASSERT(re_compile(re, "<.*?>", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "<p>foo</p>", 10, NULL, NULL) == 3);
    BTASSERT(re_match(re, "<>>", 3, NULL, NULL) == 2);

    /* Non-greedy, match as little as possible, at least one. */
    BTASSERT(re_compile(re, "<.+?>", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "<p>foo</p>", 10, NULL, NULL) == 3);

    return (0);
}

/* int test_groups(struct harness_t *harness_p) */
/* { */
/*     char re[32]; */
/*     struct re_group_t groups[2]; */
/*     size_t number_of_groups; */

/*     /\* Two groups. *\/ */
/*     BTASSERT(re_compile(re, "(\\d+)(\\w+)", 0, sizeof(re)) == 0); */

/*     number_of_groups = 2; */
/*     BTASSERT(re_match(re, "123abc", 6, groups, &number_of_groups) == 6); */
/*     BTASSERT(number_of_groups == 2); */
/*     BTASSERT(groups[0].size == 3); */
/*     BTASSERT(strncmp(groups[0].buf_p, "123", 3) == 0); */
/*     BTASSERT(groups[1].size == 3); */
/*     BTASSERT(strncmp(groups[1].buf_p, "abc", 3) == 0); */

/*     /\* Group repetition. *\/ */
/*     BTASSERT(re_match("(\\d)+", */
/*                       "123", */
/*                       groups, */
/*                       &number_of_groups, */
/*                       0) == 3); */
/*     BTASSERT(number_of_groups == 3); */
/*     BTASSERT(groups[0].buf_p != NULL); */
/*     BTASSERT(groups[0].size == 1); */
/*     BTASSERT(strncmp(groups[0].buf_p, "1", 1) == 0); */
/*     BTASSERT(groups[1].buf_p != NULL); */
/*     BTASSERT(groups[1].size == 1); */
/*     BTASSERT(strncmp(groups[1].buf_p, "2", 1) == 0); */
/*     BTASSERT(groups[2].buf_p != NULL); */
/*     BTASSERT(groups[2].size == 1); */
/*     BTASSERT(strncmp(groups[2].buf_p, "3", 1) == 0); */

/*     /\* Alternatives in a group. *\/ */
/*     BTASSERT(re_match("(\\d+|\\w+)+", */
/*                       "123abc", */
/*                       groups, */
/*                       &number_of_groups, */
/*                       0) == 6); */
/*     BTASSERT(number_of_groups == 2); */
/*     BTASSERT(groups[0].buf_p != NULL); */
/*     BTASSERT(groups[0].size == 3); */
/*     BTASSERT(strncmp(groups[0].buf_p, "123", 3) == 0); */
/*     BTASSERT(groups[1].buf_p != NULL); */
/*     BTASSERT(groups[1].size == 3); */
/*     BTASSERT(strncmp(groups[1].buf_p, "abc", 3) == 0); */

/*     return (0); */
/* } */

int test_custom(struct harness_t *harness_p)
{
    char re[64];

    BTASSERT(re_compile(re, "<.*<b{1}.??.?c>*", 0, sizeof(re)) != NULL);
    BTASSERT(re_match(re, "<a><b><c>>", 10, NULL, NULL) == 10);

    return (0);
}

int test_compile(struct harness_t *harness_p)
{
    char re[64];

    /* Compiled buffer too small. */

    BTASSERT(re_compile(re, ".", 0, 3) != NULL);
    BTASSERT(re_compile(re, ".", 0, 2) == NULL);

    BTASSERT(re_compile(re, ".?", 0, 6) != NULL);
    BTASSERT(re_compile(re, ".?", 0, 4) == NULL);

    BTASSERT(re_compile(re, ".{1}", 0, 9) != NULL);
    BTASSERT(re_compile(re, ".{1}", 0, 7) == NULL);

    BTASSERT(re_compile(re, "a", 0, 4) != NULL);
    BTASSERT(re_compile(re, "a", 0, 2) == NULL);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_text, "test_text" },
        { test_newline, "test_newline" },
        { test_special_escaped, "test_special_escaped" },
        { test_special, "test_special" },
        /* { test_special_character_set, "test_special_character_set" }, */
        { test_greed, "test_greed" },
        /* { test_groups, "test_groups" }, */
        { test_custom, "test_custom" },
        { test_compile, "test_compile" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
