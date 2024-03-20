# OpenSSL - HMACs

> ***Library***: `#include <openssl/evp.h>`

The approach of OpenSSL for HMACs is the same of the symmetric encryption and digests: incremental API.

## Pseudo codes

> ---
> ***HMAC***
> ```java
>   ctx = context_initialize(hash_algo, key)
>   while (data != NULL):
>       update_context(ctx, data)
>   end
>
>   hmac = finalize_context(ctx)
>   output(data, hmac)
> ```

> ---
> ***Verification***
> ```java
>   ctx = context_initialize(hash_algo, key)
>   while (data_recv != NULL):
>       update_context(ctx, data_recv)
>   end
>
>   hmac_computed = finalize_context(ctx)
>   if (cmp(hmac_computed, hmac_recv) == 0):
>       output("OK")
>   else:
>       output("KO")
>   end
> ```
> ---
