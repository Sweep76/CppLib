#include <iostream>
#include <string>
#include "Deque.h"
#include "gtest/gtest.h"

using std::string;

class TestDeque : public testing::Test
{
protected:
    Deque<string> deque;
    Deque<string> a;
    Deque<string> b;
    Deque<string> c;
    size_t scale;
public:
    virtual void SetUp() { scale = 32; }
    virtual void TearDown() {}

    void insert_n(Deque<string>& s, size_t n, bool at_back = true)
    {
        if (at_back)
        {
            for (size_t i = 0; i < n; ++i)
                s.insert_back(std::to_string(i));
        }
        else
        {
            for (size_t i = 0; i < n; ++i)
                s.insert_front(std::to_string(i));
        }
    }
    void remove_n(Deque<string>& s, size_t n, bool at_back = true)
    {
        if (at_back)
        {
            for (size_t i = 0; i < n; ++i)
                s.remove_back();
        }
        else
        {
            for (size_t i = 0; i < n; ++i)
                s.remove_front();
        }
    }
};

TEST_F(TestDeque, Basic)
{
    EXPECT_NO_THROW({
        Deque<string> s1;
        Deque<string> s2(scale);
        Deque<string> s3(scale, "Hello World!");
        Deque<string> s4(s1);
        Deque<string> s5(Deque<string>());

        s1 = s2;
        s2 = Deque<string>(scale);
    });
}

TEST_F(TestDeque, Capacity)
{
    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(size_t(0), deque.size());

    insert_n(deque, scale, true);
    EXPECT_EQ(scale, deque.size());
    remove_n(deque, scale, true);
    EXPECT_TRUE(deque.empty());

    insert_n(deque, scale, false);
    EXPECT_EQ(scale, deque.size());
    remove_n(deque, scale, false);
    EXPECT_TRUE(deque.empty());

    insert_n(deque, scale, false);
    EXPECT_NO_THROW({
        deque.shrink_to_fit();
    });
    EXPECT_EQ(scale, deque.size());
}

TEST_F(TestDeque, ElementAccess)
{
    EXPECT_THROW(deque.front(), std::out_of_range);
    EXPECT_THROW(deque.back(), std::out_of_range);
    EXPECT_NO_THROW({
        for (size_t i = 0; i < scale; ++i)
        {
            deque.insert_back(std::to_string(i));
            EXPECT_EQ(std::to_string(i), deque.back());
        }
        for (size_t i = 0; i < scale; ++i)
        {
            EXPECT_EQ(std::to_string(i), deque.front());
            deque.remove_front();
        }
        for (size_t i = 0; i < scale; ++i)
        {
            deque.insert_front(std::to_string(i));
            EXPECT_EQ(std::to_string(i), deque.front());
        }
        for (size_t i = 0; i < scale; ++i)
        {
            EXPECT_EQ(std::to_string(i), deque.back());
            deque.remove_back();
        }
    });
    EXPECT_THROW(deque.front(), std::out_of_range);
    EXPECT_THROW(deque.back(), std::out_of_range);

    insert_n(deque, scale);
    for (size_t i = 0; i < scale; ++i)
    {
        EXPECT_EQ(std::to_string(i), deque.at(i));
        EXPECT_EQ(std::to_string(i), deque[i]);
    }
    EXPECT_THROW(deque.at(-1), std::out_of_range);
    EXPECT_THROW(deque.at(scale), std::out_of_range);
}

TEST_F(TestDeque, Iterators)
{
    EXPECT_TRUE(deque.begin() == deque.end());
    insert_n(deque, scale);
    EXPECT_FALSE(deque.begin() == deque.end());

    auto bg = deque.begin();
    auto ed = deque.end();
//    auto rbg = deque.rbegin();
//    auto red = deque.rend();

    for (size_t i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), *bg++);
    EXPECT_TRUE(bg == deque.end());
    for (size_t i = scale; i > 0; --i)
        EXPECT_EQ(std::to_string(i - 1), *--ed);
    EXPECT_TRUE(ed == deque.begin());

//    for (size_t i = scale - 1; i >= 0; --i)
//        EXPECT_EQ(std::to_string(i), *rbg++);
//    EXPECT_EQ(rbg, deque.rend());
//    for (size_t i = 0; i < scale; ++i)
//        EXPECT_EQ(std::to_string(i), *--red);
//    EXPECT_EQ(red, deque.rbegin());

    // EXPECT_TRUE(std::is_const(std::iterator_traits<Deque::const_iterator>::value_type));
    // EXPECT_TRUE(std::is_const(std::iterator_traits<Deque::const_iterator>::value_type));
}

TEST_F(TestDeque, Modifiers)
{
    EXPECT_THROW(deque.remove_back(), std::out_of_range);
    EXPECT_THROW(deque.remove_front(), std::out_of_range);
    EXPECT_THROW(deque.remove(deque.begin()), std::out_of_range);
    EXPECT_THROW(deque.remove(deque.end()), std::out_of_range);

    EXPECT_NO_THROW({
        insert_n(deque, scale, true);
        for (size_t i = 0; i < scale; ++i)
        {
            EXPECT_EQ(std::to_string(i), deque.front());
            deque.remove_front();
        }
        insert_n(deque, scale, false);
        for (size_t i = 0; i < scale; ++i)
        {
            EXPECT_EQ(std::to_string(i), deque.back());
            deque.remove_back();
        }
//
//        for (size_t i = 0; i < scale; ++i)
//            deque.insert(deque.begin(), std::to_string(i));
//        for (size_t i = 0; i < scale; ++i)
//        {
//            EXPECT_EQ(std::to_string(i), deque.back());
//            deque.remove(std::prev(deque.end());
//        }
//        for (size_t i = 0; i < scale; ++i)
//            deque.insert(deque.end(), std::to_string(i));
//        for (size_t i = 0; i < scale; ++i)
//        {
//            EXPECT_EQ(std::to_string(i), deque.front());
//            deque.remove(deque.begin());
//        }
    });
    EXPECT_THROW(deque.remove_back(), std::out_of_range);
    EXPECT_THROW(deque.remove_front(), std::out_of_range);
    EXPECT_THROW(deque.remove(deque.begin()), std::out_of_range);
    EXPECT_THROW(deque.remove(deque.end()), std::out_of_range);

    insert_n(deque, scale);
    deque.clear();
    EXPECT_TRUE(deque.empty());
    EXPECT_THROW(deque.remove_back(), std::out_of_range);

    insert_n(a, scale);
    b.swap(a);
    EXPECT_EQ(scale, b.size());
    for (size_t i = 0; i < scale; ++i)
    {
        EXPECT_EQ(std::to_string(i), b.front());
        b.remove_front();
    }
}

TEST_F(TestDeque, Other)
{
    using std::swap;
    insert_n(a, scale);
    c = a;
    EXPECT_TRUE(c == a && c != b);
    b.swap(a);
    EXPECT_TRUE(c != a && c == b);
    swap(a, b);
    EXPECT_TRUE(c == a && c != b);
}