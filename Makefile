
ifneq (,$(findstring mingw,$(CC)))
BINEXT:=.exe
else
BINEXT:=.bin
endif
HEADERS=$(sort $(filter-out ./include/spew3d.h ./implementation/spew3d_prefix_all.h ./implementation/spew3d_prefix_miniz_c.h ./implementation/spew3d_postfix_miniz_c.h,$(wildcard ./include/*.h) $(wildcard ./implementation/*.h)))
SOURCES=$(sort $(wildcard ./implementation/*.c))
TESTPROG=$(sort $(patsubst %.c, %$(BINEXT), $(wildcard ./examples/example_*.c)))

all: amalgamate build-tests

amalgamate:
	cat implementation/spew3d_prefix_all.h vendor/miniz/include/miniz/miniz.h implementation/spew3d_prefix_miniz_c.h vendor/miniz/include/miniz/miniz.c implementation/spew3d_postfix_miniz_c.h vendor/stb/stb_image.h $(HEADERS) $(SOURCES) > include/spew3d.h

build-tests:
	cd examples && $(MAKE) clean && $(MAKE)

test: amalgamate build-tests
	cd examples && valgrind ./example_sprite.bin

update-vendor:
	@if [ -e "vendor/miniz/miniz.h" ]; then cd vendor/miniz && git reset --hard; fi
	git submodule init && git submodule update
	# Make sure miniz is available:
	cd vendor/miniz/ && mkdir -p ./include/miniz/ && rm -f ./include/miniz/miniz.c && rm -f ./include/miniz/miniz.h && rm -rf ./amalgamation/ && rm -rf ./_build && CC=gcc CXX=g++ CFLAGS= bash ./amalgamate.sh && cp ./amalgamation/miniz.c ./include/miniz/miniz.c && cp ./amalgamation/miniz.h ./include/miniz/miniz.h
	cd vendor/miniz && sed -i 's/\#include \"miniz\.h\"//g' ./include/miniz/miniz.c

clean:
	rm -f $(TESTPROG)
	rm -f ./include/spew3d.h
