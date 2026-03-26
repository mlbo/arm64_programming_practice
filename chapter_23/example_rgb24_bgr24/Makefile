COPS = -g -march=armv8-a+sve

rgb24_bgr24_sve: rgb24_bgr24_sve.o rgb24_bgr24_asm.o
	$(ARMGNU)-gcc $(COPS) -o rgb24_bgr24_sve rgb24_bgr24_sve.o rgb24_bgr24_asm.o

rgb24_bgr24_sve.o:rgb24_bgr24_sve.c
	$(ARMGNU)-gcc -c rgb24_bgr24_sve.c $(COPS)
	
rgb24_bgr24_asm.o:rgb24_bgr24_asm.S
	$(ARMGNU)-gcc -c rgb24_bgr24_asm.S $(COPS)

clean:
	rm *.o
	rm rgb24_bgr24_sve
