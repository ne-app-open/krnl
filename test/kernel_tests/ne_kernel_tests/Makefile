##################################################
# (c) Amlal El Mahrouss and Ne.app, licensed under the Apache 2.0 license.
# This file is for kernel testing.
##################################################

GCC=x86_64-w64-mingw32-g++ -Wl,-subsystem=17
LIB=-L../../../private/libSystem -lSystemKit
STD=-std=c++20 -DKT_TESTING_ENABLED
INCLUDE=-I../../../private -I../../../public -I../../../public/frameworks/ -I../../../ -I../../../private/libSystem 

OBJ_FILES = \
	error.test.exe \
	print.test.exe \
	process.test.exe

.PHONY: all
all: $(OBJ_FILES)

%.exe: %.cc
	@echo "==> Building test: $@"
	$(GCC) $(LIB) $< \
		$(STD) $(INCLUDE) -o $(basename $<).exe
