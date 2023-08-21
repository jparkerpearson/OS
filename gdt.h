#ifndef __GDT_H
#define __GDT_H
#include "types.h"

class GlobalDescriptorTable
{
public:
    class SegmentDescriptor
    {
        // Global descriptor is broken up in this weird way for historical reasons.
    private:
        // The lower 16 bits of the limit. The limit tells how big the segment is.
        uint16_t limit_lo;
        // Base is the address of the first byte of the segment.
        // The base is split into two parts.
        uint16_t base_lo;
        uint8_t base_hi;
        // Defines the type of the segment. Could be data, could be code, could be something else.
        uint8_t type;
        // Contains the 4 bits of the segments limit, also holds different flags.
        uint8_t flags_limit_hi;
        // The last 8 bits of the base.
        uint8_t base_vhi;

    public:
        SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
        uint32_t Base();
        uint32_t Limit();

    } __attribute__((packed));

    SegmentDescriptor nullSegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;

public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    uint16_t CodeSegmentSelector();
    uint16_t DataSegmentSelector();
};
#endif