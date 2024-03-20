# Attacks against Stream Ciphers

> A stream cipher tries to approximate a Shannon's one-time pad algorithm.

Basic construction of a stream cipher:
```
pt stream -----> XOR -----> ct stream
                  ^
                  |
key ---> PRNG ---> key stream
```

In old stream ciphers, the key stream was generated only from a key. In most newer stream ciphers, the key stream is generated from a key and a nonce.

The fundamental things to respect are:

- Never reuse the same nonce.
- Change the key quite often.

## Basic Attack against Stream Ciphers - Key Reuse

```
    pt_secret XOR key stream = ct1
    pt_known XOR key stream = ct2

    key stream = ct2 XOR pt_known
    pt_secret = ct1 XOR key stream !!!!!
```

## Problems with key reuse

The XOR is a very good in terms of cryptographic properties, but in the case of stream ciphers has some problems. First of all the properties of the plaintext are preserved in the ciphertext, this open the door to some mathematical deductions.

For example, if the plaintext is composed by only ASCII character we can perform a frequency analysis on the ciphertext to understand the plaintext and the key.

### ChaCha20

Attacking ChaCha20 can be quite hard... In many challenges that uses ChaCha20 from well known libraries, we have to attack how the cipher is used.

> **Remember**: The core of ChaCha20 is a mechanism to generate the key stream from the key and the nonce, the encryption operation is simply an XOR.
