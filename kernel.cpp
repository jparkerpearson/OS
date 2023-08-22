#include "types.h"
#include "gdt.h"

void printf(char *str)
{
   // Whatever is written here will be put on the screen by the graphics card.
   // We want 2 byte pointers because first byte is color information and second byte is the character.
   static uint16_t *VideoMemory = (uint16_t *)0xb8000;

   static uint8_t x = 0, y = 0;

   for (int i = 0; str[i] != '\0'; ++i)
   {

      switch (str[i])
      {
      case '\n':
         x = 0;
         y++;
         break;
      default:
         // We can use a pointer as an array to access subsequent memomory.
         // Keep the color information in the first byte, but replace the second byte with the char.
         VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
         x++;
      }

      // The screen is 80 characters wide and 25 characters tall.
      if (x >= 80)
      {
         x = 0;
         y++;
      }

      // If we reach the end of the scree clear the screen.
      if (y >= 25)
      {
         for (y = 0; y < 25; y++)
         {
            for (x = 0; x < 80; x++)
            {
               VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            }
         }

         x = 0;
         y = 0;
      }
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
   printf("Hello World! Welcome to my OS. \n");
   printf("My name is Parker.");

   GlobalDescriptorTable gdt;

   while (1)
      ;
}