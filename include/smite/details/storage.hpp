/*
** Created by doom on 16/12/18.
*/

#ifndef SMITE_DETAILS_STORAGE_HPP
#define SMITE_DETAILS_STORAGE_HPP

#include <type_traits>
#include <utility>

namespace smite::details
{
    template <typename T, typename Tag = void, bool Empty = std::is_empty_v<T>>
    class storage
    {
    private:
        using this_type = storage<T, Tag, Empty>;

    public:
        using uses_ebo = std::false_type;

        template <typename ...Args, typename = std::enable_if_t<std::is_constructible_v<T, Args...>>>
        constexpr storage(Args &&...args) : _wrapped(std::forward<Args>(args)...)
        {
        }

        constexpr storage() = default;

        constexpr storage(const storage &) = default;

        constexpr storage(storage &&) = default;

        constexpr storage &operator=(const storage &) = default;

        constexpr storage &operator=(storage &&) = default;

        constexpr T &get() & noexcept
        {
            return _wrapped;
        }

        constexpr const T &get() const & noexcept
        {
            return _wrapped;
        }

        constexpr T &&get() && noexcept
        {
            return std::move(_wrapped);
        }

        constexpr const T &&get() const && noexcept
        {
            return std::move(_wrapped);
        }

    private:
        T _wrapped;
    };

    template <typename T, typename Tag>
    class storage<T, Tag, true> : T
    {
    public:
        using uses_ebo = std::true_type;

        using T::T;

        constexpr storage() = default;

        template <typename U, typename = std::enable_if_t<
            std::is_same_v<T, std::decay_t<U>> && std::is_constructible_v<T, U>>
        >
        constexpr storage(U &&u) : T(std::forward<U>(u))
        {
        }

        constexpr storage(const storage &) = default;

        constexpr storage(storage &&) = default;

        constexpr storage &operator=(const storage &) = default;

        constexpr storage &operator=(storage &&) = default;

        constexpr T &get() & noexcept
        {
            return static_cast<T &>(*this);
        }

        constexpr const T &get() const & noexcept
        {
            return static_cast<const T &>(*this);
        }

        constexpr T &&get() && noexcept
        {
            return std::move(static_cast<T &&>(*this));
        }

        constexpr const T &&get() const && noexcept
        {
            return std::move(static_cast<const T &&>(*this));
        }
    };
}

#endif /* !SMITE_DETAILS_STORAGE_HPP */
