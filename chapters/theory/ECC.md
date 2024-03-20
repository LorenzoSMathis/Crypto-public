# Elliptic Curve 

> ***Motivation*** to Elliptic Curves: we want to find a public key cryptosystem with smaller operands (smaller keys).

## Elliptic Curve Definition

The elliptic curve over $\mathbb{Z}_p$ $(\text{or }\mathbb{GF(p)})$, where $p > 3$ is a prime number, is the set of pairs (points):

$(x,y) \in \mathbb{Z}_p\ st\ y^2\equiv x^3+ax+b \mod p$

Together wit an imaginary point $\mathcal{O}$, called **point at infinity**, where $a,b \in \mathbb{Z}_p$.

> To have a good elliptic curve, we need to have a non-singular curve. An additional condition is: $4a^3+27b^2 \neq 0 \mod p$.

### Example

$E: y^2 \equiv x^3 - 3x + 3$ over $\mathbb{R}$

> ***Important***: this example is not over $\mathbb{Z}_p$, but over $\mathbb{R}$ (we have not the modulo operation).

![Elliptic Curve](./assets/paar/ECexample.png)

<br>

We note that the curve is symmetric with respect to the x-axis, this means that the solutions to the equation $y^2 = x^3 - 3x + 3 are two pairs of points $(x,y)$ and $(x,-y)$.

$\ \ \ \ \ y = \pm \sqrt{x^3 - 3x + 3}$

## Weierstrass Form

The general form of the elliptic curve is the Weierstrass form:

$f(x,y) = y^2 - x^3 - ax - b = 0 \mod p$

## Compressed Form of a Point

The compressed form of a point $P$ over a curve $E$ is the $x$ coordinate, this means that two point over $E$ are given: $P = (x,y)$ and $P' = (x,-y)$.

## Elliptic Curve Group

To define the group, we need:

1. A set of elements: the points of the curve.

2. A binary operation that fulfills the group law.

### Group Operation: Point Addition

We need to distinguish between two cases:

1. ***Point addition***: $P+Q,\ P \neq Q$.

2. ***Point doubling***: $P+Q = 2P,\ P = Q$.

#### Point Addition

![Point Addition](./assets/paar/ECadd.png)

From a geometric point of view, the sum of two distinct points $P$ and $Q$ over the elliptic curve $E$ is the reflection of the third intersection point of the line through $P$ and $Q$ with the curve $E$. So:

1. Draw the line through $P$ and $Q$.

2. Find the third intersection point with the curve $E$.

3. Reflect the point with respect to the x-axis.

The mathematical expression of the point addition is:

Let $P = (x_1,y_1)$ and $Q = (x_2,y_2)$ be two points on the curve $E$. The sum $R = P+Q = (x_3,y_3)$ is given by:

$x_3 = s^2 - x_1 - x_2 \mod p$

$y_3 = s(x_1 - x_3) - y_1 \mod p$

Where $s = \frac{y_2 - y_1}{x_2 - x_1} \mod p$.

> ***Note***: the $s$ value is the slope of the line through $P$ and $Q$.

> ***Note***: in the computation of the $s$ value, the division is done in $\mathbb{Z}_p$, so it is the `modular inverse` of the denominator.

#### Point Doubling

![Point Doubling](./assets/paar/ECdub.png)

The point doubling is the limit case where $P = Q$. If we consider the line $L$ trough two point $P$ and $Q$ and we take the limit for $P \rightarrow Q$, that line is the tangent to the curve $E$ in the point $P$. So, the point doubling is the reflection of the second intersection point of the tangent line with the curve $E$.

1. Draw the tangent line to the curve $E$ in the point $P$.

2. Find the second intersection point with the curve $E$.

3. Reflect the point with respect to the x-axis.

The mathematical expression of the point doubling is:

Let $P = (x_1,y_1)$ be a point on the curve $E$. The doubling $R = 2P = (x_3,y_3)$ is given by:

$x_3 = s^2 - 2x_1 \mod p$

$y_3 = s(x_1 - x_3) - y_1 \mod p$

Where $s = \frac{3x_1^2 + a}{2y_1} \mod p$.

> ***Note***: in the computation of the $s$ value, the division is done in $\mathbb{Z}_p$, so it is the `modular inverse` of the denominator.

### Group Law Check

The group law is fulfilled if the following properties are satisfied:

1. **Closure**: $P+Q \in E$.

2. **Associativity**: $(P+Q)+R = P+(Q+R)$.

3. **Neutral Element**.

4. **Inverse Element**.

5. **Commutativity**: $P+Q = Q+P$.

Let analyze the properties:

1. **Closure**: with respect to the geometric interpretation, we are considering the reflection of a point of the curve with respect to the x-axis, so the result is a point of the curve. We can say that ***the closure is satisfied***.

2. **Associativity**: ***the associativity is satisfied***, with some algebraic manipulation, we can prove that the associativity is satisfied.

5. **Commutativity**: ***the commutativity is satisfied*** by definition of the point addition. The line through $P$ and $Q$ is the same as the line through $Q$ and $P$, so the result is the same.

Now comes the dirty part, the neutral element does not exists on the curve, so we define it artificially as the point at infinity $\mathcal{O}$.

#### Neutral Element - Point at Infinity

We define the point at infinity $\mathcal{O}$ such that: $\forall P \in E,\ P+\mathcal{O} = P$.

![Neutral Element](./assets/paar/ECptinf.png)

Geometrically, the point at infinity is an imaginary point for $y \rightarrow \infty$. No mathematical expression is given for the point at infinity, but we can say that the point at infinity is the neutral element of the group.

#### Inverse Element

The inverse element of a point $P = (x,y)$ is the point such that $P+(-P) = \mathcal{O}$. 

![Inverse Element](./assets/paar/ECinv.png)

The inverse of a point $P = (x,y)$ is $-P = (x,-y)$.

It is important to understand that the **"minus"** sign in front of the point $P$ is the inverse operation with respect to the group operation, not the arithmetic operation. On the contrary, the minus sign in front of the $y$ coordinate is the arithmetic operation.

#### Point doubling example

$E: y^2 \equiv x^3 + 2x + 2 \mod 17$

$P = (5,1)$

$s = \frac{3 \cdot 5^2 + 2}{2 \cdot 1} \mod 17 = (3\cdot 25 + 2) \cdot 2^{-1} \mod 17 = 9 \cdot 2^{-1} \mod 17 = 9 \cdot 9 \mod 17 = 13 \mod 17$

$x_3 = s^2 - 2 \cdot x_1 \mod p = 13^2 - 2 \cdot 5 \mod 17 = 159 \mod 17 = 6 \mod 17$

$y_3 = s \cdot (x_1 - x_3) - y_1 \mod p = 13 \cdot (5-6) - 1 \mod 17 = -14 \mod 17 = 3 \mod 17$

So, $2P = (6,3)$.

#### How to check the results?

We can check the results by putting the points in the curve equation:

$y^2 \equiv x^3 + 2x + 2 \mod 17|_{(6,3)}$

$3^2 \stackrel{?}{\equiv} 6^3 + 2 \cdot 6 + 2 \mod 17$

$9 \stackrel{?}{\equiv} 230 \mod 17$

$9 \equiv 9 \mod 17$ 

***True***.

#### Theorem

> The points on the elliptic curve, including the point at infinity $\mathcal{O}$, have cyclic subgroups. Under certain conditions, all points on the elliptic curve form a cyclic group.

#### Hasse's Theorem - Group Order

The number of points on the elliptic curve $E$ over $\mathbb{Z}_p$ bounded by:

$ p + 1 - 2\sqrt{p} \leq \#E(\mathbb{Z}_p) \leq p + 1 + 2\sqrt{p}$

So, we can say: $\#E \approx p$.

This is useful to know because, to perform some attacks, we need to know the exact number of point in $E$. Find this number is an hard problem if the curve is well chosen.

## Elliptic Curve Discrete Logarithm Problem

The elliptic curved discrete logarithm problem (ECDLP) is:

Given an elliptic curve $E$. We consider a primitive element $P$ and an element $T$ such that $P, T \in E$. The ECDLP is to fine the integer $d$, where $1 \leq d \leq \#E$ such that $T = dP = P + P + \cdots + P$.

> ***Note***: considering a cryptosystem based on the ECDLPL.
>
> $K_{PR} = d$, where $d$ is an integer.
>
> $K_{Pub} = T$, where $T$ is a point on the curve.
>
> In general, the DLP and all its variants have as private key an integer and as public key something weird defined by the group.

## How hard is the ECDLP?

If the elliptic curve is well chosen, the best algorithm to solve the ECDLP is the **generic algorithm**. The generic algorithm has a complexity of $O(\sqrt{\#E})$ (**square-root attacks**).

This means that to achieve a security level of 80 bits, we need keys of the order of:

- **RSA**: 1024 bits.

- **DLP over $\mathbb{Z}_p^*$**: 1024 bits.

- **ECDLP**: 160 bits.

# Elliptic Curve Cryptography

## Generic ECC Cryptosystem

The domain parameters of an ECC cryptosystem are the sextuple $(p,a,b,G,n,h)$, where:

- $E: y^2 \equiv x^3 + ax + b \mod p$ is the elliptic curve.

- $G = (x_G, y_G)$ is a base point on the curve $E$ (generator or primitive element).

- $n$ is the order of the base point $G$.

- $h = \frac{\#E}{n}$ is the cofactor.

> Good choices are the FIPS 186-4 curves.

### Some famous curves

- ***The Bitcoin curve***: secp256k1. $E: y^2 \equiv x^3 + 7 \mod p$, where $p = 2^{256} - 2^{32} - 977$.

- ***Koblitz curves***: This curves are defined on $\mathbb{GF(2^m)}$. The form and the operation are defined in a different way.

    - $E_0: y^2 + xy \equiv x^3 + 1 \mod p$.

    - $E_1: y^2 + xy \equiv x^3 + x^2 + 1 \mod p$.

- ***P-160***: $E: y^2 \equiv x^3 + 3 \mod p$, where $p = 2^{160} - 229233$.

- ***Curve25519***: $E: y^2 \equiv x^3 + 486662x^2 + x \mod p$, where $p = 2^{255} - 19$.

- **Edwards curves**: $E: x^2 + y^2 \equiv 1 + dx^2y^2 \mod p$.

## Elliptic Curve Diffie-Hellman Key Exchange (ECDH)

### Protocol

Alice and Bob agree on the domain parameters $(p,a,b,G,n,h)$, where $E: y^2 \equiv x^3 + ax + b \mod p$ is the elliptic curve and $G = (x_G, y_G)$ is a base point on the curve $E$.

![ECDH](./assets/paar/ecdh.png)

The $k_{AB}$ is generally one of the coordinates of the point $K_{AB} = d_A \cdot Q_B = d_B \cdot Q_A$.

### Proof of Correctness

Alice computes: $K_{A} = A \cdot pB = A \cdot (B \cdot G) = (A \cdot B) \cdot G.$

Bob computes: $K_{B} = B \cdot pA = B \cdot (A \cdot G) = (B \cdot A) \cdot G.$

So, for the associativity and commutativity of the point addition, we can say that $K_{AB} = K_{BA}$.

### Computational Aspects

All operations performed are a **scalar multiplication** of a point by an integer. This can be done by the **double-and-add algorithm**.

The double-and-add algorithm is variant of the square-and-multiply algorithm for modular exponentiation. The procedure is the same, the only difference is that the operation performed in every iteration are:

- **Square** $\rightarrow$ **Point Doubling**.

- **Multiply** $\rightarrow$ **Point Addition**.

The algorithm is the following:

Scanning the bits of the scalar $d$ from left to right, we perform:

1. Doubling the point, indicated by the bit.

2. Adding the point, if current bit is $1$.

> For the first bit we perform always the operation: $P = 1_2 \cdot P$.

## EC-Massey-Omura Key Exchange

- Alice and Bob agree on the domain parameters $(p,a,b,G,n,h)$, where $E: y^2 \equiv x^3 + ax + b \mod p$ is the elliptic curve and $G = (x_G, y_G)$ is a base point on the curve $E$.
- Alice has chosen a private key $A \in {1, ..., n-1}$ such that $A$ is easy to invert modulo $n$.
- Bob has chosen a private key $B \in {1, ..., n-1}$ such that $B$ is easy to invert modulo $n$.

1. Alice computes: $C_0 = A \cdot M$ and sends it to Bob. Where $M$ is the message to be sent represented as point over the curve.

2. Bob computes: $C_1 = B \cdot C_0$ and sends it to Alice.

3. Alice computes: $C_2 = A^{-1} \cdot C_1$ and sends it to Bob.

4. Bob computes: $M = B^{-1} \cdot C_2$ and gets the message $M$.

### Proof of Correctness

Bob computes: 

$$
\begin{align}
    B^{-1} \cdot C_2 & \equiv B^{-1} \cdot (A^{-1} \cdot C_1) \equiv \\
    & \equiv  B^{-1} \cdot A^{-1} \cdot (B \cdot C_0) \equiv \\
    & \equiv B\cdot B^{-1} \cdot A^{-1} \cdot (A \cdot M) \equiv \\
    & \equiv 1 \cdot A \cdot A^{-1} \cdot M \equiv \\
    & \equiv 1 \cdot 1 \cdot M \equiv \\
    & \equiv M \mod p.
\end{align}
$$

## ECDSA

The Elliptic Curve Digital Signature Algorithm (ECDSA) is a variant of the DSA algorithm. The domain parameters are the same as the ECDH.

### Key Generation

1. The signer chooses a private key $d \in {1, ..., n-1}$.

2. The signer computes the public key $B = d \cdot G$.

The keys are:

- $K_{PR} = d$.

- $K_{Pub} = (p,a,b,n,G,B)$.

### Signature Generation

1. The signer chooses a random integer $K \in {1, ..., n-1}$ as ephemeral key.

2. Compute the point $R = K \cdot G = (x_R, y_R)$.

3. Compute $s = K^{-1} \cdot (H(m) + d \cdot x_R) \mod n$, where $H(m)$ is the hash of the message.

The signature is $(x_R,s)$.

> ***Note***: if $s = 0 \mod n$, then choose another $K$.

### Signature Verification

1. Compute $w = s^{-1} \mod n$.

2. Compute $u_1 = H(m) \cdot w \mod n$.

3. Compute $u_2 = x_R \cdot w \mod n$.

4. Compute the point $P = u_1 \cdot G + u_2 \cdot B = (x_P, y_P)$.

5. The signature is valid if $x_P \equiv x_R \mod n$.
