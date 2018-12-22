/*
** Created by doom on 14/12/18.
*/

#include <gtest/gtest.h>
#include <vector>
#include <list>
#include <numeric>
#include <smite/smite.hpp>
#include <smite/details/compressed_pair.hpp>

namespace
{
    template <typename>
    struct empty
    {
    };

    struct not_copyable
    {
        not_copyable() = default;

        not_copyable(const not_copyable &) = delete;
    };

    struct throw_copyable
    {
        throw_copyable() = default;

        throw_copyable(const throw_copyable &) noexcept(false)
        {
        }
    };
}

#define TRAIT_CONJUNCTION(name, trait)                                                   \
    template <typename ...Ts>                                                       \
    static inline constexpr bool lowest_##name##_v = std::conjunction<trait<Ts>...>::value

TRAIT_CONJUNCTION(copy_constructor_triviality, std::is_trivially_copy_constructible);
TRAIT_CONJUNCTION(move_constructor_triviality, std::is_trivially_move_constructible);
TRAIT_CONJUNCTION(copy_assignment_triviality, std::is_trivially_copy_assignable);
TRAIT_CONJUNCTION(move_assignment_triviality, std::is_trivially_move_assignable);
TRAIT_CONJUNCTION(copy_constructor_noexceptness, std::is_nothrow_copy_constructible);
TRAIT_CONJUNCTION(move_constructor_noexceptness, std::is_nothrow_move_constructible);
TRAIT_CONJUNCTION(copy_assignment_noexceptness, std::is_nothrow_copy_assignable);
TRAIT_CONJUNCTION(move_assignment_noexceptness, std::is_nothrow_move_assignable);
TRAIT_CONJUNCTION(copy_constructibility, std::is_copy_constructible);
TRAIT_CONJUNCTION(move_constructibility, std::is_move_constructible);
TRAIT_CONJUNCTION(copy_assignability, std::is_copy_assignable);
TRAIT_CONJUNCTION(move_assignability, std::is_move_assignable);

template <typename T, typename ...Us>
static inline constexpr bool check_same_triviality_v =
    (std::is_trivially_copy_constructible_v<T> == lowest_copy_constructor_triviality_v<Us...>) &&
    (std::is_trivially_move_constructible_v<T> == lowest_move_constructor_triviality_v<Us...>) &&
    (std::is_trivially_copy_assignable_v<T> == lowest_copy_assignment_triviality_v<Us...>) &&
    (std::is_trivially_move_assignable_v<T> == lowest_move_assignment_triviality_v<Us...>);

template <typename T, typename ...Us>
static inline constexpr bool check_same_noexceptness_v =
    (std::is_nothrow_copy_constructible_v<T> == lowest_copy_constructor_noexceptness_v<Us...>) &&
    (std::is_nothrow_move_constructible_v<T> == lowest_move_constructor_noexceptness_v<Us...>) &&
    (std::is_nothrow_copy_assignable_v<T> == lowest_copy_assignment_noexceptness_v<Us...>) &&
    (std::is_nothrow_move_assignable_v<T> == lowest_move_assignment_noexceptness_v<Us...>);

template <typename T, typename ...Us>
static inline constexpr bool check_same_copyability_v =
    (std::is_copy_constructible_v<T> == lowest_copy_constructibility_v<Us...>) &&
    (std::is_copy_assignable_v<T> == lowest_copy_assignability_v<Us...>);

template <typename T, typename ...Us>
static inline constexpr bool check_same_movability_v =
    (std::is_move_constructible_v<T> == lowest_move_constructibility_v<Us...>) &&
    (std::is_move_assignable_v<T> == lowest_move_assignability_v<Us...>);


TEST(smite_details, compressed_pair)
{
    using test1 = smite::details::compressed_pair<empty<char>, empty<char>>;
    using test2 = smite::details::compressed_pair<char, int>;
    using test3 = smite::details::compressed_pair<empty<char>, empty<int>>;
    using test4 = smite::details::compressed_pair<int, not_copyable>;
    using test5 = smite::details::compressed_pair<not_copyable, throw_copyable>;

    static_assert(test1::first_base::uses_ebo::value);
    static_assert(test1::second_base::uses_ebo::value);
    static_assert(sizeof(test1) == 2);
    static_assert(check_same_triviality_v<test1, test1::first_type, test1::second_type>);
    static_assert(check_same_noexceptness_v<test1, test1::first_type, test1::second_type>);
    static_assert(check_same_copyability_v<test1, test1::first_type, test1::second_type>);
    static_assert(check_same_movability_v<test1, test1::first_type, test1::second_type>);

    static_assert(!test2::first_base::uses_ebo::value);
    static_assert(!test2::second_base::uses_ebo::value);
    static_assert(check_same_triviality_v<test2, test2::first_type, test2::second_type>);
    static_assert(check_same_noexceptness_v<test2, test2::first_type, test2::second_type>);
    static_assert(check_same_copyability_v<test2, test2::first_type, test2::second_type>);
    static_assert(check_same_movability_v<test2, test2::first_type, test2::second_type>);

    static_assert(test3::first_base::uses_ebo::value);
    static_assert(test3::second_base::uses_ebo::value);
    static_assert(sizeof(test3) == 1);
    static_assert(check_same_triviality_v<test3, test3::first_type, test3::second_type>);
    static_assert(check_same_noexceptness_v<test3, test3::first_type, test3::second_type>);
    static_assert(check_same_copyability_v<test3, test3::first_type, test3::second_type>);
    static_assert(check_same_movability_v<test3, test3::first_type, test3::second_type>);

    static_assert(!std::is_copy_constructible_v<not_copyable>);
    static_assert(!test4::first_base::uses_ebo::value);
    static_assert(test4::second_base::uses_ebo::value);
    static_assert(check_same_triviality_v<test4, test4::first_type, test4::second_type>);
    static_assert(check_same_noexceptness_v<test4, test4::first_type, test4::second_type>);
    static_assert(check_same_copyability_v<test4, test4::first_type, test4::second_type>);
    static_assert(check_same_movability_v<test4, test4::first_type, test4::second_type>);

    static_assert(!std::is_nothrow_copy_constructible_v<throw_copyable>);
    static_assert(test5::first_base::uses_ebo::value);
    static_assert(test5::second_base::uses_ebo::value);
    static_assert(check_same_triviality_v<test5, test5::first_type, test5::second_type>);
    static_assert(check_same_noexceptness_v<test5, test5::first_type, test5::second_type>);
    static_assert(check_same_copyability_v<test5, test5::first_type, test5::second_type>);
    static_assert(check_same_movability_v<test5, test5::first_type, test5::second_type>);
}

template <typename T, typename U>
static inline constexpr bool check_iterator_category_v = std::is_same_v<
    typename T::iterator_category,
    typename U::iterator_category
>;

template <typename T, typename U>
static inline constexpr bool check_difference_type_v = std::is_same_v<
    typename T::difference_type,
    typename U::difference_type
>;

template <typename T, typename U>
static inline constexpr bool check_value_type_v = std::is_same_v<
    typename T::value_type,
    typename U::value_type
>;

TEST(smite, transform_iterator)
{
    std::vector<int> vec(10, 0);

    std::iota(vec.begin(), vec.end(), 0);

    auto a = [](int i) {
        return i * 2;
    };
    auto rng = smite::transform(vec, a);

    size_t i = 0;
    for (const auto &cur : rng) {
        ASSERT_EQ(cur, i * 2);
        ++i;
    }

    using iter = typename decltype(rng)::iterator;
    using iter_traits = std::iterator_traits<iter>;
    using veciter_traits = std::iterator_traits<std::vector<int>::iterator>;
    static_assert(check_iterator_category_v<iter_traits, veciter_traits>);
    static_assert(check_difference_type_v<iter_traits, veciter_traits>);
    static_assert(check_value_type_v<iter_traits, veciter_traits>);
    static_assert(check_same_triviality_v<iter, iter::iterator_type, iter::transformer_type>);
    static_assert(check_same_noexceptness_v<iter, iter::iterator_type, iter::transformer_type>);
    static_assert(check_same_copyability_v<iter, iter::iterator_type, iter::transformer_type>);
    static_assert(check_same_movability_v<iter, iter::iterator_type, iter::transformer_type>);

    static_assert(sizeof(iter) == sizeof(std::vector<int>::iterator));
}

TEST(smite, enumerate_iterator)
{
    std::vector<int> vec(10, 0);

    auto rng = smite::enumerate(vec);

    for (auto[idx, cur] : rng) {
        cur = static_cast<int>(idx);
    }

    size_t i = 0;
    for (const auto &cur : vec) {
        ASSERT_EQ(cur, i);
        ++i;
    }

    using iter = typename decltype(rng)::iterator;
    using iter_traits = std::iterator_traits<iter>;
    using veciter_traits = std::iterator_traits<std::vector<int>::iterator>;
    static_assert(check_iterator_category_v<iter_traits, veciter_traits>);
    static_assert(check_difference_type_v<iter_traits, veciter_traits>);
    static_assert(check_value_type_v<iter_traits, veciter_traits>);
    static_assert(check_same_triviality_v<iter, iter::iterator_type, iter::difference_type>);
    static_assert(check_same_noexceptness_v<iter, iter::iterator_type, iter::difference_type>);
    static_assert(check_same_copyability_v<iter, iter::iterator_type, iter::difference_type>);
    static_assert(check_same_movability_v<iter, iter::iterator_type, iter::difference_type>);
}

TEST(smite, filter_iterator)
{
    std::vector<int> vec(10, 0);
    auto pred = [](int i) {
        return i % 2 == 0;
    };

    std::iota(vec.begin(), vec.end(), 0);
    auto rng = smite::filter(vec, pred);
    ASSERT_EQ(std::distance(rng.begin(), rng.end()), 5);
    std::vector<int> other{rng.begin(), rng.end()};
    ASSERT_EQ(other, (std::vector<int>{0, 2, 4, 6, 8}));

    std::iota(vec.begin(), vec.end(), 1);
    auto rng2 = smite::filter(vec, pred);
    ASSERT_EQ(std::distance(rng2.begin(), rng2.end()), 5);
    std::vector<int> other2{rng2.begin(), rng2.end()};
    ASSERT_EQ(other2, (std::vector<int>{2, 4, 6, 8, 10}));

    using iter = typename decltype(rng)::iterator;
    using iter_traits = std::iterator_traits<iter>;
    using veciter_traits = std::iterator_traits<std::vector<int>::iterator>;
    static_assert(check_iterator_category_v<iter_traits, veciter_traits>);
    static_assert(check_difference_type_v<iter_traits, veciter_traits>);
    static_assert(check_value_type_v<iter_traits, veciter_traits>);
    static_assert(check_same_triviality_v<iter, iter::iterator_type, iter::predicate_type>);
    static_assert(check_same_noexceptness_v<iter, iter::iterator_type, iter::predicate_type>);
    static_assert(check_same_copyability_v<iter, iter::iterator_type, iter::predicate_type>);
    static_assert(check_same_movability_v<iter, iter::iterator_type, iter::predicate_type>);
}

TEST(smite, zip_iterator)
{
    std::vector<int> vec(5, 0);
    std::vector<float> vec2(5, 0.0);

    auto rng = smite::zip(vec, vec2);

    for (auto[idx, p] : smite::enumerate(rng)) {
        auto[i, f] = p;

        i = static_cast<int>(idx);
        f = idx;
    }

    ASSERT_EQ(vec, (std::vector<int>{0, 1, 2, 3, 4}));
    ASSERT_EQ(vec2, (std::vector<float>{0, 1, 2, 3, 4}));

    using iter = typename decltype(rng)::iterator;
    using iter_traits = std::iterator_traits<iter>;
    using veciter_traits = std::iterator_traits<std::vector<int>::iterator>;
    static_assert(check_iterator_category_v<iter_traits, veciter_traits>);
    static_assert(check_difference_type_v<iter_traits, veciter_traits>);
    static_assert(check_value_type_v<iter_traits, veciter_traits>);
    static_assert(check_same_triviality_v<iter, iter::first_iterator_type, iter::second_iterator_type>);
    static_assert(check_same_noexceptness_v<iter, iter::first_iterator_type, iter::second_iterator_type>);
    static_assert(check_same_copyability_v<iter, iter::first_iterator_type, iter::second_iterator_type>);
    static_assert(check_same_movability_v<iter, iter::first_iterator_type, iter::second_iterator_type>);
    using iter_traits2 = std::iterator_traits<smite::zip_iterator<std::vector<int>::iterator, std::list<int>::iterator>>;
    static_assert(std::is_same_v<iter_traits2::iterator_category, std::bidirectional_iterator_tag>);
}

TEST(smite, combination)
{
    using namespace smite;
    std::vector<int> in(10, 0);
    std::vector<int> out;

    std::iota(in.begin(), in.end(), 0);
    auto take_index = make_transform([](auto &&pair) { return pair.first; });
    auto keep_if_even = make_filter([](auto &&pair) { return pair.second % 2 == 0; });
    auto rng = in | enumerate | keep_if_even | take_index;
    std::copy(rng.begin(), rng.end(), std::back_inserter(out));
    ASSERT_EQ(out, (std::vector<int>{0, 2, 4, 6, 8}));

    static_assert(smite::is_range_maker_v<decltype(take_index)>);
}
