#!/bin/sh

# install/uninstall headers in specified directory

if { [ $# -ne 1 ] && [ $# -ne 2 ]; } ||
   { [ $# -eq 1 ] && [ "$1" == "-u" ]; } ||
   { [ $# -eq 2 ] && [ "$1" != "-u" ]; }
then
	echo "install/uninstall headers in specified directory"
	echo "usage: $0 [-u] <incdir>"
	echo "where:"
	echo "    -u       - if specified, then uninstall headers"
	echo "    <incdir> - directory where to copy/delete headers"
	exit 1
fi

if [ $# -eq 2 ]; then
	uninstall=1
	shift
else
	uninstall=0
fi

if [ ! -d "$1" ]; then
	echo "include directory \"$1\" does not exist"
	exit 2
fi

incdir="$1"
headers="bswaps.h ccasts.h dlist.h dprint.h get_opts.inl sal_defs.h"

if [ ${uninstall} -eq 1 ]; then

	# uninstall
	for f in ${headers}; do
		if [ -e "${incdir}/$f" ]; then
			echo "rm ${incdir}/$f"
			rm "${incdir}/$f" || exit 3
		fi
	done

else

	# install
	if [ -z "${INSTALL}" ]; then
		INSTALL=install
	fi

	echo "install ${headers} -> ${incdir}"
	${INSTALL} -m 644 ${headers} "${incdir}"

fi
