GPPPPARAMS = -m32
ASPARAMS = --32

objects = loader.o kernel.o

%.o: %.cpp
	g++ $(GPPPPARAMS) -o $@ -c $<

%.0 %.s
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)