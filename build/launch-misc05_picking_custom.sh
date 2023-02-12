#!/bin/sh
bindir=$(pwd)
cd /home/andrew/programming/graphics/misc05_picking/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /home/andrew/programming/graphics/build/misc05_picking_custom 
	else
		"/home/andrew/programming/graphics/build/misc05_picking_custom"  
	fi
else
	"/home/andrew/programming/graphics/build/misc05_picking_custom"  
fi
