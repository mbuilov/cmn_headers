  
ifneq (command line,$(origin PRODUCT_NAME))
define PRODUCT_NAME
cmn_headers tests
endef
PRODUCT_NAME:=$(value PRODUCT_NAME)
endif    
ifneq (command line,$(origin PRODUCT_NAMES_H))
define PRODUCT_NAMES_H
product_names.h
endef
PRODUCT_NAMES_H:=$(value PRODUCT_NAMES_H)
endif               
ifneq (command line,$(origin OVERRIDES))
define OVERRIDES

endef
OVERRIDES:=$(value OVERRIDES)
endif            
ifneq (command line,$(origin TOP))
define TOP
/mnt/t/work/cmn_headers/tests
endef
TOP:=$(value TOP)
endif                                        
ifneq (command line,$(origin PRODUCT_VER))
define PRODUCT_VER
1.0.0
endef
PRODUCT_VER:=$(value PRODUCT_VER)
endif 
ifneq (command line,$(origin MTOP))
define MTOP
/mnt/t/work/clean-build
endef
endif                           
ifneq (command line,$(origin VENDOR_COPYRIGHT))
define VENDOR_COPYRIGHT
Copyright (C) 2015-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
endef
VENDOR_COPYRIGHT:=$(value VENDOR_COPYRIGHT)
endif                                                                 
ifneq (command line,$(origin OS))
define OS
LINUX
endef
endif               
ifneq (command line,$(origin CLEAN_BUILD_REQUIRED_VERSION))
define CLEAN_BUILD_REQUIRED_VERSION
0.6.3
endef
CLEAN_BUILD_REQUIRED_VERSION:=$(value CLEAN_BUILD_REQUIRED_VERSION)
endif          
ifneq (command line,$(origin BUILD))
define BUILD
/mnt/t/work/cmn_headers/tests/build
endef
BUILD:=$(value BUILD)
endif       
ifneq (command line,$(origin VENDOR_NAME))
define VENDOR_NAME
Michael M. Builov
endef
VENDOR_NAME:=$(value VENDOR_NAME)
endif                                    
ifneq (command line,$(origin PATH))
define PATH
/bin:/usr/bin:/opt/bin:/usr/local/bin:/usr/X11R7/bin
endef
endif 
ifneq (command line,$(origin SHELL))
define SHELL
/bin/sh
endef
endif
