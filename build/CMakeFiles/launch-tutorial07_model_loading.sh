#!/bin/sh
bindir=$(pwd)
cd /home/andrew/programming/graphics/tutorial07_model_loading/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /home/andrew/programming/graphics/build/tutorial07_model_loading 
	else
		"/home/andrew/programming/graphics/build/tutorial07_model_loading"  
	fi
else
	"/home/andrew/programming/graphics/build/tutorial07_model_loading"  
fi
