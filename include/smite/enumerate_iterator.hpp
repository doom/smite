/*
** Created by doom on 14/12/18.
*/

#ifndef SMITE_ENUMERATE_ITERATOR_HPP
#define SMITE_ENUMERATE_ITERATOR_HPP

#include <utility>
#include <iterator>
#include <smite/range.hpp>
#include <smite/details/fake_ptr.hpp>
#include <smite/details/compressed_pair.hpp>

namespace smite
{
    template <typename Iter>
    class enumerate_iterator :
        private details::compressed_pair<Iter, typename std::iterator_traits<Iter>::difference_type>
    {
    private:
        using base_type = details::compressed_pair<Iter, typename std::iterator_traits<Iter>::difference_type>;
        using iterator_traits = std::iterator_traits<Iter>;

    public:
        using iterator_type = Iter;

    public:
        using difference_type = typename iterator_type::difference_type;
        using value_type = typename iterator_type::value_type;
        using reference = std::pair<difference_type, typename iterator_traits::reference>;
        using pointer = details::fake_ptr<reference>;
        using iterator_category = typename iterator_type::iterator_category;

        constexpr enumerate_iterator(iterator_type iter, difference_type start_at) : base_type(iter, start_at)
        {
        }

        constexpr enumerate_iterator(const enumerate_iterator &) = default;

        constexpr enumerate_iterator(enumerate_iterator &&) = default;

        constexpr enumerate_iterator &operator=(const enumerate_iterator &) = default;

        constexpr enumerate_iterator &operator=(enumerate_iterator &&) = default;

        constexpr pointer operator->() const
        {
            return pointer{**this};
        }

        constexpr reference operator*() const
        {
            return reference{count(), *base()};
        }

        constexpr enumerate_iterator &operator++()
        {
            ++base();
            ++count();
            return *this;
        }

        constexpr const enumerate_iterator operator++(int)
        {
            auto tmp = *this;

            ++*this;
            return tmp;
        }

        constexpr enumerate_iterator &operator--()
        {
            --base();
            --count();
            return *this;
        }

        constexpr const enumerate_iterator operator--(int)
        {
            auto tmp = *this;

            --*this;
            return tmp;
        }

        constexpr enumerate_iterator operator+(difference_type n)
        {
            return enumerate_iterator(base() + n, count() + n);
        }

        constexpr enumerate_iterator &operator+=(difference_type n)
        {
            base() += n;
            count() += n;
            return *this;
        }

        constexpr enumerate_iterator operator-(difference_type n)
        {
            return enumerate_iterator(base() - n, count() - n);
        }

        constexpr difference_type operator-(enumerate_iterator other)
        {
            return std::distance(other.base(), base());
        }

        constexpr enumerate_iterator &operator-=(difference_type n)
        {
            base() -= n;
            count() -= n;
            return *this;
        }

        constexpr iterator_type &base() noexcept
        {
            return base_type::first();
        }

        constexpr const iterator_type &base() const noexcept
        {
            return base_type::first();
        }

        constexpr difference_type &count() noexcept
        {
            return base_type::second();
        }

        constexpr const difference_type &count() const noexcept
        {
            return base_type::second();
        }
    };

    template <typename Iter>
    inline constexpr bool operator==(const enumerate_iterator<Iter> &lhs, const enumerate_iterator<Iter> &rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <typename Iter>
    inline constexpr bool operator!=(const enumerate_iterator<Iter> &lhs, const enumerate_iterator<Iter> &rhs)
    {
        return !(rhs == lhs);
    }

    template <typename Iter>
    inline constexpr bool operator<(const enumerate_iterator<Iter> &lhs, const enumerate_iterator<Iter> &rhs)
    {
        return lhs.base() < rhs.base();
    }

    template <typename Iter>
    inline constexpr bool operator>(const enumerate_iterator<Iter> &lhs, const enumerate_iterator<Iter> &rhs)
    {
        return rhs < lhs;
    }

    template <typename Iter>
    inline constexpr bool operator<=(const enumerate_iterator<Iter> &lhs, const enumerate_iterator<Iter> &rhs)
    {
        return !(rhs < lhs);
    }

    template <typename Iter>
    inline constexpr bool operator>=(const enumerate_iterator<Iter> &lhs, const enumerate_iterator<Iter> &rhs)
    {
        return !(lhs < rhs);
    }

    template <typename Iter, typename ItTraits = std::iterator_traits<Iter>>
    inline constexpr auto make_enumerate_iterator(Iter iter, typename ItTraits::difference_type start_at = 0)
    {
        return enumerate_iterator<Iter>(iter, start_at);
    }

    namespace details
    {
        struct enumerater
        {
            using smite_tag = range_maker_tag;

            template <typename Range, typename ItTraits = std::iterator_traits<typename std::decay_t<Range>::iterator>>
            constexpr auto operator()(Range &&r, typename ItTraits::difference_type start_at = 0) const
            {
                return make_range(make_enumerate_iterator(std::begin(std::forward<Range>(r)), start_at),
                                  make_enumerate_iterator(std::end(std::forward<Range>(r)), -1));
            }
        };
    }

    inline constexpr details::enumerater enumerate;
}

#endif /* !SMITE_ENUMERATE_ITERATOR_HPP */
