TARGET = libgawen.so
MAJOR  = 1
MINOR  = 5
PATCH  = 1

SRC  = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
DEPS = $(SRC:.c=.d)

CFLAGS := -O2 -fPIC -fomit-frame-pointer -std=c99 \
	-pedantic -Wall -Wextra -MMD -pipe
LDFLAGS := -shared

CFLAGS += -D_LARGEFILE64_SOURCE

version = "$(MAJOR).$(MINOR).$(PATCH)"
CFLAGS += -DLIBGAWEN_VERSION="\"$(version)\"" \
					-DLIBGAWEN_VERSION_MAJOR="$(MAJOR)" \
					-DLIBGAWEN_VERSION_MINOR="$(MINOR)" \
					-DLIBGAWEN_VERSION_PATCH="$(PATCH)"

commit = $(shell ./hash.sh)
ifneq ($(commit), UNKNOWN)
	CFLAGS += -DLIBGAWEN_COMMIT="\"$(commit)\""
endif

ifndef DISABLE_DEBUG
	CFLAGS += -ggdb
else
	CFLAGS += -DNDEBUG=1
endif

ifdef VERBOSE
	Q :=
else
	Q := @
endif

# In FreeBSD systems, sometimes the correct cputype is not picked up.
# We check the log and enable it when it is available.
SSE42_SUPPORT=$(shell $(CC) -march=native -dM -E - < /dev/null | grep SSE4_2)
ifeq ($(SSE42_SUPPORT),)
	SSE42_SUPPORT=$(shell if [ -f /var/run/dmesg.boot ] ; then grep SSE4\.2 /var/run/dmesg.boot ; fi)
endif
ifneq ($(SSE42_SUPPORT),)
	CFLAGS += -msse4.2
endif

.PHONY: all clean install uninstall

%.o: %.c
	@echo "===> CC $<"
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET).$(version): $(OBJS)
	@echo "===> LD $@"
	$(Q)$(CC) $(OBJS) $(LDFLAGS) -o $@

clean:
	@echo "===> CLEAN"
	$(Q)rm -f *.o
	$(Q)rm -f *.d
	$(Q)rm -f $(TARGET) $(TARGET).$(version)

install: $(TARGET).$(version)
	@echo "===> Installing $(TARGET).$(version)"
	$(Q)install -s $(TARGET).$(version) /usr/lib
	$(Q)ln -fs /usr/lib/$(TARGET).$(version) /usr/lib/$(TARGET)
	$(Q)ln -fs /usr/lib/$(TARGET).$(version) /usr/lib/$(TARGET).$(MAJOR)
	$(Q)ln -fs /usr/lib/$(TARGET).$(version) /usr/lib/$(TARGET).$(MAJOR).$(MINOR)
	@echo "===> Installing headers"
	$(Q)mkdir -p /usr/include/gawen
	$(Q)cp *.h /usr/include/gawen
	$(Q)chmod 755 /usr/include/gawen
	$(Q)chmod 644 /usr/include/gawen/*.h

uninstall:
	@echo "===> Uninstalling $(TARGET).$(version)"
	$(Q)rm -f /usr/lib/$(TARGET).$(version)
	$(Q)rm -f /usr/lib/$(TARGET).$(MAJOR)
	$(Q)rm -f /usr/lib/$(TARGET).$(MAJOR).$(MINOR)
	$(Q)rm -f /usr/lib/$(TARGET)
	@echo "===> Uninstalling headers"
	$(Q)rm -rf /usr/include/gawen


-include $(DEPS)
