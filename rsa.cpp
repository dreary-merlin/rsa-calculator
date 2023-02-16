#include "rsa.h"

rsa::rsa(std::uint64_t _e, std::uint64_t _n)
{
    _key.e=_e;
    _key.n=_n;
}

bool rsa::trial_div(std::uint32_t num) { // метод пробных делений
    std::uint32_t r = 2;
    while (r < sqrt(num)) {
        if (num%r==0)
            return false;
        else
            r++;
    }
    return true;
}

std::uint64_t rsa::powm(std::uint64_t a, std::uint64_t b, std::uint64_t m) { // быстрое возведение в степень по модулю
    std::uint64_t c = 1;
    while (b) {
        if (b % 2 == 0) {
            b /= 2;
            a = (a*a) % m;
        }
        else {
            b--;
            c = (c*a) % m;
        }
    }
    return c;
}

std::uint64_t rsa::modadd(std::uint64_t a, std::uint64_t b, std::uint64_t mod) {
    if (a >= mod)
        a %= mod;
    if (b >= mod)
        b %= mod;
    a += b;
    if (a >= mod || a < b)
        a -= mod;
    return a;
}

std::uint64_t rsa::modmult(std::uint64_t a, std::uint64_t b, std::uint64_t mod) {
    if (a == 0 || b < mod / a)
        return (static_cast<std::uint64_t>(a*b)) % mod;
    std::uint64_t sum = 0;
    while (b>0) {
        if (b & 1)
            sum = modadd(sum, a, mod);
        a = modadd(a, a, mod);
        b >>= 1;
    }
    return sum;
}

std::uint64_t rsa::modpow(std::uint64_t a, std::uint64_t b, std::uint64_t mod) {

    std::uint64_t product, pseq;
    product = 1;
    pseq = a%mod;
    while (b>0) {
        if (b & 1)
            product = modmult(product, pseq, mod);
        pseq = modmult(pseq, pseq, mod);
        b >>= 1;
    }
    return product;
}

bool rsa::Miller_Rabin_test(std::uint32_t num) { // тест простоты Миллера-Рабина
    std::uint32_t t = num - 1;
    std::uint16_t s = 0;
    while (t > 0) {
        if (t % 2 == 0) {
            s++;
            t = t / 2;
        }
        else
            break;
    }
    std::mt19937 mersenne(static_cast<std::uint32_t>(time(nullptr)));
    std::uniform_int_distribution<std::uint32_t> uid(2, num - 2);
    std::uint16_t k = static_cast<std::uint16_t>(round(log(num) / log(2)));
    for (std::uint16_t i = 0; i < k; i++) {
        std::uint32_t a = uid(mersenne);
        std::uint64_t x = powm(a, t, num);
        if (x == 1 || x == num - 1)
            continue;
        for (std::uint16_t j = 0; j <= s - 1; j++) {
            x = powm(x, 2, num);
            if (x == 1)
                return false;
            if (x == num - 1)
                break;
        }
        if (x != num - 1)
            return false;
    }
    return true;
}


std::uint32_t rsa::prime_number(std::uint16_t size){ // генерация простого числа на основе теста Поклингтона
    std::mt19937 mersenne(static_cast<std::uint32_t>(time(nullptr)));
    std::uniform_int_distribution<uint32_t> uid(size, 4 * size + 2);
    std::uint32_t r, n;
    while (true) {
        r = uid(mersenne);
        if (r % 2 == 0) {
            n = size*r + 1;
            if (trial_div(n))
                if (Miller_Rabin_test(n))
                    return n;
        }
        else
            continue;
    }
}

std::uint64_t rsa::Euclidean_algorithm(std::uint64_t a, std::uint64_t b) {
  if (a % b == 0)
    return b;
  if (b % a == 0)
    return a;

  if (a > b)
    return Euclidean_algorithm(a%b, b);
  return Euclidean_algorithm(a, b%a);
}

std::int64_t rsa::Extended_Euclidean_algorithm(std::uint64_t a, std::uint64_t b, std::int64_t &x, std::int64_t &y) { // расширенный алгоритм Евклида
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    std::int64_t x1, y1;
    std::uint64_t gcd = Extended_Euclidean_algorithm(b, a%b, x1, y1);
    x = y1;
    y = x1 - (a / b)*y1;
    return gcd;
}

std::int64_t rsa::inverse(std::uint64_t a, std::uint64_t b) { // мультипликативное обратное a по модулю b
    int64_t x, y, gcd = Extended_Euclidean_algorithm(a, b, x, y);
    //x = (x % b + b) % b;
    if (x < 0)
        x = x + b;
    if (gcd == 1)
        return x;
    return -1;
}

std::string rsa::crypt(const std::string str, Key k, bool encrypt) {
    std::uint16_t size = static_cast<uint16_t>(ceil(log10(k.n + 0.5)) - encrypt); // k - количество цифр в блоке
    std::vector<std::uint64_t> vec;
    size_t i = 0;
    while (i < str.length()) {
        std::string block = "";
        for (size_t j = i; j < (size+i); j++) {
            block += str[j];
            if (j == str.length())
                break;
        }
        uint64_t num = stoull(block);
        if (i != 0 && block[0] == '0' && encrypt == true) {
            block.pop_back();
            char c = vec[vec.size() - 1] % 10+'0';
            if (c == '0') {
                block.insert(block.begin(), c);
                c = vec[vec.size() - 1] / 10 % 10 + '0';
                if ((size + i) <= str.length()) {
                    block.pop_back();
                    i--;
                }
                vec[vec.size() - 1] = vec[vec.size() - 1] / 10;
            }
            block.insert(block.begin(), c);
            vec[vec.size() - 1] = vec[vec.size() - 1] / 10;
            num = stoull(block);
            i--;
        }
        if (num < k.n) {
            vec.push_back(num);
            i = i + size;
        }
        else {
            num = num / 10;
            vec.push_back(num);
            i = i + size - 1;
        }
    }
    std::string crypt="";
    for (size_t j = 0; j < vec.size(); j++) {
        if (encrypt == true && (ceil(log10(modpow(vec[j], k.e, k.n) + 0.5))) <= size){
            for (uint16_t i = 0; i <= (size - ceil(log10(modpow(vec[j], k.e, k.n) + 0.5))); i++)
                crypt += "0";
        }
        crypt+=std::to_string(modpow(vec[j], k.e, k.n));
    }
    return crypt;
}
