/*
** Created by doom on 03/02/19.
*/

#ifndef SMITE_MULTISTEP_ITERATOR_HPP
#define SMITE_MULTISTEP_ITERATOR_HPP

#include <utility>
#include <iterator>
#include <smite/range.hpp>

namespace smite
{
    template <typename Iter>
    class multistep_iterator
    {
    public:
        using iterator_type = Iter;

    private:
        using iterator_traits = std::iterator_traits<Iter>;

    public:
        using difference_type = typename iterator_traits::difference_type;
        using value_type = typename iterator_traits::value_type;
        using reference = typename iterator_traits::reference;
        using pointer = typename iterator_traits::pointer;
        using iterator_category = typename iterator_traits::iterator_category;

        constexpr multistep_iterator(Iter iter, std::size_t step, Iter end = Iter()) :
            _iter(iter), _end(end), _step(step)
        {
        }

        constexpr multistep_iterator(const multistep_iterator &) = default;

        constexpr multistep_iterator(multistep_iterator &&) = default;

        constexpr multistep_iterator &operator=(const multistep_iterator &) = default;

        constexpr multistep_iterator &operator=(multistep_iterator &&) = default;

        constexpr pointer operator->() const
        {
            return _iter;
        }

        constexpr reference operator*() const
        {
            return *_iter;
        }

        constexpr multistep_iterator &operator++()
        {
            ++_iter;
            for (std::size_t i = 1; i < _step && _iter != _end; ++i) {
                ++_iter;
            }
            return *this;
        }

        constexpr const multistep_iterator operator++(int)
        {
            auto tmp = *this;

            ++*this;
            return tmp;
        }

        constexpr multistep_iterator &operator--()
        {
            --_iter;
            for (std::size_t i = 1; i < _step; ++i) {
                --_iter;
            }
            return *this;
        }

        constexpr const multistep_iterator operator--(int)
        {
            auto tmp = *this;

            --*this;
            return tmp;
        }

        constexpr multistep_iterator operator+(difference_type n)
        {
            auto tmp = *this;

            tmp += n;
            return tmp;
        }

        constexpr multistep_iterator &operator+=(difference_type n)
        {
            while (n--) {
                ++*this;
            }
            return *this;
        }

        constexpr multistep_iterator operator-(difference_type n)
        {
            auto tmp = *this;

            tmp -= n;
            return tmp;
        }

        constexpr difference_type operator-(multistep_iterator other)
        {
            auto tmp = *this;
            difference_type n = 0;

            while (tmp.base() != other.base()) {
                ++other;
                ++n;
            }
            return n;
        }

        constexpr multistep_iterator &operator-=(difference_type n)
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

        constexpr const std::size_t &step() const noexcept
        {
            return _step;
        }

    private:
        Iter _iter;
        Iter _end;
        std::size_t _step;
    };

    template <typename Iter>
    inline constexpr bool
    operator==(const multistep_iterator<Iter> &lhs, const multistep_iterator<Iter> &rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <typename Iter>
    inline constexpr bool
    operator!=(const multistep_iterator<Iter> &lhs, const multistep_iterator<Iter> &rhs)
    {
        return !(rhs == lhs);
    }

    template <typename Iter>
    inline constexpr bool
    operator<(const multistep_iterator<Iter> &lhs, const multistep_iterator<Iter> &rhs)
    {
        return lhs.base() < rhs.base();
    }

    template <typename Iter>
    inline constexpr bool
    operator>(const multistep_iterator<Iter> &lhs, const multistep_iterator<Iter> &rhs)
    {
        return rhs < lhs;
    }

    template <typename Iter>
    inline constexpr bool
    operator<=(const multistep_iterator<Iter> &lhs, const multistep_iterator<Iter> &rhs)
    {
        return !(rhs < lhs);
    }

    template <typename Iter>
    inline constexpr bool
    operator>=(const multistep_iterator<Iter> &lhs, const multistep_iterator<Iter> &rhs)
    {
        return !(lhs < rhs);
    }

    template <typename Iter>
    inline constexpr auto make_step_iterator(Iter iter, std::size_t step, Iter end = Iter())
    {
        return multistep_iterator<Iter>(iter, step, end);
    }

    template <typename Container>
    inline constexpr auto step(Container &&container, std::size_t step)
    {
        return make_range(
            make_step_iterator(std::begin(std::forward<Container>(container)), step,
                               std::end(std::forward<Container>(container))),
            make_step_iterator(std::end(std::forward<Container>(container)), step,
                               std::end(std::forward<Container>(container)))
        );
    }

    namespace details
    {
        struct step_maker
        {
            using smite_tag = range_maker_tag;

            template <typename Range>
            constexpr auto operator()(Range &&rng) const
            {
                return step(std::forward<Range>(rng), _step);
            }

            std::size_t _step;
        };
    }

    inline constexpr auto make_step(std::size_t step)
    {
        return details::step_maker{step};
    }
}

#endif /* !SMITE_MULTISTEP_ITERATOR_HPP */
