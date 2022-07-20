
HEADERS=$(filter-out ./spew3d.h,$(wildcard ./*.h))
SOURCES=$(wildcard ./*.c)

all:
	cat $(HEADERS) $(SOURCES) > spew3d.h
