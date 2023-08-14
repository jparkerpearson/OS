.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
	.long MAGIC
	.long FLAGS
	.long CHECKSUM


.section .text
.extern kernelMain
.extern callConstructors
.global loader

loader: 
    #  Stack pointer is not set initially
    #  this is a problem because c++ programs expect
    #  that the stack pointer is already set.
    mov $kernel_stack, %esp


    #  We might need to initialize some constructors in the begining
    #  im not totally sure why yet.
    call callConstructors

    #  I this this is just a calling a function convention
    #  where the contents of the registers are saved to the stack 
    #  before calling the function. I am not really sure though,
    #  and I don't think theres anything in those regsiters we need.
    push %eax
    push %ebx
    call kernelMain

_stop:
    # ; cli clears the interupt flag so the program wont be interupted
    cli
    # ; hlt halts the cpu until an interupt is recieved
    hlt
    jmp _stop


.section .bss
# This sets 2 MiB of space before setting the kerenl stack pointer
# The kernel stack grows down so this just gives space for the kernel
# stack.
.space 2*1024*1024; # 2 MiB
# This label is basicaly a pointer to the memory location after all the space
# which is basically then start of the stack (which grows into the space).
kernel_stack:
