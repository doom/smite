/*
** Created by doom on 17/12/18.
*/

#ifndef SMITE_DETAILS_COMPRESSED_PAIR_HPP
#define SMITE_DETAILS_COMPRESSED_PAIR_HPP

#include <type_traits>
#include <utility>
#include <smite/details/storage.hpp>

namespace smite::details
{
    template <typename T, typename U>
    struct compressed_pair :
        private storage<T, struct compressed_pair_first>,
        private storage<U, struct compressed_pair_second>
    {
        using first_base = storage<T, struct compressed_pair_first>;
        using second_base = storage<U, struct compressed_pair_second>;

        using first_type = T;
        using second_type = U;

        constexpr compressed_pair() = default;

        template <typename T2, typename U2>
        constexpr compressed_pair(T2 &&t2, U2 &&u2) : first_base(std::forward<T2>(t2)),
                                                      second_base(std::forward<U2>(u2))
        {
        }

        constexpr compressed_pair(const compressed_pair &) = default;

        constexpr compressed_pair(compressed_pair &&) = default;

        constexpr compressed_pair &operator=(const compressed_pair &) = default;

        constexpr compressed_pair &operator=(compressed_pair &&) = default;

        constexpr T &first() & noexcept
        {
            return first_base::get();
        }

        constexpr U &second() & noexcept
        {
            return second_base::get();
        }

        constexpr const T &first() const & noexcept
        {
            return first_base::get();
        }

        constexpr const U &second() const & noexcept
        {
            return second_base::get();
        }

        constexpr T &&first() && noexcept
        {
            return std::move(first_base::get());
        }

        constexpr U &&second() && noexcept
        {
            return std::move(second_base::get());
        }

        constexpr const T &&first() const && noexcept
        {
            return std::move(first_base::get());
        }

        constexpr const U &&second() const && noexcept
        {
            return std::move(second_base::get());
        }
    };
}

#endif /* !SMITE_DETAILS_COMPRESSED_PAIR_HPP */
