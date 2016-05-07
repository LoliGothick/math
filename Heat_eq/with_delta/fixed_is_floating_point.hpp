#ifndef FIXED_IS_FLOATING_POINT_HPP
#define FIXED_IS_FLOATING_POINT_HPP
#include <boost/multiprecision/cpp_dec_float.hpp>

namespace concept{
    namespace mp = boost::multiprecision;

    namespace detail{
        template < typename T >
        struct fixed_is_floating_point
        {
            static constexpr bool value = std::is_floating_point<T>::value;
        };

        template < >
        struct fixed_is_floating_point<mp::cpp_dec_float_100>
        {
            static constexpr bool value = true;
        };
    }
    using detail::fixed_is_floating_point;
    template < typename T >
    constexpr bool fixed_is_floating_point_v = fixed_is_floating_point<T>::value;
}

#endif // !FIXED_IS_FLOATING_POINT_HPP