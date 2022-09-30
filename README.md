# rsa-calculator
Description of the application algorithm:
1. The user selects a prime number that defines a range of values to create a pseudo prime number, which is then checked for primeness by tests.
2. Generation of cryptographic parameters of the RSA algorithm:

    a. The primes p and q are generated based on the Pocklington test using the Miller-Rabin primality test and trial divisions.
    
    b. The open exponent e is one of the Fermat primes (17, 257, 65537) depending on the value of the Euler function φ(n).
    
    c. The closed exponent d is found using the extended Euclid algorithm, which is based on finding the greatest common divisor of two numbers.
3. Entering a message by the user or loading text from a file.
4. Translation of each character of the message into an ASCII code.
5. Dividing the data into blocks, encrypting each block using a fast exponentiation algorithm, padding with zeros and concatenating the data.
6. Splitting the data into blocks, decrypting each block using the fast exponentiation algorithm, padding with zeros and concatenating the data.
7. Dividing the cryptogram into numbers from 0 to 255 and converting them from ASCII codes to characters.

In this implementation, the prime numbers p and q can be up to 32 bits long, n, e, d up to 64 bits, that is, the public/private key can be up to 128 bits long. Encryption / decryption is carried out only using keys up to 128 bits.

The developed application performs encryption/decryption operations with messages consisting of Arabic numerals, punctuation marks, Russian and English characters.
