#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <forward_list>
#include <list>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <queue>
#include <stack>

namespace string_ext {
constexpr const static char ELEMENT_SEPERATOR = '|';
constexpr const static char INNER_SEPERATOR   = ',';

namespace is_streamable_impl {
template <typename T, typename Enable = void>
struct is_streamable : public std::false_type {};

template <typename T>
struct is_streamable<
    T,
    std::enable_if_t<std::is_same<decltype(std::declval<std::ostream&>() << std::declval<T>()),
                                  std::ostream&>::value>> : public std::true_type {};
} // namespace is_streamable_impl

template <typename T>
struct is_streamable
    : public std::integral_constant<bool, is_streamable_impl::is_streamable<T>::value> {};

template <typename>
struct is_pair : std::false_type {};

template <typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type {};

namespace is_stl_container_impl {
template <typename T>
struct is_stl_container : std::false_type {};
template <typename T, std::size_t N>
struct is_stl_container<std::array<T, N>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::vector<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::deque<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::list<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::forward_list<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::set<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::multiset<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::map<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::multimap<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::unordered_set<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::unordered_multiset<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::unordered_map<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::unordered_multimap<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::stack<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::queue<Args...>> : std::true_type {};
template <typename... Args>
struct is_stl_container<std::priority_queue<Args...>> : std::true_type {};
} // namespace is_stl_container_impl

template <typename T>
struct is_stl_container {
    static constexpr bool const value =
        is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
};

/**
 * @brief T is streamable.
 */
template <typename T, std::enable_if_t<is_streamable<T>::value>* = nullptr>
std::string to_string(const T& t) {
    std::stringstream res;
    res << t;
    return res.str();
}

/**
 * @brief std::tuple end case
 */
template <size_t I = 0, typename... Ts, std::enable_if_t<I == sizeof...(Ts)>* = nullptr>
char to_string(const std::tuple<Ts...>& tup) {
    return ELEMENT_SEPERATOR;
}

/**
 * @brief std::tuple
 */
template <size_t I = 0,
          typename... Ts,
          std::enable_if_t<
              I<sizeof...(Ts)>* = nullptr,
              std::enable_if_t<is_streamable<typename std::tuple_element<I, std::tuple<Ts...>>::type>::value>* = nullptr>
std::string to_string(const std::tuple<Ts...>& tup) {
    std::stringstream res;
    if (I + 1 == sizeof...(Ts)) {
        res << std::get<I>(tup) << to_string<I + 1>(tup);
    } else {
        res << std::get<I>(tup) << INNER_SEPERATOR << to_string<I + 1>(tup);
    }
    return res.str();
}

/**
 * @brief C is STL container, which contains streamable elements.
 */
template <typename C,
          std::enable_if_t<is_stl_container<C>::value>*                   = nullptr,
          std::enable_if_t<is_streamable<typename C::value_type>::value>* = nullptr>
std::string to_string(const C& container) {
    std::stringstream res;
    for (const auto& element : container) {
        res << element << ELEMENT_SEPERATOR;
    }
    return res.str();
}

/**
 * @brief C is STL map-like container.
 */
template <typename C,
          std::enable_if_t<is_stl_container<C>::value>*             = nullptr,
          std::enable_if_t<is_pair<typename C::value_type>::value>* = nullptr>
std::string to_string(const C& mp) {
    std::stringstream res;
    for (const auto& element : mp) {
        res << to_string(element.first) << INNER_SEPERATOR << to_string(element.second)
            << ELEMENT_SEPERATOR;
    }
    return res.str();
}

/**
 * @brief C is nested STL container.
 */
template <typename C, std::enable_if_t<is_stl_container<typename C::value_type>::value>* = nullptr>
std::string to_string(const C& container) {
    std::stringstream res;
    for (const auto& element : container) {
        res << to_string(element) << ELEMENT_SEPERATOR;
    }
    return res.str();
}
}
