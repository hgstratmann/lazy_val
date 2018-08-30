/*
 *  Copyright (c) 2018 Henry G. Stratmann III
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include "lazy_val.h"
#include <gtest/gtest.h>
#include <string>

static int constructorCallCount;
static int destructorCallCount;

class LazyValueTest : public testing::Test
{
protected:
    LazyValueTest()
    {
        constructorCallCount = 0;
        destructorCallCount = 0;
    }

    struct Foobar
    {
        Foobar()
        {
            constructorCallCount += 1;
        }

        Foobar(int x, const std::string &y, bool z) : x(x), y(y), z(z)
        {
            constructorCallCount += 1;
        }

        ~Foobar()
        {
            destructorCallCount += 1;
        }

        int x = 0;
        std::string y;
        bool z = false;
    };
};

TEST_F(LazyValueTest, uninitialized)
{
    {
        lazy_val<Foobar> foobar;
    }

    EXPECT_EQ(constructorCallCount, 0);
    EXPECT_EQ(destructorCallCount, 0);
}

TEST_F(LazyValueTest, init)
{
    {
        lazy_val<Foobar> foobar;
        foobar.init();
    }

    EXPECT_EQ(constructorCallCount, 1);
    EXPECT_EQ(destructorCallCount, 1);
}

TEST_F(LazyValueTest, init_with_arguments)
{
    {
        lazy_val<Foobar> foobar;
        foobar.init(42, "Hello World!", true);

        EXPECT_EQ(foobar->x, 42);
        EXPECT_EQ(foobar->y, "Hello World!");
        EXPECT_EQ(foobar->z, true);
    }

    EXPECT_EQ(constructorCallCount, 1);
    EXPECT_EQ(destructorCallCount, 1);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
