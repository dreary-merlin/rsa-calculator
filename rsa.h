#include <cstdint> //библиотека для целых чисел фиксированной длины
#include <random> //содержит генератор случайных чисел
#include <ctime> //для работы с датой и временем
#include <vector>
#include <string>
//#include <utility> //библиотека содержит класс pair

#ifndef RSA_H
#define RSA_H


class rsa
{
public:
    struct Key{
        std::uint64_t e;
        std::uint64_t n;
    };
    Key _key;
    rsa(std::uint64_t _e, std::uint64_t _n);
    bool trial_div(std::uint32_t num);
    std::uint64_t powm(std::uint64_t a, std::uint64_t b, std::uint64_t m);
    std::uint64_t modadd(std::uint64_t a, std::uint64_t b, std::uint64_t mod);
    std::uint64_t modmult(std::uint64_t a, std::uint64_t b, std::uint64_t mod);
    std::uint64_t modpow(std::uint64_t a, std::uint64_t b, std::uint64_t mod);
    bool Miller_Rabin_test(std::uint32_t num);
    std::uint32_t prime_number(std::uint16_t size);
    std::uint64_t Euclidean_algorithm(std::uint64_t a, std::uint64_t b);
    std::int64_t Extended_Euclidean_algorithm(std::uint64_t a, std::uint64_t b, std::int64_t &x, std::int64_t &y);
    std::int64_t inverse(std::uint64_t a, std::uint64_t b);
    std::vector<std::uint64_t> resize(const std::vector<std::uint64_t> &data, std::uint8_t in_size, std::uint8_t out_size);
    std::vector<std::uint8_t> process_bytes(const std::vector<std::uint8_t> &data, Key k, bool encrypt);
    std::string blabla(const std::string str, Key k, bool encrypt);
};

#endif // RSA_H
