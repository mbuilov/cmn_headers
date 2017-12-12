#----------------------------------------------------------------------------------
# cmn_headers tests
# Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
# Licensed under Apache License v2.0, see LICENSE.TXT
#----------------------------------------------------------------------------------

# original file: $(CLEAN_BUILD_DIR)/stub/project.mk
# description:   project configuration makefile

# NOTE *********************************************************************************
# according to the clean-build principles, it is acceptable to use environment variables
# only while autoconfiguration: in this file and the files included by this one
# **************************************************************************************

# Assume custom project has the following directory structure:
# 
# +- my_project/
#   +- make/
#   |  |- project.mk    (modified copy of this file)
#   |  |- overrides.mk  (support for 'config' goal and OVERRIDES variable)
#   |  |- submakes.mk   (support for sub-makefiles)
#   |  |- c.mk          (support for building C/C++ sources)
#   |  ...
#   +-- src/
#   +-- include/
#   ...
#
# where 'make' - directory of the project build system,
#  some of the files in it can be the copies of the files of clean-build 'stub' directory

# the only case when variable TOP is overridden - after completing project configuration
ifneq (override,$(origin TOP))

# TOP - project root directory
# Note: TOP may be used in target makefiles for referencing sources, other makefiles, include paths, etc.
# Note: define TOP according to the project directory structure shown above - path to 'my_project' folder
override TOP := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))..)

# BUILD - path to the root directory of all built artifacts
# Note: this variable is required by clean-build and must be defined prior including core clean-build files
# Note: $(BUILD) directory is automatically created by clean-build when building targets
#  and automatically deleted by the predefined 'distclean' goal
BUILD := $(TOP)/build

# version of clean-build required by this custom project
CLEAN_BUILD_REQUIRED_VERSION := 0.9.0

# next variables are needed for generating:
# - header file with version info (see $(MTOP)/extensions/version/version.mk)
# - under Windows, resource files with version info (see $(MTOP)/compilers/msvc/stdres.mk)
PRODUCT_NAMES_H  := product_names.h
VENDOR_NAME      := Michael M. Builov
PRODUCT_NAME     := cmn_headers tests
VENDOR_COPYRIGHT := Copyright (C) 2015-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers

# global product version
# Note: this is default value of MODVER - per-module version number
# format: major.minor.patch
PRODUCT_VER := 1.0.0

# Note: may pre-define clean-build macros here - predefined values will override default ones, e.g:
#  SUPPORTED_TARGETS := DEVEL PRODUCTION

# include core clean-build definitions, processing of CONFIG and OVERRIDES variables,
#  define MTOP variable - path to clean-build
include $(dir $(lastword $(MAKEFILE_LIST)))overrides.mk

# Note: may redefine core clean-build macros here, e.g.:
#  $(call define_prepend,DEF_HEAD_CODE,$$(info target makefile: $$(TARGET_MAKEFILE)))

endif # TOP
