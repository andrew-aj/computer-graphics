#!/bin/sh
bindir=$(pwd)
cd /home/andrew/programming/graphics/tutorial05_textured_cube/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /home/andrew/programming/graphics/build/tutorial05_textured_cube 
	else
		"/home/andrew/programming/graphics/build/tutorial05_textured_cube"  
	fi
else
	"/home/andrew/programming/graphics/build/tutorial05_textured_cube"  
fi
