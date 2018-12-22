/*
** Created by doom on 14/12/18.
*/

#ifndef SMITE_TRANSFORM_ITERATOR_HPP
#define SMITE_TRANSFORM_ITERATOR_HPP

#include <utility>
#include <iterator>
#include <smite/range.hpp>
#include <smite/details/fake_ptr.hpp>
#include <smite/details/compressed_pair.hpp>

namespace smite
{
    template <typename Iter, typename Transformer>
    class transform_iterator :
        private details::compressed_pair<Iter, Transformer>
    {
    public:
        using iterator_type = Iter;
        using transformer_type = Transformer;

    private:
        using base_type = details::compressed_pair<Iter, Transformer>;
        using return_type = std::invoke_result_t<Transformer, decltype(*std::declval<Iter>())>;
        using iterator_traits = std::iterator_traits<Iter>;

    public:
        using difference_type = typename iterator_traits::difference_type;
        using value_type = typename iterator_traits::value_type;
        using reference = return_type;
        using pointer = details::fake_ptr<reference>;
        using iterator_category = typename iterator_traits::iterator_category;

        // TODO: universal refs
        constexpr transform_iterator(Iter iter, Transformer transformer) : base_type(iter, transformer)
        {
        }

        constexpr transform_iterator(const transform_iterator &) = default;

        constexpr transform_iterator(transform_iterator &&) = default;

        constexpr transform_iterator &operator=(const transform_iterator &) = default;

        constexpr transform_iterator &operator=(transform_iterator &&) = default;

        constexpr pointer operator->() const
        {
            return pointer{**this};
        }

        constexpr reference operator*() const
        {
            return transformer()(*base());
        }

        constexpr transform_iterator &operator++()
        {
            ++base();
            return *this;
        }

        constexpr const transform_iterator operator++(int)
        {
            auto tmp = *this;

            ++*this;
            return tmp;
        }

        constexpr transform_iterator &operator--()
        {
            --base();
            return *this;
        }

        constexpr const transform_iterator operator--(int)
        {
            auto tmp = *this;

            --*this;
            return tmp;
        }

        constexpr transform_iterator operator+(difference_type n)
        {
            return transform_iterator(base() + n, transformer());
        }

        constexpr transform_iterator &operator+=(difference_type n)
        {
            base() += n;
            return *this;
        }

        constexpr transform_iterator operator-(difference_type n)
        {
            return transform_iterator(base() - n, transformer());
        }

        constexpr difference_type operator-(transform_iterator other)
        {
            return std::distance(other.base(), base());
        }

        constexpr transform_iterator &operator-=(difference_type n)
        {
            base() -= n;
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

        constexpr transformer_type &transformer() noexcept
        {
            return base_type::second();
        }

        constexpr const transformer_type &transformer() const noexcept
        {
            return base_type::second();
        }
    };

    template <typename Iter, typename Transformer>
    inline constexpr bool operator==(const transform_iterator<Iter, Transformer> &lhs,
                                     const transform_iterator<Iter, Transformer> &rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <typename Iter, typename Transformer>
    inline constexpr bool operator!=(const transform_iterator<Iter, Transformer> &lhs,
                                     const transform_iterator<Iter, Transformer> &rhs)
    {
        return !(rhs == lhs);
    }

    template <typename Iter, typename Transformer>
    inline constexpr bool operator<(const transform_iterator<Iter, Transformer> &lhs,
                                    const transform_iterator<Iter, Transformer> &rhs)
    {
        return lhs.base() < rhs.base();
    }

    template <typename Iter, typename Transformer>
    inline constexpr bool operator>(const transform_iterator<Iter, Transformer> &lhs,
                                    const transform_iterator<Iter, Transformer> &rhs)
    {
        return rhs < lhs;
    }

    template <typename Iter, typename Transformer>
    inline constexpr bool operator<=(const transform_iterator<Iter, Transformer> &lhs,
                                     const transform_iterator<Iter, Transformer> &rhs)
    {
        return !(rhs < lhs);
    }

    template <typename Iter, typename Transformer>
    inline constexpr bool operator>=(const transform_iterator<Iter, Transformer> &lhs,
                                     const transform_iterator<Iter, Transformer> &rhs)
    {
        return !(lhs < rhs);
    }

    template <typename Iter, typename Transformer>
    inline constexpr auto make_transform_iterator(Iter iter, Transformer &&transformer)
    {
        using decayed_iter = std::decay_t<Iter>;
        using decayed_transformer = std::decay_t<Transformer>;
        return transform_iterator<decayed_iter, decayed_transformer>(iter, std::forward<Transformer>(transformer));
    }

    template <typename Container, typename Transformer>
    inline constexpr auto transform(Container &&container, Transformer &&transformer)
    {
        return make_range(make_transform_iterator(std::begin(std::forward<Container>(container)), transformer),
                          make_transform_iterator(std::end(std::forward<Container>(container)), transformer));
    }

    namespace details
    {
        template <typename Transformer>
        struct transform_maker
        {
            using smite_tag = range_maker_tag;

            template <typename Range>
            constexpr auto operator()(Range &&rng) const
            {
                return transform(std::forward<Range>(rng), _transformer);
            }

            Transformer _transformer;
        };
    }

    template <typename Transformer>
    inline constexpr auto make_transform(Transformer &&transformer)
    {
        return details::transform_maker<std::decay_t<Transformer>>{std::forward<Transformer>(transformer)};
    }
}

#endif /* !SMITE_TRANSFORM_ITERATOR_HPP */
