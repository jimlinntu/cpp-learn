/*
$ g++ -std=c++17 -o prog try_decay.cpp
$ ./prog
*/
#include <iostream>
#include <cassert>

// Ref: https://stackoverflow.com/questions/7797839/why-does-the-array-decay-to-a-pointer-in-a-template-function
template <class T1>
T1 make_str1(T1 x){
    if (std::is_same_v<decltype(x), char*>){
        std::cout << "make_str1 T1 = char *\n";
    } else if(std::is_same_v<decltype(x), std::string>){
        std::cout << "make_str1 T1 = std::string. Note that this is inefficient because of passing by value. \n";
    }
    return x;
}

template <class T1>
T1 make_str2(T1& x){
    if(std::is_same_v<decltype(x), std::string&>){
        std::cout << "make_str2 T1 = std::string&.\n";
    }
    return x;
}

template <class T1>
typename std::decay<T1>::type make_str3(T1& x){
    if(std::is_same_v<decltype(x), char(&)[6]>){
        std::cout << "make_str3 T1 = char(&)[6]. Note that we now return char* instead of char[6]!\n";
    }else if(std::is_same_v<decltype(x), std::string&>){
        std::cout << "make_str3 T1 = std::string&.\n";
    }
    return x;
}


int main(){
    std::cout << std::boolalpha;
    char raw_str[] = "hello";
    char bad_raw_str[2] = {'a', 'b'};

    // "hello\0"
    assert(sizeof(raw_str) == 6);

    std::string out;
    out = make_str1(raw_str);
    std::string s = "abc";
    out = make_str1(s);

    // Do not compile because char[6] cannot be the return type!

    /*
    try_decay.cpp:48:5: error: no matching function for call to 'make_str2'
        make_str2(raw_str);
        ^~~~~~~~~
    try_decay.cpp:16:4: note: candidate template ignored: substitution failure [with T1 = char[6]]: function cannot return array type 'char[6]'
    T1 make_str2(T1& x){
    ~~ ^
    1 error generated.
    */

    /* make_str2(raw_str); */

    out = make_str2(s);

    // Note that the conversion there is a constructor for
    // std::string(char*) so this works!
    // Explanation:
    // raw_str will be returned as char*.
    // And later std::string(char*) will be called.
    out = make_str3(raw_str);
    out = make_str3(bad_raw_str);
    assert(out == "abhello");
    std::cout << "[!] memory vulnerability!! out: " << out << std::endl;
    out = make_str3(s);

    int a[5] = {0, 1, 2, 3, 4};
    // std::pair<std::vector<int>, int> c = std::make_pair(a, 10);
    // Error message:
    /*
try_decay.cpp:64:38: error: no viable conversion from 'pair<typename __unwrap_ref_decay<int (&)[5]>::type, [...]>' to 'pair<std::vector<int>, [...]>'
    std::pair<std::vector<int>, int> c = std::make_pair(a, 10);
                                     ^   ~~~~~~~~~~~~~~~~~~~~~
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__utility/pair.h:74:5: note: candidate constructor not viable: no known conversion from 'pair<typename __unwrap_ref_decay<int (&)[5]>::type, typename __unwrap_ref_decay<int>::type>' (aka 'pair<int *, int>') to 'const pair<vector<int>, int> &' for 1st argument
    pair(pair const&) = default;
    ^
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__utility/pair.h:75:5: note: candidate constructor not viable: no known conversion from 'pair<typename __unwrap_ref_decay<int (&)[5]>::type, typename __unwrap_ref_decay<int>::type>' (aka 'pair<int *, int>') to 'pair<vector<int>, int> &&' for 1st argument
    pair(pair&&) = default;
    ^
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__utility/pair.h:248:5: note: candidate template ignored: requirement '_CheckArgs::__enable_implicit()' was not satisfied [with _U1 = int *, _U2 = int]
    pair(pair<_U1, _U2> const& __p)
    ^
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__utility/pair.h:266:5: note: candidate template ignored: requirement '_CheckArgs::__enable_implicit()' was not satisfied [with _U1 = int *, _U2 = int]
    pair(pair<_U1, _U2>&& __p)
    ^
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__utility/pair.h:295:5: note: candidate template ignored: requirement '_CheckTLC<pair<int *, int>>::__enable_implicit()' was not satisfied [with _Tuple = std::pair<int *, int>]
    pair(_Tuple&& __p)
    ^
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__utility/pair.h:239:14: note: explicit constructor is not a candidate
    explicit pair(pair<_U1, _U2> const& __p)
             ^
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__utility/pair.h:257:14: note: explicit constructor is not a candidate
    explicit pair(pair<_U1, _U2>&&__p)
             ^
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__utility/pair.h:287:14: note: explicit constructor is not a candidate
    explicit pair(_Tuple&& __p)
             ^
1 error generated.
     */
    return 0;
}
