/*
 * SPDX-FileCopyrightText: 2023 Dmitrii Lebed <lebed.dmitry@gmail.com>
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef REG_OP_COMPRESS_HPP
#define REG_OP_COMPRESS_HPP

#include <cstdint>
#include <cstddef>

namespace hal {

struct reg_op_entry {
    uint16_t    offset;
    uint32_t    value;
};

constexpr size_t reg_op_comp_size(const struct reg_op_entry entries[])
{
    size_t size = 1;
    size_t i = 1;

    while (entries[i].offset != 0xFFFF) {
        if (entries[i - 1].value != entries[i].value) {
            size++;
        }

        i++;
    }

    return size;
}

struct reg_op_compressed_entry {
    uint16_t    offset;
    uint16_t    repeat_count;
    uint32_t    value;
};

template <const struct reg_op_entry orig[]>
struct reg_op_compressed {
    static constexpr size_t element_count = reg_op_comp_size(orig);

    struct reg_op_compressed_entry entries[element_count];

    consteval reg_op_compressed() : entries()
    {
        size_t repeat_count = 0;
        size_t out_idx = 0;
        size_t i = 1;

        entries[out_idx].offset = orig[0].offset;
        entries[out_idx].value = orig[0].value;

        while (orig[i].offset != 0xFFFF) {
            if (orig[i - 1].value == orig[i].value) {
                repeat_count++;
            } else {
                entries[out_idx].repeat_count = static_cast<uint16_t>(repeat_count);
                out_idx++;
                repeat_count = 0;

                entries[out_idx].offset = orig[i].offset;
                entries[out_idx].value = orig[i].value;
            }

            i++;
        }

        entries[out_idx].repeat_count = static_cast<uint16_t>(repeat_count);
    }
};


}

#endif /* REG_OP_COMPRESS_HPP */
