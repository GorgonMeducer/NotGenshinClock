#======================================================================================================================#
CC_SRC  := $(wildcard *.c)
CC_SRC  += $(wildcard platform/*.c)
CC_SRC  += $(wildcard platform/math/*.c)
CC_SRC  += $(wildcard genshin_clock/*.c)
CC_SRC  += $(wildcard genshin_clock/resources/*.c)
CC_SRC  += $(wildcard Arm-2D/Library/Source/*.c)
CC_SRC  += $(wildcard Arm-2D/Helper/Source/*.c)
CC_SRC  += $(wildcard Arm-2D/examples/common/asset/*.c)
CC_SRC  += $(wildcard Arm-2D/examples/common/benchmark/*.c)
CC_SRC  += $(wildcard Arm-2D/examples/common/controls/*.c)

CC_INC  := .
CC_INC  += genshin_clock
CC_INC  += sdl2/32/include/SDL2
CC_INC  += platform
CC_INC  += platform/math
CC_INC  += platform/math/dsp
CC_INC  += Arm-2D/Library/Include
CC_INC  += Arm-2D/Helper/Include
CC_INC  += Arm-2D/examples/common/benchmark
CC_INC  += Arm-2D/examples/common/controls

CC_DEF  := ARM_SECTION\(x\)=
CC_DEF  += __va_list=va_list

LD_INC  := sdl2/32/lib/x86

LD_LIB  := SDL2main SDL2

#======================================================================================================================#
OUT     :=  build\arm_2d.exe

CROSS   ?=  i686-w64-mingw32-

#----------------------------------------------------------------------------------------------------------------------#
CC      :=  $(CROSS)gcc
STRIP   :=  $(CROSS)strip
SIZE    :=  $(CROSS)size

#----------------------------------------------------------------------------------------------------------------------#
CCFLAG  +=  -std=gnu11 -Ofast -MMD -g
CCFLAG  +=  -ffunction-sections -fdata-sections
CCFLAG  +=  -flto

LDFLAG  +=  -Wl,--warn-common
LDFLAG  +=  -Wl,--gc-sections
LDFLAG  +=  -flto

#======================================================================================================================#
.DEFAULT_GOAL = all
_Comma := ,

ifneq ($(MAKECMDGOALS),clean)
  ifeq (${wildcard obj},)
    _NUL := ${shell mkdir obj}
  endif

  CC_OBJ := $(addprefix obj/,$(addsuffix .co,$(notdir $(CC_SRC))))

  $(foreach obj,$(CC_SRC),$(eval obj/$(notdir $(obj)).co : $(obj)))

  -include $(CC_OBJ:.co=.d)

  CCSuffix := $(CCFLAG) $(addprefix -I,$(CC_INC)) $(addprefix -D,$(CC_DEF))
  LDObject := $(CC_OBJ) $(addprefix -l,$(LD_LIB))
  LDSuffix := $(LDFLAG) $(addprefix -Wl$(_Comma)-L,$(LD_INC))
endif

#======================================================================================================================#
.PHONY: all
all: build/SDL2.dll $(OUT)
	@echo Build Completed.

#----------------------------------------------------------------------------------------------------------------------#
build/SDL2.dll:
	@echo Copy SDL2.dll
	@-cmd /c mkdir build
	@-cmd /c copy sdl2\32\bin\SDL2.dll build\SDL2.dll

#----------------------------------------------------------------------------------------------------------------------#
$(OUT): $(CC_OBJ)
	@echo Linking $(OUT) ...
	@$(CC) $(LDObject) $(LDSuffix) -o $(OUT)
	@echo

#----------------------------------------------------------------------------------------------------------------------#
obj/%.co:
	@echo Compile $(@F:.co=) ...
	@$(CC) -c $< -o $@ $(CCSuffix)

#----------------------------------------------------------------------------------------------------------------------#
.PHONY: clean
clean:
	@-cmd /c rd /s /q build
	@-cmd /c rd /s /q obj
	@echo Clean Completed.