# Root Makefile that delegates to Source/Makefile

all:
	$(MAKE) -C Source

clean:
	$(MAKE) -C Source clean

test:
	$(MAKE) -C Source test

.PHONY: all clean test
