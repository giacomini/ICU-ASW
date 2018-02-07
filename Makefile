RM = /bin/rm -fr
	
# Path to find C source code
VPATH = DB_definition:ext_and_data_structures:management_1553:PUS_services:telecommand:telemetry:interr_isr:.
ifeq (${DSW_VER}, 2_2_5)
	EXEC=asw_drv_2_2_5.exe
	RAW_FILE=asw_2_2_5.raw
else
	EXEC=asw_drv_2_1.exe
	RAW_FILE=asw_2_1.raw
endif

#EXEC=asw_drv_v1_10.exe

# Definition of the Project directory and executable (the user should not change this)
PGM=${ARCH}/$(EXEC)

MANAGERS=all

# Example of assembly source code filenames that could be included
ASSRCS = DeathReport.S
ASOBJS_ = $(ASSRCS:.S=.o)
ASOBJS = $(ASOBJS_:%=${ARCH}/%)

# C source code and headers filenames used in the example
CSRCS = init.c \
		tasks.c \
		RT_Configuration.c \
		BC_Configuration.c \
		TC_function.c \
		PUS_Service_1.c \
		PUS_Service_3.c \
		PUS_Service_3_diag.c \
		PUS_Service_5.c \
		PUS_3_DB_def.c \
		PUS_5_DB_def.c \
		PUS_8_DB_def.c \
		PUS_6_DB_def.c \
		PUS_Service_6.c \
		PUS_8_TM_HK_def.c \
		PUS_Service_8_1_transitions.c \
		PUS_Service_8_1.c \
		PUS_Service_8_1_DPU_BSW.c \
		PUS_Service_8_DCU.c \
		PUS_Service_8_DPU_config.c \
		PUS_Service_8_CPU.c \
		PUS_Service_8_SCE.c \
		PUS_Service_9.c \
		PUS_Service_17.c \
		db_type_def.c \
		tm.c \
		external_functions.c \
		icu_interr.c \
		PUS_Service_8_1_fwa_gwa.c \
		PUS_Service_8_1_heaters.c \
		PUS_Service_8_1_leds.c \
		PUS_Service_8_1_watchdog.c \
		PUS_Service_8_1_FDIR.c 

# Header files
CHDRS = rtems_configuration.h #RT_Configuration.h BC_Configuration.h milbus.h tc_header.h PUS_Service_1.h tm.h stdarg.h printk.h external_functions.h
COBJS_ = $(CSRCS:.c=.o)
COBJS = $(COBJS_:%=${ARCH}/%) ${ASOBJS}

# RTEMS Improvement RTEMS source code
RTEMS_USES_NEWLIB = no

# Include dsw library and include files
ifeq (${DSW_VER}, 2_2_5)
	BASE_DRIVERS_PATH = dsw_v2_2_5/include
	EBB_LIBRARY_DRIVERS_PATH = dsw_v2_2_5/lib
else
	BASE_DRIVERS_PATH = dsw_v2_1/include
	EBB_LIBRARY_DRIVERS_PATH = dsw_v2_1/lib
endif

#BASE_DRIVERS_PATH = dsw_v2_1/include
#EBB_LIBRARY_DRIVERS_PATH = dsw_v2_1/lib

LINK_LIBS += -B$(EBB_LIBRARY_DRIVERS_PATH) -licu_dsw_full

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

# Add the directories for header files of ASW
CFLAGS += -IDB_definition/interface \
	    -Iext_and_data_structures/interface \
	    -Iinterr_isr/interface \
	    -Imanagement_1553/interface \
	    -IPUS_services/interface \
		-Itelecommand/interface \
	    -Itelemetry/interface 	   

# Add the directories to the list of directories to be searched for header files
CFLAGS += -I$(BASE_DRIVERS_PATH)/common \
		  -I$(BASE_DRIVERS_PATH)/edac \
		  -I$(BASE_DRIVERS_PATH)/eeprom \
		  -I$(BASE_DRIVERS_PATH)/file_manager \
		  -I$(BASE_DRIVERS_PATH)/fpga_timers \
		  -I$(BASE_DRIVERS_PATH)/interrupt_manager \
		  -I$(BASE_DRIVERS_PATH)/led \
		  -I$(BASE_DRIVERS_PATH)/mdpa_timers \
		  -I$(BASE_DRIVERS_PATH)/milbus \
		  -I$(BASE_DRIVERS_PATH)/milbus/milbus_bc \
		  -I$(BASE_DRIVERS_PATH)/milbus/milbus_rt \
		  -I$(BASE_DRIVERS_PATH)/obt \
		  -I$(BASE_DRIVERS_PATH)/reset \
		  -I$(BASE_DRIVERS_PATH)/asw_runtime_init \
		  -I$(BASE_DRIVERS_PATH)/spi \
		  -I$(BASE_DRIVERS_PATH)/watchdog \
		  -I$(BASE_DRIVERS_PATH)/tm_acq_manager \
		  -I$(BASE_DRIVERS_PATH)/heater \
		  -I$(BASE_DRIVERS_PATH)/fwa_gwa_motors

OBJS = $(COBJS)

all: ${ARCH} $(PGM)
	
$(PGM): $(OBJS) $(CHDRS) $(LIBS)
	$(make-exe)
	sparc-rtems4.8-objcopy -O binary -S -g --gap-fill 0 o-optimize/${EXEC} o-optimize/${RAW_FILE}
	sparc-rtems4.8-ld o-optimize/${EXEC} -Map o-optimize/memory_map.txt 
	$(RM) a.out

# Make clean
clean :
	$(RM) -f *.o *.lst *.map *.dis *.hex *.srec *.raw $(PROG)


print-%  : ; @echo $* = $($*)
