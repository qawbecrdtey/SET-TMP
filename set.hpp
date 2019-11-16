#ifndef misc_type_set_hpp
#define misc_type_set_hpp

#include <type_traits>

namespace misc {
    template<bool b, typename T, typename U>
    struct IF;
    template<typename T, typename U>
    struct IF<true, T, U> {
        using result = T;
    };
    template<typename T, typename U>
    struct IF<false, T, U> {
        using result = U;
    };

    template<typename T, typename U>
    struct merge_type_set;

    template<typename ...Args>
    struct type_set;
    template<typename T, typename U, typename ...Args>
    struct type_set<T, U, Args...> {
        static constexpr bool is_empty = false;
        static constexpr std::size_t size = 2 + sizeof...(Args);
        template<typename V>
        static constexpr bool in = std::is_same_v<T, V> ? true : type_set<U, Args>::in<V>;
        
        using simplify = typename IF<
            type_set<U, Args...>::in<T>,
            typename merge_type_set<typename type_set<U, Args...>::simplify, typename type_set<T>>::result,
            typename type_set<U, Args...>::simplify
        >::result;

        template<typename V>
        static constexpr bool contains = false;
        template<typename V, typename ...ArgsV>
        static constexpr bool contains<type_set<V, ArgsV...>> = type_set<T, U, Args...>::in<V> ? type_set<T, U, Args...>::contains<type_set<ArgsV...>> : false;
        template<>
        static constexpr bool contains<type_set<>> = true;
    };
    template<typename T, typename U>
    struct type_set<T, U> {
        static constexpr bool is_empty = false;
        static constexpr bool std::size_t size = 2;
        template<typename V>
        static constexpr bool in = std::is_same_v<T, V> || std::is_same_v<U, V> ? true : false;

        using simplify = typename IF<
            std::is_same_v<T, U>,
            typename type_set<T>,
            typename type_set<T, U>
        >::result;

        template<typename V>
        static constexpr bool contains = false;
        template<typename V, typename ...ArgsV>
        static constexpr bool contains<type_set<V, ArgsV...>> = std::is_same_v<T, V> || std::is_same_v<U, V> ? type_set<T, U>::contains<type_set<ArgsV...>> : false;
        template<>
        static constexpr bool contains<type_set<>> = true;
    };
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
        static constexpr bool contains<type_set<V, ArgsV...>> = std::is_same_v<T, V> ? type_set<T>::contains<type_set<ArgsV...>> : false;
        template<>
        static constexpr bool contains<type_set<>> = true;
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
    };
}

#endif