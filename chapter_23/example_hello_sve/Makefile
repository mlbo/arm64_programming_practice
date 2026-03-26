ARMGNU ?= aarch64-linux-gnu

all:
	$(ARMGNU)-gcc -g -march=armv8-a+sve -o hello hello_sve.S

clean:
	rm -f hello