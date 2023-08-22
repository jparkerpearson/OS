#include "gdt.h"
#include "types.h"

// Instructions are of the following format: See (https://en.wikipedia.org/wiki/Segment_descriptor) for more info.

////
//  Base 24:31  | Granularity Bit | Default operand size bit | (unused long bit) | Available bit for software |
//   Byte 7     |                                           Byte 6
//      Limit 16:19 | Present Bit | Privelage level (2 bits) | System Segment Bit | Type 0:3 |    Base 16:23
//                                  Byte 5                                                   |     Byte 4
//          -----------------------------
//    Base 0:15     |   Limit 0:15
//  Byte 3 | Byte 2 | Byte 1 | Byte 0
//////
/// Meanings
//      Granularity Bit - if clear limit in units of bytes, else limit in units of 4kb blocks
//      Default operand size bit - if clear 16 bit mode, else 32 bit mode. We always set to 32 bit mode.
//

// The limit is the size of the segment, base is the start of the segment, access is the type of segment it is
GlobalDescriptorTable::GlobalDescriptorTable()
    : nullSegmentSelector(0, 0, 0),
      unusedSegmentSelector(0, 0, 0),
      codeSegmentSelector(0, 64 * 1024 * 1024, 0x9A),
      dataSegmentSelector(0, 64 * 1024 * 1024, 0x92)
{
    // Store address of the gdt
    uint32_t i[2];
    i[0] = (uint32_t)this;
    // Shift to the right because its supposed to be the high address
    i[1] = sizeof(GlobalDescriptorTable) << 16;

    // Tell the processor to use this new GDT.
    asm volatile("lgdt (%0)"
                 :
                 : "p"(((uint8_t *)i) + 2));
}

GlobalDescriptorTable::~GlobalDescriptorTable()
{
}

uint16_t GlobalDescriptorTable::DataSegmentSelector()
{
    // The offset of the data segment is 1.
    return (uint8_t *)&dataSegmentSelector - (uint8_t *)this;
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector()
{
    // The offset of the code segment is 2.
    return (uint8_t *)&codeSegmentSelector - (uint8_t *)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags)
{
    uint8_t *target = (uint8_t *)this;

    //// Start Limit ////
    // If the limit is less then 16 bit limit
    if (limit <= 65536)
    {
        // Tells the processor to set granularity to 0 and operand to 1. This will use bytes for the limit.
        target[6] = 0x40;
    }

    else
    {
        // If it is larger than 16 bit limit then we're gonna use 4kb blocks.
        // If the limit passed in ends in 12 1's then it is already a multiple
        // of 4kb blocks so we just use the first 20 bits. If it's not a multiple
        // we subtract 1 to 'round' down. That way we are not overlapping memomory
        // that might be used for something else.
        if (limit & 0xFFF != 0xFFF)
        {
            limit = (limit >> 12) - 1;
        }
        else
        {
            limit = limit >> 12;
        }

        // Tells the processor to set granularity to 1 and operand to 1. This will use pages for the limit.
        target[6] = 0xC0;
    }

    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;
    ////////

    //// Address ////
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;
    ////////

    // Access
    target[5] = flags;
    ////////
}

// Return the rebuilt base
uint32_t GlobalDescriptorTable::SegmentDescriptor::Base()
{
    uint8_t *target = (uint8_t *)this;
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;
}

//  Return the rebuilt limit
uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit()
{
    uint8_t *target = (uint8_t *)this;
    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    // If the granularity bit is set then we need to multiply by 4kb
    if (target[6] & 0xC0 == 0xC0)
    {
        result = (result << 12) | 0xFFFF;
    }

    return result;
}