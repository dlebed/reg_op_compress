/*
 * SPDX-FileCopyrightText: 2023 Dmitrii Lebed <lebed.dmitry@gmail.com>
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gtest/gtest.h>

#include <reg_op_compress.hpp>

using namespace hal;

constexpr struct reg_op_entry reg_op[] = {
    { 0, 123 },
    { 4, 123 },
    { 8, 123 },
    { 12, 2 },
    { 16, 2 },

    {0xFFFF}
};

constexpr reg_op_compressed comp = reg_op_compressed<reg_op>();

TEST(BitFieldSetTest, BasicTest)
{
    EXPECT_EQ(reg_op_comp_size(reg_op), 2);

    EXPECT_EQ(comp.element_count, 2);

    EXPECT_EQ(comp.entries[0].offset, 0);
    EXPECT_EQ(comp.entries[0].repeat_count, 2);
    EXPECT_EQ(comp.entries[0].value, 123);

    EXPECT_EQ(comp.entries[1].offset, 12);
    EXPECT_EQ(comp.entries[1].repeat_count, 1);
    EXPECT_EQ(comp.entries[1].value, 2);

    EXPECT_EQ(sizeof(comp), 8 * 2);
}