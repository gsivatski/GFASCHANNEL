###############################################################################
# @file deliver.mk
#
# @brief CERN delivery
#
# Automatically generated by driver-gen.
# Delivery command looks like this: make deliver <-- deliver on $(ACCS)
# which is {lab, oper, oplhc} (as of 01.01.2008)
#
# You can deliver it for the specific one:
# make deliver lab
# make deliver oper
#
# You can deliver it for several:
# make deliver lab oper
#
#
# @author Copyright (C) 2003-2010 CERN. Yury GEORGIEVSKIY <ygeorgie@cern.ch>
#
# @date Created on 25/08/2009
#
# @section license_sec License
#          Released under the GPL
###############################################################################

# Major delivery pathes
UTILINSTDIR = /acc/local/$(CPU)/dg
EXECINSTDIR = /acc/dsc

wrong-dest := $(filter-out $(strip $(ACCS)) _deliver,$(MAKECMDGOALS))
__destination = $(filter $(strip $(ACCS)),$(MAKECMDGOALS))

# Don't allow make deliver (without explicit destination $(ACCS))
#destination := $(if $(__destination), $(__destination), $(ACCS))

destination := $(if $(__destination), $(__destination), $(ACCS))

# quiet down $(ACCS)
$(destination):
	@:

ifeq ($(notdir $(shell pwd)), ins_rm_mod)
# Deliver generic installation/deinstallation driver-gen executables
_deliver:
	$(if $(wrong-dest), \
		$(error wrong delivery place(s) "$(wrong-dest)"),)
	$(if $(destination),, \
		$(error you should explicitly specify destination "$(ACCS)"))
	@for a in $(destination); do \
		tmp_dir="$(EXECINSTDIR)/$$a/$(CPU)/bin"; \
		if [ -w $$tmp_dir ]; then \
			echo -en "\nDelivering [dgmodinst] && [dgmoduninst] in $$tmp_dir -- "; \
			dsc_install dgmodinst.$(CPU) $$tmp_dir; \
			sudo chown root.root $$tmp_dir/dgmodinst; \
			sudo chmod 6555 $$tmp_dir/dgmodinst; \
			\
			dsc_install dgmoduninst.$(CPU) $$tmp_dir; \
			sudo chown root.root $$tmp_dir/dgmoduninst; \
			sudo chmod 6555 $$tmp_dir/dgmoduninst; \
			echo -e "Done"; \
		fi; \
	done
	@echo ""
else ifeq ($(notdir $(shell pwd)), driver)
# Deliver the driver
include ./makefiles/drvr-paramount.mk
_deliver:
	$(if $(wrong-dest), \
		$(error wrong delivery place(s) "$(wrong-dest)"),)
	$(if $(destination),, \
		$(error you should explicitly specify destination "$(ACCS)"))
	@for a in $(destination); do \
		tmp_dir="$(EXECINSTDIR)/$$a/$(CPU)/$(KVER)/$(DRIVER_NAME)"; \
		if [ -w $$tmp_dir ]; then \
			echo -e "\nDelivering $(notdir $(DRIVER)) in $$tmp_dir "; \
			dsc_install $(DRIVER) $$tmp_dir; \
			\
			echo -e "\nDelivering $(notdir $(SIMULATOR)) in $$tmp_dir "; \
			dsc_install $(SIMULATOR) $$tmp_dir; \
			\
			echo -e "\nDelivering $(DRIVER_NAME).info in $$tmp_dir "; \
			dsc_install ../$(FINAL_DEST)/$(DRIVER_NAME).info $$tmp_dir; \
		elif [ ! -e $$tmp_dir ]; then \
			echo -e "\nCreating $$tmp_dir directory... "; \
			mkdir $$tmp_dir; \
			echo -e "Delivering $(notdir $(DRIVER)) in $$tmp_dir "; \
			dsc_install $(DRIVER) $$tmp_dir; \
			\
			echo -e "\nDelivering $(notdir $(SIMULATOR)) in $$tmp_dir "; \
			dsc_install $(SIMULATOR) $$tmp_dir; \
			\
			echo -e "\nDelivering $(DRIVER_NAME).info in $$tmp_dir "; \
			dsc_install ../$(FINAL_DEST)/$(DRIVER_NAME).info $$tmp_dir; \
		elif [ -e $$tmp_dir ]; then \
			echo -e "\nCan't deliver $(notdir $(DRIVER_NAME)) in $$tmp_dir"; \
			echo -e "You don't have write permissions!"; \
		fi; \
	done
	@echo ""
else ifeq ($(notdir $(shell pwd)), test)
# Deliver driver test program
_deliver:
	$(if $(wrong-dest), \
		$(error wrong delivery place(s) "$(wrong-dest)"),)
	$(if $(destination),, \
		$(error you should explicitly specify destination "$(ACCS)"))
	@for a in $(destination); do \
		tmp_dir="$(EXECINSTDIR)/$$a/$(CPU)/bin"; \
		if [ -w $$tmp_dir ]; then \
			echo -en "\nDelivering $(EXEC_OBJS) in $$tmp_dir -- "; \
			dsc_install $(FINAL_DEST)/$(EXEC_OBJS) $$tmp_dir; \
			echo -e "Done"; \
		fi; \
	done
	@echo ""
else ifeq ($(notdir $(shell pwd)), lib)
# Deliver .a
INSTDIR = $(UTILINSTDIR)
_deliver:
	@ umask 0002; \
	if [ -d $(INSTDIR)/$(DRVR) ]; then \
		if [ ! -d $(INSTDIR)/$(DRVR)/previousV ]; then \
			mkdir -p $(INSTDIR)/$(DRVR)/previousV; \
		fi; \
		echo "Saving current libraries..."; \
		$(CP) --preserve=mode,timestamps $(INSTDIR)/$(DRVR)/*.a $(INSTDIR)/$(DRVR)/previousV 2>/dev/null; \
		rm -f $(INSTDIR)/$(DRVR)/*.a; \
	else \
		echo "Creating $(INSTDIR)/$(DRVR) lib directory..."; \
		mkdir -p $(INSTDIR)/$(DRVR); \
	fi; \
	echo "Copying new $(LIBRARIES) libraries to '$(INSTDIR)/$(DRVR)' directory..."; \
	dsc_install $(addprefix $(FINAL_DEST)/, $(LIBRARIES)) $(INSTDIR)/$(DRVR)
else ifeq ($(notdir $(shell pwd)), install)
# Deliver .so
_deliver:
	@ umask 0002; \
	$(if $(wrong-dest), \
		$(error wrong delivery place(s) "$(wrong-dest)"),)
	$(if $(destination),, \
		$(error you should explicitly specify destination "$(ACCS)"))
	@for a in $(destination); do \
		tmp_dir="$(EXECINSTDIR)/$$a/$(CPU)/$(KVER)/$(DRIVER_NAME)"; \
		if [ -w $$tmp_dir ]; then \
			echo -e "\nDelivering $(LIBRARIES) in $$tmp_dir"; \
			dsc_install $(addprefix $(FINAL_DEST)/, $(LIBRARIES)) $$tmp_dir; \
		elif [ ! -e $$tmp_dir ]; then \
			echo -e "\nCreating $$tmp_dir directory... "; \
			mkdir $$tmp_dir; \
			echo -e "Delivering $(LIBRARIES) in $$tmp_dir"; \
			dsc_install $(addprefix $(FINAL_DEST)/, $(LIBRARIES)) $$tmp_dir; \
		elif [ -e $$tmp_dir ]; then \
			echo -e "\nCan't deliver $(LIBRARIES) in $$tmp_dir"; \
			echo -e "You don't have write permissions!"; \
		fi; \
	done
else ifeq ($(notdir $(shell pwd)), include)
# Deliver .h
INSTDIR = $(UTILINSTDIR)
_deliver:
	@ umask 0002; \
	if [ -d $(INSTDIR)/$(DRVR) ]; then \
		if [ ! -d $(INSTDIR)/$(DRVR)/previousV ]; then \
			mkdir -p $(INSTDIR)/$(DRVR)/previousV; \
		fi; \
		echo "Saving current header files..."; \
		$(CP) --preserve=mode,timestamps $(INSTDIR)/$(DRVR)/*.h $(INSTDIR)/$(DRVR)/previousV; \
		rm $(INSTDIR)/$(DRVR)/*.h; \
	else \
		echo "Creating $(INSTDIR)/$(DRVR) include directory..."; \
		mkdir -p $(INSTDIR)/$(DRVR); \
	fi; \
	echo "Copying new header files to '$(INSTDIR)/$(DRVR)' directory..."; \
	$(CP) --preserve=mode,timestamps *.h $(INSTDIR)/$(DRVR);
endif