################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
posix/PTLS.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/PTLS.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/aeabi_portable.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/aeabi_portable.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/clock.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/clock.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/memory.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/memory.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/mqueue.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/mqueue.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/pthread.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/pthread.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/pthread_barrier.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/pthread_barrier.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/pthread_cond.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/pthread_cond.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/pthread_mutex.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/pthread_mutex.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/pthread_rwlock.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/pthread_rwlock.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/sched.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/sched.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/semaphore.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/semaphore.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/sleep.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/sleep.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/timer.o: C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/freertos/timer.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"C:/Users/brand/workspace_ccstheia/freertos_builds_LP_MSPM0G3507_release_ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_2_05_01_00/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_01_00/source/ti/posix/ticlang" -I"C:/ti/mspm0_sdk_2_05_01_00/source" -I"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/include" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


