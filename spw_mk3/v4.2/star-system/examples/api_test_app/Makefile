# The output directory to store the built executable
OUTPUT_DIR = bin/

# The output target
TARGET = $(OUTPUT_DIR)api_test_app

# The operating system
ifndef $(OPERATING_SYSTEM)
	OPERATING_SYSTEM := $(shell uname -s)
	ifneq ($(OPERATING_SYSTEM), QNX)
		OPERATING_SYSTEM := $(shell uname -o)
	endif
endif

SRC = advanced_address_example.c						\
      advanced_continuous_receive_example.c				\
	  advanced_multiple_packet_example.c				\
	  advanced_receive_example.c						\
	  advanced_send_and_receive_example.c				\
	  advanced_send_example.c							\
	  advanced_two_way_example.c						\
	  all_version_example.c								\
	  api_test_app.c									\
	  api_version_example.c								\
	  application_name_example.c						\
	  channel_callback_example.c						\
	  driver_list_example.c								\
	  firmware_version_example.c						\
	  simple_receive_example.c							\
	  simple_send_and_receive_example.c					\
	  simple_send_example.c								\
	  simple_two_way_example.c							\
	  time-code_example.c								\
	  transfer_callback_example.c						\
	  transfer_operation_list_send_receive_example.c	\
	  transfer_operation_list_send_example.c			\
	  transmit_errors_example.c							\
	  utilities.c

# Operating system specific properties
ifeq ($(OPERATING_SYSTEM), RTEMS)
    CC=sparc-rtems-gcc
    LD=sparc-rtems-ld
	LINK_FLAGS += -lstar_conf_service -lstar_conf_mssgr -lstar_rmap_initiator -lrmap_packet_library -lstar_driver_if -lstar-port -lstar_pci_driver
	CFLAGS += -DSTAR_CONFIG_API_REQUIRED
	SRC += star_rtems_init.c
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

ifdef DEBUG
	CFLAGS += -g -O0
else
	CFLAGS += -O3
endif

CFLAGS += -Wall -Wextra -I../../inc/star

OBJS = $(addsuffix .o, $(basename $(SRC)))

all: $(OUTPUT_DIR) $(TARGET)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

ifeq ($(OPERATING_SYSTEM), RTEMS)
RTEMS_INIT_C = star_rtems_init.c
$(RTEMS_INIT_C):
	cp ../star_rtems_init.c .
endif

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -lstar-api -lstar_conf_api_router -lstar_conf_api_mk2 $(LINK_FLAGS) -o $@

clean:
	$(RM) $(OBJS) $(TARGET) $(RTEMS_INIT_C)
