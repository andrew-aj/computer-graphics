#!/bin/sh
bindir=$(pwd)
cd /home/andrew/programming/graphics/tutorial12_extensions/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /home/andrew/programming/graphics/build/tutorial12_extensions 
	else
		"/home/andrew/programming/graphics/build/tutorial12_extensions"  
	fi
else
	"/home/andrew/programming/graphics/build/tutorial12_extensions"  
fi
