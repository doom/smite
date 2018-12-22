/*
** Created by doom on 17/12/18.
*/

#ifndef SMITE_ZIP_ITERATOR_HPP
#define SMITE_ZIP_ITERATOR_HPP

#include <iterator>
#include <smite/details/fake_ptr.hpp>
#include <smite/details/compressed_pair.hpp>

namespace smite
{
    namespace details
    {
        std::input_iterator_tag simplest_iterator_category(std::input_iterator_tag,
                                                           std::input_iterator_tag);

        std::output_iterator_tag simplest_iterator_category(std::output_iterator_tag,
                                                            std::output_iterator_tag);

        std::forward_iterator_tag simplest_iterator_category(std::forward_iterator_tag,
                                                             std::forward_iterator_tag);

        std::bidirectional_iterator_tag simplest_iterator_category(std::bidirectional_iterator_tag,
                                                                   std::bidirectional_iterator_tag);

        std::random_access_iterator_tag simplest_iterator_category(std::random_access_iterator_tag,
                                                                   std::random_access_iterator_tag);
    }

    template <typename Iter1, typename Iter2>
    class zip_iterator :
        private details::compressed_pair<Iter1, Iter2>
    {
    public:
        using first_iterator_type = Iter1;
        using second_iterator_type = Iter2;

    private:
        using base_type = details::compressed_pair<Iter1, Iter2>;
        using first_iterator_traits = std::iterator_traits<Iter1>;
        using second_iterator_traits = std::iterator_traits<Iter2>;

    public:
        using difference_type = typename first_iterator_traits::difference_type;
        using value_type = typename first_iterator_traits::value_type;
        using reference = std::pair<
            typename first_iterator_traits::reference,
            typename second_iterator_traits::reference
        >;
        using pointer = details::fake_ptr<reference>;
        using iterator_category = decltype(details::simplest_iterator_category(
            typename first_iterator_traits::iterator_category{},
            typename second_iterator_traits::iterator_category{}
        ));

        constexpr zip_iterator(Iter1 iter, Iter2 iter2) : base_type(iter, iter2)
        {
        }

        constexpr zip_iterator(const zip_iterator &) = default;

        constexpr zip_iterator(zip_iterator &&) = default;

        constexpr zip_iterator &operator=(const zip_iterator &) = default;

        constexpr zip_iterator &operator=(zip_iterator &&) = default;

        constexpr pointer operator->() const
        {
            return pointer{**this};
        }

        constexpr reference operator*() const
        {
            return reference{*first_base(), *second_base()};
        }

        constexpr zip_iterator &operator++()
        {
            ++first_base();
            ++second_base();
            return *this;
        }

        constexpr const zip_iterator operator++(int)
        {
            auto tmp = *this;

            ++*this;
            return tmp;
        }

        constexpr zip_iterator &operator--()
        {
            --first_base();
            --second_base();
            return *this;
        }

        constexpr const zip_iterator operator--(int)
        {
            auto tmp = *this;

            --*this;
            return tmp;
        }

        constexpr zip_iterator operator+(difference_type n)
        {
            return zip_iterator(first_base() + n, second_base() + n);
        }

        constexpr zip_iterator &operator+=(difference_type n)
        {
            first_base() += n;
            second_base() += n;
            return *this;
        }

        constexpr zip_iterator operator-(difference_type n)
        {
            return zip_iterator(first_base() - n, second_base() - n);
        }

        constexpr difference_type operator-(zip_iterator other)
        {
            return std::distance(other.first_base(), first_base());
        }

        constexpr zip_iterator &operator-=(difference_type n)
        {
            first_base() -= n;
            second_base() -= n;
            return *this;
        }

        constexpr first_iterator_type &first_base() noexcept
        {
            return base_type::first();
        }

        constexpr const first_iterator_type &first_base() const noexcept
        {
            return base_type::first();
        }

        constexpr second_iterator_type &second_base() noexcept
        {
            return base_type::second();
        }

        constexpr const second_iterator_type &second_base() const noexcept
        {
            return base_type::second();
        }
    };

    template <typename Iter1, typename Iter2>
    inline constexpr bool operator==(const zip_iterator<Iter1, Iter2> &lhs, const zip_iterator<Iter1, Iter2> &rhs)
    {
        return lhs.first_base() == rhs.first_base() && lhs.second_base() == rhs.second_base();
    }

    template <typename Iter1, typename Iter2>
    inline constexpr bool operator!=(const zip_iterator<Iter1, Iter2> &lhs, const zip_iterator<Iter1, Iter2> &rhs)
    {
        return !(rhs == lhs);
    }

    template <typename Iter1, typename Iter2>
    inline constexpr bool operator<(const zip_iterator<Iter1, Iter2> &lhs, const zip_iterator<Iter1, Iter2> &rhs)
    {
        return lhs.first_base() < rhs.first_base() && lhs.second_base() < rhs.second_base();
    }

    template <typename Iter1, typename Iter2>
    inline constexpr bool operator>(const zip_iterator<Iter1, Iter2> &lhs, const zip_iterator<Iter1, Iter2> &rhs)
    {
        return rhs < lhs;
    }

    template <typename Iter1, typename Iter2>
    inline constexpr bool operator<=(const zip_iterator<Iter1, Iter2> &lhs, const zip_iterator<Iter1, Iter2> &rhs)
    {
        return !(rhs < lhs);
    }

    template <typename Iter1, typename Iter2>
    inline constexpr bool operator>=(const zip_iterator<Iter1, Iter2> &lhs, const zip_iterator<Iter1, Iter2> &rhs)
    {
        return !(lhs < rhs);
    }

    template <typename Iter1, typename Iter2>
    inline constexpr auto make_zip_iterator(Iter1 iter, Iter2 &&iter2)
    {
        return zip_iterator<Iter1, Iter2>(iter, iter2);
    }

    namespace details
    {
        struct zipper
        {
            using smite_tag = range_maker_tag;

            template <typename Range1, typename Range2>
            constexpr auto operator()(Range1 &&r1, Range2 &&r2) const
            {
                return make_range(
                    make_zip_iterator(std::begin(std::forward<Range1>(r1)), std::begin(std::forward<Range2>(r2))),
                    make_zip_iterator(std::end(std::forward<Range1>(r1)), std::end(std::forward<Range2>(r2)))
                );
            }
        };
    }

    inline constexpr details::zipper zip;
}

#endif /* !SMITE_ZIP_ITERATOR_HPP */
