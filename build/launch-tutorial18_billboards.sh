#!/bin/sh
bindir=$(pwd)
cd /home/andrew/programming/graphics/tutorial18_billboards_and_particles/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /home/andrew/programming/graphics/build/tutorial18_billboards 
	else
		"/home/andrew/programming/graphics/build/tutorial18_billboards"  
	fi
else
	"/home/andrew/programming/graphics/build/tutorial18_billboards"  
fi
