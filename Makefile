AR = ar rcu
CC += -std=c99
MKDIR_P = mkdir -p
RANLIB = ranlib

LIBS += -lm `pkg-config --libs openssl`
CFLAGS += -Wall -Wextra -pedantic -Werror -g `pkg-config --cflags openssl`
CPPFLAGS += -Ipkg
LDFLAGS += -g

PACKAGE_A = pkg/libprelude.a
PACKAGE_O = pkg/array.o pkg/assert.o pkg/bytes.o pkg/core.o pkg/error.o \
			pkg/rng.o pkg/string.o pkg/test.o

ALL_O = $(PACKAGE_O) cmd/hello.o test/test_assert.o test/test_rng.o \
		test/test_string.o
ALL_T = $(PACKAGE_A) bin/hello test/test_assert test/test_rng test/test_string
ALL_A = $(PACKAGE_A)

.PHONY: all
all: $(ALL_T)

$(PACKAGE_A): $(PACKAGE_O)
	$(AR) $@ $(PACKAGE_O)
	$(RANLIB) $@

bin/hello: cmd/hello.o $(PACKAGE_A)
	$(MKDIR_P) bin && $(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

test/test_assert: test/test_assert.o $(PACKAGE_A)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

test/test_rng: test/test_rng.o $(PACKAGE_A)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

test/test_string: test/test_string.o $(PACKAGE_A)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

pkg/%.o : pkg/%.c pkg/prelude.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(ALL_T) $(ALL_O)

.PHONY: check
check: test/test_assert test/test_rng test/test_string
	test/test_assert
	test/test_rng
	test/test_string
