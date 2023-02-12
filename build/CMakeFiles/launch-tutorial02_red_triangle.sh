#!/bin/sh
bindir=$(pwd)
cd /home/andrew/programming/graphics/tutorial02_red_triangle/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /home/andrew/programming/graphics/build/tutorial02_red_triangle 
	else
		"/home/andrew/programming/graphics/build/tutorial02_red_triangle"  
	fi
else
	"/home/andrew/programming/graphics/build/tutorial02_red_triangle"  
fi
