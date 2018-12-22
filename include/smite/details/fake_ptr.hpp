/*
** Created by doom on 14/12/18.
*/

#ifndef SMITE_FAKE_PTR_HPP
#define SMITE_FAKE_PTR_HPP

namespace smite::details
{
    template <typename T>
    struct fake_ptr
    {
        constexpr decltype(auto) operator->() const
        {
            return _t;
        }

        T _t;
    };
}

#endif /* !SMITE_FAKE_PTR_HPP */
