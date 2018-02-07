#
# BSP specific settings. To be included in application Makefiles
#

RTEMS_BSP = ebb

prefix = rtems-g
exec_prefix = rtems-g/sparc-rtems4.8

CC_FOR_TARGET = sparc-rtems4.8-gcc --pipe
CXX_FOR_TARGET = 
AS_FOR_TARGET = sparc-rtems4.8-as
AR_FOR_TARGET = sparc-rtems4.8-ar
NM_FOR_TARGET = sparc-rtems4.8-nm
LD_FOR_TARGET = sparc-rtems4.8-ld
SIZE_FOR_TARGET = sparc-rtems4.8-size
OBJCOPY_FOR_TARGET = sparc-rtems4.8-objcopy

CC= $(CC_FOR_TARGET)
CXX= $(CXX_FOR_TARGET)
AS= $(AS_FOR_TARGET)
LD= $(LD_FOR_TARGET)
NM= $(NM_FOR_TARGET)
AR= $(AR_FOR_TARGET)
SIZE= $(SIZE_FOR_TARGET)
OBJCOPY= $(OBJCOPY_FOR_TARGET)

export CC
export CXX
export AS
export LD
export NM
export AR
export SIZE
export OBJCOPY

RTEMS_CUSTOM = $(prefix)/make/custom/$(RTEMS_BSP).cfg
PROJECT_ROOT = $(prefix)
RTEMS_USE_OWN_PDIR = no
RTEMS_HAS_CPLUSPLUS = no

export RTEMS_BSP
export RTEMS_CUSTOM
export PROJECT_ROOT

# FIXME: The following shouldn't be here
RTEMS_ROOT = $(PROJECT_ROOT)
export RTEMS_ROOT
