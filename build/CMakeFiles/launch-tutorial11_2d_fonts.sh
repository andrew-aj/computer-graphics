#!/bin/sh
bindir=$(pwd)
cd /home/andrew/programming/graphics/tutorial11_2d_fonts/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /home/andrew/programming/graphics/build/tutorial11_2d_fonts 
	else
		"/home/andrew/programming/graphics/build/tutorial11_2d_fonts"  
	fi
else
	"/home/andrew/programming/graphics/build/tutorial11_2d_fonts"  
fi
