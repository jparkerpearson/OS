#include "types.h"
#include "gdt.h"

void printf(char *str)
{
   // Whatever is written here will be put on the screen by the graphics card.
   // We want 2 byte pointers because first byte is color information and second byte is the character.
   uint16_t *VideoMemory = (uint16_t *)0xb8000;

   for (int i = 0; str[i] != '\0'; ++i)
   {
      // We can use a pointer as an array to access subsequent memomory.
      // Keep the color information in the first byte, but replace the second byte with the char.
      VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
   }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
   for (constructor *i = &start_ctors; i != &end_ctors; i++)
   {
      (*i)();
   }
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber)
{
   printf("Hello World! Welcome to my OS.");

   GlobalDescriptorTable gdt;

   while (1)
      ;
}