all: build 

# install

build:
	make -C hall-effect-a1104
	make -C sonar-hcsr04

clean:
	rm -f *~ *.bak
	make -C hall-effect-a1104 clean
	make -C sonar-hcsr04 clean
