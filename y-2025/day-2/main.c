#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

// EXPERIMENT:
// - **NO** DYNAMIC MEMORY ALLOCATIONS

#define MAX_FILE_SIZE 1024
#define MAX_NUM_PAIRS 100
#define MAX_NUM_WIDTH 23 // align reads for len as well

typedef struct NumberRepr
{
    char repr[MAX_NUM_WIDTH];
    uint8_t len;
} NumberRepr;

NumberRepr make_num_repr(const char *str)
{
    NumberRepr ret_val = {0};
    size_t i;
    for (i = 0; i < MAX_NUM_WIDTH && str[i] != 0; i++)
    {
        ret_val.len++;
        ret_val.repr[i] = str[i];
    }
    assert(str[i] == 0);
    return ret_val;
}

NumberRepr num_repr_add(const NumberRepr a, const NumberRepr b)
{
    NumberRepr ret_rev = {0};
    size_t add_rel_idx = 0;
    uint8_t carry = 0;

    for (add_rel_idx = 0; add_rel_idx < a.len && add_rel_idx < b.len; add_rel_idx++)
    {
        const size_t a_idx = a.len - add_rel_idx - 1;
        const size_t b_idx = b.len - add_rel_idx - 1;
        const uint8_t newVal = (a.repr[a_idx] - '0') + (b.repr[b_idx] - '0') + carry % 10;
        carry = carry / 10 + newVal / 10;
        ret_rev.repr[add_rel_idx] = '0' + (newVal % 10);
        ret_rev.len++;
    }
    assert(add_rel_idx == a.len || add_rel_idx == b.len);

    const NumberRepr rest = add_rel_idx == a.len ? b : a;
    while (add_rel_idx < rest.len)
    {
        const size_t rest_idx = rest.len - add_rel_idx - 1;
        const uint8_t newVal = (rest.repr[rest_idx] - '0') + carry % 10;
        carry = carry / 10 + newVal / 10;
        ret_rev.repr[add_rel_idx] = '0' + (newVal % 10);
        ret_rev.len++;
        add_rel_idx++;
    }
    assert(add_rel_idx == rest.len);

    while (carry > 0)
    {
        ret_rev.repr[add_rel_idx] = '0' + carry % 10;
        carry = carry / 10;
        ret_rev.len++;
        add_rel_idx++;
    }

    NumberRepr ret_val = {0};
    ret_val.len = ret_rev.len;
    for (size_t i = 0; i < ret_rev.len; i++)
    {
        ret_val.repr[i] = ret_rev.repr[ret_rev.len - 1 - i];
    }
    return ret_val;
}

// return a-b
int num_rep_cmp(const NumberRepr a, const NumberRepr b)
{
    if (a.len < b.len)
    {
        return -1;
    }
    else if (a.len > b.len)
    {
        return 1;
    }

    assert(a.len == b.len);
    for (size_t i = 0; i < a.len; i++)
    {
        if (a.repr[i] < b.repr[i])
        {
            return -1;
        }
        else if (a.repr[i] > b.repr[i])
        {
            return 1;
        }
    }

    return 0;
}

typedef struct NumberPair
{
    NumberRepr lo;
    NumberRepr hi;
} NumberPair;

void read_file(const char *file_name, NumberPair *out_buf, size_t *out_buf_len)
{
    assert(out_buf != NULL);
    assert(out_buf_len != NULL);

    FILE *file = fopen(file_name, "r");
    assert(file != NULL);

    char file_buf[MAX_FILE_SIZE] = {0};
    const size_t read = fread(file_buf, sizeof(char), MAX_FILE_SIZE, file);
    assert(read <= MAX_FILE_SIZE && feof(file));
    assert(0 == fclose(file));

    out_buf[0] = (NumberPair){0};
    *out_buf_len = 0;
    enum
    {
        nt_LOW,
        nt_HIGH
    } number_type = nt_LOW;

    for (size_t i = 0; i < read; i++)
    {
        const char ch = file_buf[i];
        NumberPair *pair = &out_buf[*out_buf_len];

        if ('0' <= ch && ch <= '9')
        {
            NumberRepr *num_repr = NULL;
            if (number_type == nt_LOW)
            {
                num_repr = &pair->lo;
            }
            else if (number_type == nt_HIGH)
            {
                num_repr = &pair->hi;
            }
            assert(num_repr != NULL);
            assert(num_repr->len < MAX_NUM_WIDTH);
            num_repr->repr[num_repr->len] = ch;
            num_repr->len++;
        }
        else if (ch == '-')
        {
            number_type = number_type == nt_LOW ? nt_HIGH : nt_LOW;
        }
        else if (ch == ',')
        {
            number_type = nt_LOW;
            (*out_buf_len)++;
            out_buf[*out_buf_len] = (NumberPair){0};
            assert((*out_buf_len) < MAX_NUM_PAIRS);
        }
        else
        {
            assert(false && "Unknown char");
        }
    }
    (*out_buf_len)++;
}

NumberRepr get_prefix(const NumberRepr a, const size_t prefix_size)
{
    assert(prefix_size <= MAX_NUM_WIDTH);
    assert(a.len >= prefix_size);

    NumberRepr ret_val = {0};
    ret_val.len = prefix_size;
    for (size_t i = 0; i < prefix_size; i++)
    {
        ret_val.repr[i] = a.repr[i];
    }
    return ret_val;
}

NumberRepr repeat_prefix(const NumberRepr prefix, const size_t times)
{
    assert((prefix.len * times) <= MAX_NUM_WIDTH);
    NumberRepr ret_val = {0};
    ret_val.len = prefix.len * times;

    for (size_t i = 0; i < times; i++)
    {
        for (size_t p_idx = 0; p_idx < prefix.len; p_idx++)
        {
            ret_val.repr[i * prefix.len + p_idx] = prefix.repr[p_idx];
        }
    }

    return ret_val;
}

NumberRepr get_min_of_len(const size_t len)
{
    NumberRepr ret_val = repeat_prefix(make_num_repr("0"), len);
    ret_val.repr[0] = '1';
    return ret_val;
}

bool is_repeated_seq_ex(const NumberRepr n, const size_t parts_num)
{
    if (n.len % parts_num != 0)
    {
        return false;
    }

    const size_t seq_size = n.len / parts_num;
    const NumberRepr prefix = get_prefix(n, seq_size);
    const NumberRepr rep_seq = repeat_prefix(prefix, parts_num);

    return (num_rep_cmp(n, rep_seq) == 0);
}

bool is_repeated_seq(const NumberRepr n)
{
    for (size_t parts = n.len; parts > 1; parts--)
    {
        if (is_repeated_seq_ex(n, parts))
        {
            return true;
        }
    }

    return false;
}

NumberRepr get_next_rseq_num_ex(const NumberRepr n, const size_t parts_num)
{
    const size_t seq_size = n.len / parts_num;
    if (n.len % parts_num != 0)
    {
        const NumberRepr prefix = get_min_of_len(seq_size + 1);
        return repeat_prefix(prefix, parts_num);
    }

    const NumberRepr prefix = get_prefix(n, seq_size);
    const NumberRepr try1 = repeat_prefix(prefix, parts_num);

    if (num_rep_cmp(n, try1) < 0)
    {
        return try1;
    }

    const NumberRepr inc_prefix = num_repr_add(prefix, make_num_repr("1"));
    const NumberRepr try2 = repeat_prefix(inc_prefix, parts_num);
    assert(num_rep_cmp(n, try2) < 0);

    return try2;
}

NumberRepr get_next_rseq_num(const NumberRepr n)
{
    if (n.len == 1)
    {
        // single digit -> 11 is the next repeated sequence number
        return make_num_repr("11");
    }

    NumberRepr ret_val = {0};
    for (size_t parts = n.len; parts > 1; parts--)
    {
        const NumberRepr next = get_next_rseq_num_ex(n, parts);
        if (num_rep_cmp(n, next) < 0)
        {
            if (ret_val.len == 0)
            {
                ret_val = next;
            }

            if (num_rep_cmp(ret_val, next) > 0)
            {
                ret_val = next;
            }
        }
    }

    // check for parts in between
    for (size_t lower_len = n.len + 1; lower_len <= ret_val.len; lower_len++)
    {
        const NumberRepr min = get_min_of_len(lower_len);
        for (size_t parts = min.len; parts > 1; parts--)
        {
            const NumberRepr next = get_next_rseq_num_ex(min, parts);
            if (num_rep_cmp(n, next) < 0 &&
                num_rep_cmp(ret_val, next) > 0)
            {
                ret_val = next;
            }
        }
    }

    return ret_val;
}

void p1(const NumberPair *pairs, const size_t pairs_len)
{
    NumberRepr sol_rep = make_num_repr("0");
    for (size_t pair_idx = 0; pair_idx < pairs_len; pair_idx++)
    {
        if (is_repeated_seq_ex(pairs[pair_idx].lo, 2))
        {
            sol_rep = num_repr_add(sol_rep, pairs[pair_idx].lo);
        }

        NumberRepr curr = get_next_rseq_num_ex(pairs[pair_idx].lo, 2);
        while (num_rep_cmp(curr, pairs[pair_idx].hi) <= 0)
        {
            sol_rep = num_repr_add(sol_rep, curr);
            curr = get_next_rseq_num_ex(curr, 2);
        }
    }

    printf("P1: %s\n", sol_rep.repr);
}

void p2(const NumberPair *pairs, const size_t pairs_len)
{
    NumberRepr sol_rep = make_num_repr("0");
    for (size_t pair_idx = 0; pair_idx < pairs_len; pair_idx++)
    {
        if (is_repeated_seq(pairs[pair_idx].lo))
        {
            sol_rep = num_repr_add(sol_rep, pairs[pair_idx].lo);
        }

        NumberRepr curr = get_next_rseq_num(pairs[pair_idx].lo);
        while (num_rep_cmp(curr, pairs[pair_idx].hi) <= 0)
        {
            sol_rep = num_repr_add(sol_rep, curr);
            curr = get_next_rseq_num(curr);
        }
    }

    printf("P2: %s\n", sol_rep.repr);
}

// run with "gcc main.c -o main.exe -std=c99 -Wall -Wpedantic && ./main.exe"
int main(int argc, char *argv[])
{
    NumberPair pairs[MAX_NUM_PAIRS];
    size_t pairs_len;

    // read_file("test.txt", pairs, &pairs_len);
    read_file("input.txt", pairs, &pairs_len);
    p1(pairs, pairs_len);
    p2(pairs, pairs_len);

    return 0;
}