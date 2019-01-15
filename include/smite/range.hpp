/*
** Created by doom on 14/12/18.
*/

#ifndef SMITE_RANGE_HPP
#define SMITE_RANGE_HPP

#include <utility>

namespace smite
{
    template <typename Iter>
    class range
    {
    public:
        using iterator = Iter;

        constexpr range(iterator b, iterator e) : _begin(b), _end(e)
        {
        }

        constexpr range(const range &other) = default;

        constexpr range(range &&other) = default;

        constexpr range &operator=(const range &other) = default;

        constexpr range &operator=(range &&other) = default;

        constexpr iterator begin() const
        {
            return _begin;
        }

        constexpr iterator end() const
        {
            return _end;
        }

    private:
        iterator _begin;
        iterator _end;
    };

    template <typename Iter>
    inline constexpr range<Iter> make_range(Iter begin, Iter end)
    {
        return range<Iter>{begin, end};
    }

    struct range_maker_tag
    {
    };

    namespace details
    {
        template <typename T, typename = void>
        struct is_range_maker_helper : std::false_type
        {
        };

        template <typename T>
        struct is_range_maker_helper<T, std::void_t<typename T::smite_tag>> :
            std::is_same<typename T::smite_tag, range_maker_tag>
        {
        };

        template <typename T, typename = void>
        struct is_range_helper : std::false_type
        {
        };

        template <typename T>
        struct is_range_helper<T, std::void_t<decltype(std::declval<T &>().begin())>> : std::true_type
        {
        };
    }

    template <typename T>
    struct is_range : details::is_range_helper<T>
    {
    };

    template <typename T>
    struct is_range_maker : details::is_range_maker_helper<T>
    {
    };

    template <typename T>
    inline constexpr bool is_range_maker_v = is_range_maker<T>::value;

    template <typename T>
    inline constexpr bool is_range_v = is_range<T>::value;

    template <typename Func>
    struct range_maker
    {
        using smite_tag = range_maker_tag;

        template <typename ...Args>
        constexpr auto operator()(Args &&...args) const
        {
            return _f(std::forward<Args>(args)...);
        }

        Func _f;
    };

    template <typename M1, typename M2, std::enable_if_t<is_range_maker_v<M1> && is_range_maker_v<M2>, int> = 0>
    inline constexpr auto operator|(const M1 &m1, const M2 &m2)
    {
        auto f = [m1, m2](auto &&...params) {
            return m2(m1(params...));
        };
        return range_maker<decltype(f)>{std::move(f)};
    }

    template <typename R, typename M, std::enable_if_t<is_range_v<R> && is_range_maker_v<M>, int> = 0>
    inline constexpr auto operator|(const R &r, const M &m)
    {
        return m(r);
    }
}

#endif /* !SMITE_RANGE_HPP */
