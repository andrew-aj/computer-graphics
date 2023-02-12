#!/bin/sh
bindir=$(pwd)
cd /home/andrew/programming/graphics/tutorial16_shadowmaps/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /home/andrew/programming/graphics/build/tutorial16_shadowmaps_simple 
	else
		"/home/andrew/programming/graphics/build/tutorial16_shadowmaps_simple"  
	fi
else
	"/home/andrew/programming/graphics/build/tutorial16_shadowmaps_simple"  
fi
