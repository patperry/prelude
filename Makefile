AR = ar rcu
CC += -std=c99
MKDIR_P = mkdir -p
RANLIB = ranlib

LIBS += -lm `pkg-config --libs openssl`
CFLAGS += -Wall -Wextra -pedantic -Werror -g `pkg-config --cflags openssl`
CPPFLAGS += -Ipkg
LDFLAGS += -g

PACKAGE_A = pkg/libprelude.a
PACKAGE_O = pkg/core.o

ALL_O = $(PACKAGE_O) cmd/hello.o
ALL_T = $(PACKAGE_A) bin/hello
ALL_A = $(PACKAGE_A)

.PHONY: all
all: $(ALL_T)

$(PACKAGE_A): $(PACKAGE_O)
	$(AR) $@ $(PACKAGE_O)
	$(RANLIB) $@

bin/hello: cmd/hello.o $(PACKAGE_A)
	$(MKDIR_P) bin && $(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

pkg/%.o : pkg/%.c pkg/prelude.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(ALL_T) $(ALL_O)
