#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.elf.C674{1,0,7.3,4
#
ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.oe674.dep
endif

package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.oe674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.oe674: package/package_ti.syslink.utils.rtos.bios6.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/package -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/package -s oe674 $< -C   -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/package -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.oe674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.oe674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.se674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.se674: package/package_ti.syslink.utils.rtos.bios6.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 -n $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/package -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/package -s oe674 $< -C  -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/package -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.se674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.se674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.oe674.dep
endif

package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.oe674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.oe674: ../osal/bios6/OsalPrint.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -s oe674 $< -C   -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.oe674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.oe674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.se674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.se674: ../osal/bios6/OsalPrint.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 -n $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -s oe674 $< -C  -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.se674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.se674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.oe674.dep
endif

package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.oe674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.oe674: ../osal/bios6/OsalKfile.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -s oe674 $< -C   -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.oe674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.oe674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.se674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.se674: ../osal/bios6/OsalKfile.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 -n $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -s oe674 $< -C  -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.se674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.se674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.oe674.dep
endif

package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.oe674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.oe674: ../osal/bios6/MemoryOS.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -s oe674 $< -C   -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.oe674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.oe674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.se674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.se674: ../osal/bios6/MemoryOS.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 -n $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -s oe674 $< -C  -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.se674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.se674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.oe674.dep
endif

package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.oe674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.oe674: ../../common/Trace.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -s oe674 $< -C   -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.oe674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.oe674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.se674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.se674: ../../common/Trace.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 -n $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -s oe674 $< -C  -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.se674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.se674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.oe674.dep
endif

package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.oe674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.oe674: ../../common/Cfg.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -s oe674 $< -C   -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.oe674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.oe674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.se674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.se674: ../../common/Cfg.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 -n $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -s oe674 $< -C  -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.se674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.se674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.oe674.dep
endif

package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.oe674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.oe674: TraceDrv.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6 -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6 -s oe674 $< -C   -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.oe674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.oe674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.se674: | .interfaces
package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.se674: TraceDrv.c lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle674 -n $< ...
	$(ti.targets.elf.C674.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6 -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.syslink.utils.rtos.bios6 -s oe674 $< -C  -n -s --symdebug:none -qq -pdsw225 -mv6740 --abi=eabi -eo.oe674 -ea.se674  -mi10 -mo --gcc -DSYSLINK_BUILD_RTOS  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_3_4 -O2  $(XDCINCS) -I$(ti.targets.elf.C674.rootDir)/include -fs=./package/lib/lib/release/ti.syslink.utils.rtos.bios6 -fr=./package/lib/lib/release/ti.syslink.utils.rtos.bios6
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.se674: export C_DIR=
package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.se674: PATH:=$(ti.targets.elf.C674.rootDir)/bin/:$(PATH)

clean,e674 ::
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.oe674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.oe674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.oe674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.oe674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.oe674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.oe674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.oe674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.se674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.se674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.se674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.se674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.se674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.se674
	-$(RM) package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.se674

lib/release/ti.syslink.utils.rtos.bios6.ae674: package/lib/lib/release/ti.syslink.utils.rtos.bios6/package/package_ti.syslink.utils.rtos.bios6.oe674 package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalPrint.oe674 package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/OsalKfile.oe674 package/lib/lib/release/ti.syslink.utils.rtos.bios6/../osal/bios6/MemoryOS.oe674 package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Trace.oe674 package/lib/lib/release/ti.syslink.utils.rtos.bios6/../../common/Cfg.oe674 package/lib/lib/release/ti.syslink.utils.rtos.bios6/TraceDrv.oe674 lib/release/ti.syslink.utils.rtos.bios6.ae674.mak

clean::
	-$(RM) lib/release/ti.syslink.utils.rtos.bios6.ae674.mak
