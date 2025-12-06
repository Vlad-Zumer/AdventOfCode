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
#define MAX_NUM_WIDTH 16

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

uint64_t repr2int(const NumberRepr num)
{
    uint64_t ret_val = 0;
    for (size_t i = 0; i < num.len; i++)
    {
        ret_val = ret_val * 10 + (num.repr[i] - '0');
    }
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

bool is_doubled_seq(const NumberRepr n)
{
    if (n.len % 2 != 0)
    {
        return false;
    }

    const size_t mid = n.len / 2;
    for (size_t i = 0; i < mid; i++)
    {
        if (n.repr[i] != n.repr[mid + i])
        {
            return false;
        }
    }

    return true;
}

NumberRepr get_next_dseq_num(const NumberRepr prev)
{
    NumberRepr ret_val = {0};
    if (prev.len % 2 == 1)
    {
        // odd number of digits
        ret_val.len = prev.len + 1;
        const size_t mid = ret_val.len / 2;
        ret_val.repr[0] = '1';
        ret_val.repr[mid] = '1';

        for (size_t i = 1; i < mid; i++)
        {
            ret_val.repr[i] = '0';
            ret_val.repr[i + mid] = '0';
        }

        return ret_val;
    }

    // make a doubled sequence number from the previous' first half
    ret_val.len = prev.len;
    const size_t mid = ret_val.len / 2;
    for (size_t i = 0; i < mid; i++)
    {
        ret_val.repr[i] = prev.repr[i];
        ret_val.repr[i + mid] = prev.repr[i];
    }

    // new doubled sequence number is larger than prev => is next
    if (num_rep_cmp(ret_val, prev) > 0)
    {
        return ret_val;
    }

    // find the first digit to increase
    size_t inc_index;
    for (inc_index = mid - 1; inc_index >= 0; inc_index--)
    {
        if (ret_val.repr[inc_index] < '9')
        {
            break;
        }
    }

    // need to check '<= mid' because unsigned-ness
    if (inc_index <= mid && inc_index >= 0)
    {
        ret_val.repr[inc_index]++;
        ret_val.repr[inc_index + mid]++;
        for (size_t i = inc_index + 1; i < mid; i++)
        {
            ret_val.repr[i] = '0';
            ret_val.repr[i + mid] = '0';
        }
        return ret_val;
    }

    // prev number is all 9s
    ret_val.len += 2;
    const size_t new_mid = ret_val.len / 2;
    ret_val.repr[0] = '1';
    ret_val.repr[new_mid] = '1';

    for (size_t i = 1; i < new_mid; i++)
    {
        ret_val.repr[i] = '0';
        ret_val.repr[i + new_mid] = '0';
    }

    return ret_val;
}

void p1(const NumberPair *pairs, const size_t pairs_len)
{
    NumberRepr sol_rep = make_num_repr("0");
    for (size_t pair_idx = 0; pair_idx < pairs_len; pair_idx++)
    {
        if (is_doubled_seq(pairs[pair_idx].lo))
        {
            sol_rep = num_repr_add(sol_rep, pairs[pair_idx].lo);
        }

        NumberRepr curr = get_next_dseq_num(pairs[pair_idx].lo);
        while (num_rep_cmp(curr, pairs[pair_idx].hi) <= 0)
        {
            sol_rep = num_repr_add(sol_rep, curr);
            curr = get_next_dseq_num(curr);
        }
    }

    printf("P1: %s\n", sol_rep.repr);
}

// run with "gcc main.c -o main.exe -std=c99 && ./main.exe"
int main(int argc, char *argv[])
{
    NumberPair pairs[MAX_NUM_PAIRS];
    size_t pairs_len;

    read_file("test.txt", pairs, &pairs_len);
    // read_file("input.txt", pairs, &pairs_len);
    p1(pairs, pairs_len);

    printf("OK");
    return 0;
}