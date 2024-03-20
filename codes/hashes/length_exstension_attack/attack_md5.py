import os
from md5 import md5, padding, _encode

if __name__ == "__main__":
    
    secret = os.urandom(16)     # Unknown secret (generated for testing)
    
    
    message = b'this is the long message I will hash!!!!! '
    
    # Generate the original hash for testing
    g = md5()
    g.update(secret + message)
    original_hash = g.hexdigest()
    print("Original hash: ", original_hash)
    
    # Set-up a new md5 generator with a specific state and counter to continue hashing
    i = 2 # Guess the number
    g_attack = md5(state= bytes.fromhex(original_hash) , count= i * md5.block_size*8 )
    print("State:         ", _encode(g_attack.state, 16).hex())
    
    # Add the additional message
    additional_message = b'pwned!!!!'
    g_attack.update(additional_message)
    attacked_hash = g_attack.hexdigest()
    print("Attacked hash: ", attacked_hash)
    
    # Test
    pad = padding(len(secret + message) * 8)
    # print("Padding:       ", pad.hex()) 
    
    test_msg = secret + message + pad + additional_message
    att_msg = message + pad + additional_message
    g_test = md5()
    g_test.update(test_msg)
    test_hash = g_test.hexdigest()
    
    print("Test message:  ", test_msg) 
    print("Attacked msg:  ", att_msg)
    print("Test hash:     ", test_hash)
    
    print("Is the attack successful? ", test_hash == attacked_hash)
    print(f"i = {i}")