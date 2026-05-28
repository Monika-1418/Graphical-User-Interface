# The output directory to store the built executable
OUTPUT_DIR = bin/

# The output target
TARGET = $(OUTPUT_DIR)rmap_examples

# The operating system
ifndef $(OPERATING_SYSTEM)
	OPERATING_SYSTEM := $(shell uname -s)
	ifneq ($(OPERATING_SYSTEM), QNX)
		OPERATING_SYSTEM := $(shell uname -o)
	endif
endif

# Operating system specific properties
ifeq ($(OPERATING_SYSTEM), RTEMS)
    CC=sparc-rtems-gcc
    LD=sparc-rtems-ld
	LINK_FLAGS += -L../../lib
else
	ifeq ($(OPERATING_SYSTEM), QNX)
		CC = qcc
		CFLAGS += -Vgcc_ntox86
	else
		ifeq ($(OPERATING_SYSTEM), Cygwin)
			CPU_ARCHITECTURE := $(shell uname -m | sed "s/i.86/x86-32/" | sed "s/x86_64/x86-64/")
			LINK_FLAGS += -L../../lib/$(CPU_ARCHITECTURE)
		else
			CFLAGS += -pthread
		endif
		
		LINK_FLAGS += -lrt -lpthread
	endif
endif

SRC = examples.c	\
	crc_example.c	\
	check_packet_example.c	\
	read_command_packet_example.c	\
	read_reply_packet_example.c	\
	write_command_packet_example.c	\
	write_reply_packet_example.c	\
	rmw_command_packet_example.c	\
	rmw_reply_packet_example.c	\
	version_example.c

ifdef DEBUG
	CFLAGS += -g -O0
else
	CFLAGS += -O3
endif

CFLAGS += -Wall -I../../inc/star

OBJS = $(addsuffix .o, $(basename $(SRC)))

all: $(OUTPUT_DIR) $(TARGET)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -lrmap_packet_library $(LINK_FLAGS) -o $@

clean:
	$(RM) $(OBJS) $(TARGET)
