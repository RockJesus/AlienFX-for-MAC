LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
   ALIENFX_BUILD := 64bit
else
   ALIENFX_BUILD := 32bit
endif

all:	build	deb

build:  build$(ALIENFX_BUILD)

build64bit: alienfx.cpp
	g++ -lusb-1.0 -o alienfx alienfx.cpp

build32bit: alienfx.cpp
	g++ -m32 -I/usr/include/libusb-1.0 -g -fPIC -lusb-1.0 -lpthread -o alienfx alienfx.cpp

install:
	install -o root -g root -m 4755 alienfx /usr/bin/alienfx
	install -o root -g root -m 4755 alienfx /usr/bin/alienfx

deb:	deb$(ALIENFX_BUILD)

deb64bit:
	equivs-build --full dpkg_amd64

deb32bit:
	equivs-build --full dpkg_i386

tarball:
	rm source.tar.gz
	cd .. && tar cvzf source.tar.gz alienfx/ && mv source.tar.gz alienfx/ && cd alienfx/

man:
	groff -Tascii -man alienfx.1 | less
