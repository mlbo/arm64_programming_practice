COPS = -g -march=armv8-a+sve

sve_test: sve_test.o sve_asm.o
	$(ARMGNU)-gcc $(COPS) -o sve_test sve_test.o sve_asm.o

sve_test.o:sve_test.c
	$(ARMGNU)-gcc -c sve_test.c $(COPS)
	
sve_asm.o:sve_asm.S
	$(ARMGNU)-gcc -c sve_asm.S $(COPS)

clean:
	rm *.o
	rm sve_test
