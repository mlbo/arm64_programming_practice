ARMGNU ?= aarch64-linux-gnu
COPS = -g -march=armv8-a+sve

sve_strcmp: sve_strcmp.o strcmp_asm.o
	$(ARMGNU)-gcc $(COPS) -o sve_strcmp sve_strcmp.o strcmp_asm.o

sve_strcmp.o:sve_strcmp.c
	$(ARMGNU)-gcc -c sve_strcmp.c $(COPS)

strcmp_asm.o:strcmp_asm.S
	$(ARMGNU)-gcc -c strcmp_asm.S $(COPS)

clean:
	rm -f *.o sve_strcmp
