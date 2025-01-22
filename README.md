# string-extension
## Attention
TODO: `std::queue` and `std::stack`.

## General
The function `std::string string_ext::to_string(const T&)` returns a string, which is the content of the parameter `T`.

`T` could be:
- std::pair
- std::tuple
- all kinds of STL containers

## Limitation
The types of elements inside of `T` must be streamable.

C++14 or higher is necessary.

## Example
```
int main() {
    std::map<std::string, std::set<int>> mp {{"abc", {1, 2}}, {"def", {3, 4, 5}}};
    std::cout << string_ext::to_string(mp) << std::endl;

    std::array<int, 3> arr {1, 2, 3};
    std::cout << string_ext::to_string(arr) << std::endl;

    std::vector<std::pair<int, int>> vec{{1, 2}, {3, 4}};
    std::cout << string_ext::to_string(vec) << std::endl;

    std::tuple<int, int, int> tp {1, 2, 3};
    std::cout << string_ext::to_string(tp) << std::endl;

    return 0;
}
```
