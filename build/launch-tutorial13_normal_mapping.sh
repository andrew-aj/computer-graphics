#!/bin/sh
bindir=$(pwd)
cd /home/andrew/programming/graphics/tutorial13_normal_mapping/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /home/andrew/programming/graphics/build/tutorial13_normal_mapping 
	else
		"/home/andrew/programming/graphics/build/tutorial13_normal_mapping"  
	fi
else
	"/home/andrew/programming/graphics/build/tutorial13_normal_mapping"  
fi
