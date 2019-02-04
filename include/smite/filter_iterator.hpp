/*
** Created by doom on 15/12/18.
*/

#ifndef SMITE_FILTER_ITERATOR_HPP
#define SMITE_FILTER_ITERATOR_HPP

#include <utility>
#include <iterator>
#include <smite/range.hpp>

namespace smite
{
    template <typename Iter, typename Predicate>
    class filter_iterator
    {
    public:
        using iterator_type = Iter;
        using predicate_type = Predicate;

    private:
        using iterator_traits = std::iterator_traits<Iter>;

    public:
        using difference_type = typename iterator_traits::difference_type;
        using value_type = typename iterator_traits::value_type;
        using reference = typename iterator_traits::reference;
        using pointer = typename iterator_traits::pointer;
        using iterator_category = typename iterator_traits::iterator_category;

        constexpr filter_iterator(Iter iter, Predicate pred, Iter end = Iter()) :
            _iter(iter), _end(end), _predicate(pred)
        {
            while (!_is_satisfying()) {
                ++_iter;
            }
        }

        constexpr filter_iterator(const filter_iterator &) = default;

        constexpr filter_iterator(filter_iterator &&) = default;

        constexpr filter_iterator &operator=(const filter_iterator &) = default;

        constexpr filter_iterator &operator=(filter_iterator &&) = default;

        constexpr pointer operator->() const
        {
            return _iter;
        }

        constexpr reference operator*() const
        {
            return *_iter;
        }

    private:
        constexpr bool _is_satisfying() const
        {
            return _iter == _end || _predicate(*_iter);
        }

    public:
        constexpr filter_iterator &operator++()
        {
            ++_iter;
            while (!_is_satisfying()) {
                ++_iter;
            }
            return *this;
        }

        constexpr const filter_iterator operator++(int)
        {
            auto tmp = *this;

            ++*this;
            return tmp;
        }

        constexpr filter_iterator &operator--()
        {
            --_iter;
            while (!_is_satisfying()) {
                --_iter;
            }
            return *this;
        }

        constexpr const filter_iterator operator--(int)
        {
            auto tmp = *this;

            --*this;
            return tmp;
        }

        constexpr filter_iterator operator+(difference_type n)
        {
            auto tmp = *this;

            tmp += n;
            return tmp;
        }

        constexpr filter_iterator &operator+=(difference_type n)
        {
            while (n--) {
                ++*this;
            }
            return *this;
        }

        constexpr filter_iterator operator-(difference_type n)
        {
            auto tmp = *this;

            tmp -= n;
            return tmp;
        }

        constexpr difference_type operator-(filter_iterator other)
        {
            auto tmp = *this;
            difference_type n = 0;

            while (tmp.base() != other.base()) {
                ++other;
                ++n;
            }
            return n;
        }

        constexpr filter_iterator &operator-=(difference_type n)
        {
            while (n--) {
                --*this;
            }
            return *this;
        }

        constexpr const iterator_type &base() const noexcept
        {
            return _iter;
        }

        constexpr iterator_type &base() noexcept
        {
            return _iter;
        }

        constexpr const predicate_type &predicate() const noexcept
        {
            return _predicate;
        }

    private:
        Iter _iter;
        Iter _end;
        Predicate _predicate;
    };

    template <typename Iter, typename Predicate>
    inline constexpr bool operator==(const filter_iterator<Iter, Predicate> &lhs, const filter_iterator<Iter, Predicate> &rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <typename Iter, typename Predicate>
    inline constexpr bool operator!=(const filter_iterator<Iter, Predicate> &lhs, const filter_iterator<Iter, Predicate> &rhs)
    {
        return !(rhs == lhs);
    }

    template <typename Iter, typename Predicate>
    inline constexpr bool operator<(const filter_iterator<Iter, Predicate> &lhs, const filter_iterator<Iter, Predicate> &rhs)
    {
        return lhs.base() < rhs.base();
    }

    template <typename Iter, typename Predicate>
    inline constexpr bool operator>(const filter_iterator<Iter, Predicate> &lhs, const filter_iterator<Iter, Predicate> &rhs)
    {
        return rhs < lhs;
    }

    template <typename Iter, typename Predicate>
    inline constexpr bool operator<=(const filter_iterator<Iter, Predicate> &lhs, const filter_iterator<Iter, Predicate> &rhs)
    {
        return !(rhs < lhs);
    }

    template <typename Iter, typename Predicate>
    inline constexpr bool operator>=(const filter_iterator<Iter, Predicate> &lhs, const filter_iterator<Iter, Predicate> &rhs)
    {
        return !(lhs < rhs);
    }

    template <typename Iter, typename Predicate>
    inline constexpr auto make_filter_iterator(Iter iter, Predicate &&predicate, Iter end = Iter())
    {
        return filter_iterator<Iter, Predicate>(iter, std::forward<Predicate>(predicate), end);
    }

    template <typename Container, typename Predicate>
    inline constexpr auto filter(Container &&container, Predicate &&predicate)
    {
        return make_range(
            make_filter_iterator(std::begin(std::forward<Container>(container)), predicate,
                                 std::end(std::forward<Container>(container))),
            make_filter_iterator(std::end(std::forward<Container>(container)), predicate,
                                 std::end(std::forward<Container>(container)))
        );
    }

    namespace details
    {
        template <typename Predicate>
        struct filter_maker
        {
            using smite_tag = range_maker_tag;

            template <typename Range>
            constexpr auto operator()(Range &&rng) const
            {
                return filter(std::forward<Range>(rng), _predicate);
            }

            Predicate _predicate;
        };
    }

    template <typename Predicate>
    inline constexpr auto make_filter(Predicate &&predicate)
    {
        return details::filter_maker<std::decay_t<Predicate>>{std::forward<Predicate>(predicate)};
    }
}

#endif /* !SMITE_FILTER_ITERATOR_HPP */
