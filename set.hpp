#ifndef misc_type_set_hpp
#define misc_type_set_hpp

#include <cstddef>
#include <type_traits>

namespace misc {
    template<bool b, typename T, typename U>
    struct IF;
    template<typename T, typename U>
    struct IF<true, T, U> { using result = T; };
    template<typename T, typename U>
    struct IF<false, T, U> { using result = U; };

    template<typename ...Args>
    struct type_set;
    template<typename T, typename U, typename ...Args>
    struct type_set<T, U, Args...> {
        static constexpr bool is_empty = false;
        static constexpr std::size_t size = 2 + sizeof...(Args);
        template<typename V>
        static constexpr bool in = std::is_same_v<T, V> || type_set<U, Args...>::template in<V>;

		template<typename V>
		using add_element = typename IF<
			type_set<T, U, Args...>::template in<V>,
			typename type_set<T, U, Args...>,
			typename type_set<T, U, Args..., V>
		>::result;

        using simplify = typename IF<
            type_set<U, Args...>::template in<T>,
            typename type_set<U, Args...>::simplify,
            typename type_set<U, Args...>::simplify::template add_element<T>
        >::result;

        template<typename V>
        static constexpr bool contains = false;
        template<typename V, typename ...ArgsV>
        static constexpr bool contains<type_set<V, ArgsV...>> = type_set<T, U, Args...>::template in<V> && type_set<T, U, Args...>::template contains<type_set<ArgsV...>>;
        template<>
        static constexpr bool contains<type_set<>> = true;
    };/*
    template<typename T, typename U>
    struct type_set<T, U> {
        static constexpr bool is_empty = false;
        static constexpr std::size_t size = 2;
        template<typename V>
        static constexpr bool in = std::is_same_v<T, V> || std::is_same_v<U, V>;

        using simplify = typename IF<
            std::is_same_v<T, U>,
            typename type_set<T>,
            typename type_set<U, T>
        >::result;

        template<typename V>
        static constexpr bool contains = false;
        template<typename V, typename ...ArgsV>
        static constexpr bool contains<type_set<V, ArgsV...>> = (std::is_same_v<T, V> || std::is_same_v<U, V>) && type_set<T, U>::template contains<type_set<ArgsV...>>;
        template<>
        static constexpr bool contains<type_set<>> = true;

        template<typename V>
        using add_element = typename IF<
            std::is_same_v<T, V> || std::is_same_v<U, V>,
            typename type_set<T, U>,
            typename type_set<T, U, V>
        >::result::simplify;
    };*/
    template<typename T>
    struct type_set<T> {
        static constexpr bool is_empty = false;
        static constexpr std::size_t size = 1;
        template<typename V>
        static constexpr bool in = std::is_same_v<T, V>;

        using simplify = typename type_set<T>;

        template<typename V>
        static constexpr bool contains = false;
        template<typename V, typename ...ArgsV>
        static constexpr bool contains<type_set<V, ArgsV...>> = std::is_same_v<T, V> && type_set<T>::template contains<type_set<ArgsV...>>;
        template<>
        static constexpr bool contains<type_set<>> = true;

        template<typename V>
        using add_element = typename IF<
            std::is_same_v<T, V>,
            typename type_set<T>,
            typename type_set<T, V>
        >::result;
    };
    template<>
    struct type_set<> {
        static constexpr bool is_empty = true;
        static constexpr std::size_t size = 0;
        template<typename V>
        static constexpr bool in = false;

        using simplify = typename type_set<>;

        template<typename V>
        static constexpr bool contains = false;
        template<>
        static constexpr bool contains<type_set<>> = true;

        template<typename V>
        using add_element = typename type_set<V>;
    };

    template<typename T>
    struct is_type_set : std::false_type {};
    template<typename ...Args>
    struct is_type_set<type_set<Args...>> : std::true_type {};

    template<typename T, typename U>
    struct merge_type_set {
        static_assert(is_type_set<T>::value, "T is not of type type_set.");
        static_assert(is_type_set<U>::value, "U is not of type type_set.");
    };
    template<typename T, typename U, typename ...Args>
    struct merge_type_set<T, type_set<U, Args...>> {
        static_assert(is_type_set<T>::value, "T is not of type type_set.");

        using result = typename merge_type_set<typename T::template add_element<U>, typename type_set<Args...>>::result;
    };
    template<typename T>
    struct merge_type_set<T, type_set<>> {
        static_assert(is_type_set<T>::value, "T is not of type type_set.");

        using result = typename T::simplify;
    };
}

#endif
