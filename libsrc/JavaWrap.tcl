#!/bin/sh
# \
exec tclsh8.4 $0 $@


proc FixLine { Line } {

regsub -all \{ $Line " __BRACKETBEGIN__ "  Line
regsub -all \} $Line " __BRACKETEND__ " Line
regsub -all {\(} $Line " __PARENBEGIN__ "  Line
regsub -all {\)} $Line " __PARENEND__ " Line
regsub -all , $Line " __COMMA__ "  Line
regsub -all {"} $Line "  __QUOTE__ " Line
regsub -all {\<} $Line {} Line
regsub -all {\>} $Line {} Line
regsub -all {&} $Line {} Line

return $Line

}

proc GetNextWord { fd } {
global CurrentIndex
global CurrentLine

# puts "Looking at $CurrentLine"
set StillLooking 1
while { $StillLooking } {
	set StillLooking 0
	if { $CurrentIndex >= [ llength $CurrentLine ] } {
# puts "Getting New Line"
		set CurrentIndex 0
		set CurrentLine [ FixLine [ gets $fd ] ]
		}
	set Word [ lindex $CurrentLine $CurrentIndex ]
	incr CurrentIndex
	if { ( [ string first "//" $Word ] >= 0 ) } {
# puts "Swallow Comment"
		set CurrentIndex [ llength $CurrentLine ]
		set StillLooking 1
		}
	if { ( [ string first "/*" $Word ] >= 0 ) } {
# puts "Begin Comment"
		while { [ string first "*/" $Word ] < 0 } {
			set Word [ GetNextWord $fd ]
			}
# puts "End Comment"
		set StillLooking 1
		}
}
return $Word
}

proc NewMethod { Method } {
global JavaClass
global CurrentMethod

regsub {\*} $Method {} Method
set Methods {}
if { [ info exists JavaClass(Methods) ] } {
	set Methods $JavaClass(Methods)
}
if { [ lsearch $Methods $Method ] != -1 } {
	puts "$Method already Exists"
	set JavaClass(Arguments,$Method) {}
} else {
	set JavaClass(Methods) "$Methods $Method"
	set JavaClass(Arguments,$Method) {}
}
set CurrentMethod $Method
}

proc ProcessWord { Word BracketLevel fd } {
global LastWord
global Class
global JavaClass
global ParenLevel
global CurrentMethod

# puts "Process $Word $BracketLevel"
switch $BracketLevel {
	0 {
		puts "Derived From $Word"
		if { [ info exists JavaClass(Extends) ] == 0 } {
			set JavaClass(Extends) $Word
		} else {
			set Implements {}
			if { [ info exists JavaClass(Implements) ] } { 
			  set Implements [ array get JavaClass Implements ]
			}
			set JavaClass(Implements) "$Implements $Word"
			}
		}
	1 {
		switch $Word {
			XdmfGetValueMacro {
# puts "Begin Macro"
				set Paren [ GetNextWord $fd ]
#				incr ParenLevel
				set What [ GetNextWord $fd ]
				set Comma [ GetNextWord $fd ]
				set Macro [ format "%s_Get%s" $Class $What ]
# puts "Macro = $Macro"
 puts "Method = $Macro"
				NewMethod Get$What
				set ParamType [ GetNextWord $fd ]
				while { $ParamType != "__PARENEND__" } {
#					lappend JavaClass(Arguments,$CurrentMethod) $ParamType
					set ParamType [ GetNextWord $fd ]
				}
				set Word $Macro
				}
			XdmfGetIndexValueMacro {
# puts "Begin Macro"
				set Paren [ GetNextWord $fd ]
#				incr ParenLevel
				set What [ GetNextWord $fd ]
				set Comma [ GetNextWord $fd ]
				set Macro [ format "%s_Get%s" $Class $What ]
# puts "Macro = $Macro"
 puts "Method = $Macro"
				NewMethod Get$What
				lappend JavaClass(Arguments,$CurrentMethod) long
				lappend JavaClass(Arguments,$CurrentMethod) Index
				set ParamType [ GetNextWord $fd ]
				while { $ParamType != "__PARENEND__" } {
#					lappend JavaClass(Arguments,$CurrentMethod) $ParamType
					set ParamType [ GetNextWord $fd ]
				}
				set Word $Macro
				}
			XdmfSetIndexValueMacro {
puts "Begin Macro"
				set Paren [ GetNextWord $fd ]
#				incr ParenLevel
				set What [ GetNextWord $fd ]
				set Comma [ GetNextWord $fd ]
				set Macro [ format "%s_Set%s" $Class $What ]
puts "Macro = $Macro"
 puts "Method = $Macro"
				NewMethod Set$What
				lappend JavaClass(Arguments,$CurrentMethod) long
				lappend JavaClass(Arguments,$CurrentMethod) Index
				lappend JavaClass(Arguments,$CurrentMethod) __COMMA__
				set ParamType [ GetNextWord $fd ]
				while { $ParamType != "__PARENEND__" } {
					lappend JavaClass(Arguments,$CurrentMethod) $ParamType
					set ParamType [ GetNextWord $fd ]
				}
				lappend JavaClass(Arguments,$CurrentMethod) $What
				set Word $Macro
				}
			XdmfSetValueMacro {
puts "Begin Macro"
				set Paren [ GetNextWord $fd ]
#				incr ParenLevel
				set What [ GetNextWord $fd ]
				set Comma [ GetNextWord $fd ]
				set Macro [ format "%s_Set%s" $Class $What ]
puts "Macro = $Macro"
 puts "Method = $Macro"
				NewMethod Set$What
				set ParamType [ GetNextWord $fd ]
				while { $ParamType != "__PARENEND__" } {
					lappend JavaClass(Arguments,$CurrentMethod) $ParamType
					set ParamType [ GetNextWord $fd ]
				}
				lappend JavaClass(Arguments,$CurrentMethod) $What
				set Word $Macro
				}
			__PARENBEGIN__ {
				if { $ParenLevel == 0 } {
					if { $LastWord == $Class } {
 puts "Constructor/Destructor"
					NewMethod $LastWord
					} else {
 puts "Method = $LastWord"
					NewMethod $LastWord
						}
				} else {
#  puts "ParenLevel = $ParenLevel"
				}
				incr ParenLevel
				}
			__PARENEND__ {
				incr ParenLevel -1
				}
			default {
				switch $ParenLevel {
					1 {
puts "--Argument for $CurrentMethod = $Word"
					lappend JavaClass(Arguments,$CurrentMethod) $Word
					}
					default {
						puts "--- $Word"
						}
					}
				}
		}
		}
	default {
		puts "...................... Ignore $Word"
		}
	}
set LastWord $Word
}



proc JavaParse { FileName OutName } {

global CurrentIndex
global CurrentLine
global LastWord
global Class
global JavaClass
global ParenLevel
set CurrentIndex 0
set LastWord {}

puts "Parsing $FileName"
set fd [ open $FileName "r" ]
set JavaClass(Implements) {}
set JavaClass(Methods) {}
set Done 0
while { ( [ eof $fd ] == 0 ) && ( $Done == 0 )  } {
	set Line [ FixLine [ gets $fd ] ]
	if { [ string toupper [ lindex $Line 0 ] ]  == "CLASS" } {
		set CurrentLine $Line
		set Class [ lindex $Line 1 ]
		puts "Found Class -$Class-"
		set JavaClass(Name) $Class
		set SuperClass {}
		set IsDerived [ lsearch $Line ":" ]
		if { $IsDerived >= 0 } {
			set CurrentIndex $IsDerived
		} else {
			set CurrentIndex 2
		}
		if { $Class != {} } {
			set Word [ GetNextWord $fd ]
			set Done 0
			set BracketLevel 0
			set ParenLevel 0
			while { $Done == 0 } {
				regsub {~} $Word {} Word
				regsub {\-} $Word {} Word
				switch [ string toupper $Word ]  {
					__BRACKETBEGIN__ {
# puts "Begin Bracket"
						incr BracketLevel
						}
					__BRACKETEND__ {
# puts "End Bracket"
						incr BracketLevel -1
						if { $BracketLevel <= 0 } {
							set Done 1
							}
						}
					VIRTUAL -
					VIRTUAL: -
					PRIVATE -
					PRIVATE: -
					PUBLIC: -
					PUBLIC {
						}
					";" -
					":" {
						}
					""  {
						}
					default {
						ProcessWord $Word $BracketLevel $fd 
						}
					}
				set Word [ GetNextWord $fd ]
				}
			}
		}
	}
parray JavaClass
}

proc WrapMethod { Method NativeMethod ReturnType OutFd } {
global JavaClass

set Class $JavaClass(Name)
set Args $JavaClass(Arguments,$Method)
set ClassPointer [ format "%sPointer" $Class ]
regsub -all {\*} $Args { __STAR__ } Args
regsub -all {=} $Args { __EQUAL__ } Args
set Nargs 0
if { [ llength $Args ] > 0 } {
	set Nargs [ regexp -all __COMMA__ $Args ]
	incr Nargs
}
if { [ lindex $Args 0 ] == "void" } {
	set Nargs 0
	}
puts "Wrapping $NativeMethod with $Method with $Nargs Arguments -$Args-"
set ArgNumber 0
set Index 0
if { $Nargs > 0 } {
	set Opaque 1
	set Type long
	set ArgDefault ""
	set Pointer 0
	set ArgName Arg
	set MinArgs $Nargs
	foreach Arg $Args {
		switch $Arg {
			__EQUAL__ {
				set ArgDefault [ lindex $Args [ expr $ArgNumber + 1 ] ]
				if { $MinArgs == $Nargs } {
					set MinArgs $Index
					}
				}
			char {
				set Opaque 0
				if { [ lindex $Args [ expr $ArgNumber + 1 ] ] == "__STAR__" } {
					set Pointer 1
					set Type String
				} else {
					set Type byte
					}
				}
			XdmfString {
				set Pointer 1
				set Type String
				}
			__STAR__ {
				set Pointer 1
				}	
			XdmfInt8 -
			byte {
				set Opaque 0
				set Type byte
				}
			XdmfBoolean -
			XdmfInt32 -
			int {
				set Opaque 0
				set Type int
				}
			XdmfInt64 -
			long {
				set Opaque 0
				set Type long
				}
			XdmfFloat32 -
			float {
				set Opaque 0
				set Type float
				}
			XdmfFloat64 -
			double {
				set Opaque 0
				set Type double
				}
			__COMMA__ {
				if { $ArgDefault == {} } {
					set ArgName [ lindex $Args [ expr $ArgNumber - 1 ] ] 
				} else {
					set ArgName [ lindex $Args [ expr $ArgNumber - 3 ] ] 
				}
				if { $ArgName == "__STAR__" } {
					set Type String
					set ArgName "ValueString"
					}
				if { ( $Pointer == 1 ) && ( $Type != "String" )  } {
#					set Type long
					if { $Opaque == 0 } {
puts "------- Changing from \"$Type $ArgName\" to \"$Type \[\] $ArgName\""
						set Type "$Type \[\]"
					} else {
puts "------- Changing from \"$Type $ArgName\" to \"long $ArgName\""
						set Type long
						}
					}
				if { $ArgName == "String" } {
					set ArgName "ValueString"
					}
				set Values(ArgPointer,$Index) $Pointer
				set Values(ArgName,$Index) $ArgName
				set Values(ArgType,$Index) $Type
				set Values(ArgDefault,$Index) $ArgDefault
				incr Index
				set Opaque 1
				set Type long
				set ArgDefault ""
				set Pointer 0
				set ArgName Arg
				}
			default {
				}
			}
		incr ArgNumber
		}
		if { $ArgDefault == {} } {
			set ArgName [ lindex $Args [ expr $ArgNumber - 1 ] ] 
		} else {
			set ArgName [ lindex $Args [ expr $ArgNumber - 3 ] ] 
		}
		if { $ArgName == "__STAR__" } {
			set Type String
			set ArgName "ValueString"
			}
		if { ( $Pointer == 1 ) && ( $Type != "String" )  } {
#			set Type long
			if { $Opaque == 0 } {
puts "------- Changing from \"$Type $ArgName\" to \"$Type \[\] $ArgName\""
				set Type "$Type \[\]"
			} else {
puts "------- Changing from \"$Type $ArgName\" to \"long $ArgName\""
				set Type long
				}
			}
		if { $ArgName == "String" } {
			set ArgName "ValueString"
			}
		set Values(ArgPointer,$Index) $Pointer
		set Values(ArgName,$Index) $ArgName
		set Values(ArgType,$Index) $Type
		set Values(ArgDefault,$Index) $ArgDefault
	
# Output The Method
	puts -nonewline $OutFd "public $ReturnType $Method\("
	set LastArg [ expr $Nargs - 1 ]
	for { set i 0 } { $i < $LastArg } { incr i } {
		puts -nonewline $OutFd " $Values(ArgType,$i) $Values(ArgName,$i),"
		}
	puts $OutFd " $Values(ArgType,$i) $Values(ArgName,$i) ) \{"
	if { $ReturnType != "void" } {
		puts -nonewline $OutFd " return\("
	}
# Output the Call to the Native Method
	puts -nonewline $OutFd " $NativeMethod\( this.$ClassPointer "
	for { set i 0 } { $i < $Nargs } { incr i } {
		puts -nonewline $OutFd ", $Values(ArgName,$i)"
		}
	if { $ReturnType != "void" } {
		puts $OutFd " \)\);"
	} else {
		puts $OutFd "\);"
	}
	puts $OutFd "\}"
	if { $MinArgs != $Nargs } {
puts ".... Alternte MinArgs = $MinArgs"
# Output The Method
	puts -nonewline $OutFd "public $ReturnType $Method\("
		if { $MinArgs > 0 } {
		  puts -nonewline $OutFd " $Values(ArgType,0) $Values(ArgName,0)"
		  for { set i 1 } { $i < $MinArgs } { incr i } {
			puts -nonewline $OutFd ", $Values(ArgType,$i) $Values(ArgName,$i)"
			}
		}
	puts -nonewline $OutFd ") \{"
	if { $ReturnType != "void" } {
		puts -nonewline $OutFd " return\("
	}
	puts -nonewline $OutFd " $NativeMethod\( this.$ClassPointer "
		for { set i 0 } { $i < $MinArgs } { incr i } {
			puts -nonewline $OutFd ", $Values(ArgName,$i)"
			}
		for { set i $MinArgs } { $i < $LastArg } { incr i } {
			set DefaultValue $Values(ArgDefault,$i)
			if { ( $DefaultValue == "NULL" ) || 
				( $DefaultValue == "null" ) } {
				if { ( $Values(ArgPointer,$i) == 1 ) && ( $Values(ArgType,$i) == "String" ) } {
					set DefaultValue "null"
				} else {
					set DefaultValue "0"
				}
			}
			puts -nonewline $OutFd ", $DefaultValue"
			}
		set DefaultValue $Values(ArgDefault,$i)
		if { ( $DefaultValue == "NULL" ) || 
			( $DefaultValue == "null"  ) } {
				if { ( $Values(ArgPointer,$i) == 1 ) && ( $Values(ArgType,$i) == "String" ) } {
				set DefaultValue "null"
			} else {
				set DefaultValue "0"
			}
		}
		puts -nonewline $OutFd ", $DefaultValue"
	if { $ReturnType != "void" } {
		puts $OutFd " \)\);"
	} else {
		puts $OutFd "\);"
	}
	puts $OutFd "\}"

		}
    	} else {
	# No Arguments
	puts $OutFd "public $ReturnType $Method\() \{"
	if { $ReturnType != "void" } {
		puts -nonewline $OutFd "	return\("
	}
	puts -nonewline $OutFd " $NativeMethod\( this.$ClassPointer "
	if { $ReturnType != "void" } {
		puts $OutFd " \)\);"
	} else {
		puts $OutFd "\);"
	}
	puts $OutFd "\}"
	}
}

proc  JavaPrintClass { OutFd } {
global JavaClass

set Class $JavaClass(Name)

puts $OutFd "// java class for $JavaClass(Name)"
puts $OutFd "// "
puts $OutFd "package xdmf;"
puts $OutFd ""
puts $OutFd "import Xdmf;"
puts $OutFd "import xdmf.*;"
puts $OutFd ""
# if { [ info exists JavaClass(Extends) ] } {
# 	puts $OutFd "import $JavaClass(Extends).*;"
# }

set Extended 0
set ClassPointer [ format "%sPointer" $Class ]
set ClassPointerIsMine [ format "%sPointerIsMine" $Class ]
puts $OutFd "public class $Class"
if { [ info exists JavaClass(Extends) ] } {
	set Extended 1
	puts $OutFd " extends $JavaClass(Extends)"	
}
puts $OutFd " {"
puts $OutFd "	public $Class\(\) { 
			super();
			this.$ClassPointer = Xdmf.new_$Class\(\) ;
			this.XdmfInitialize();
			this.$ClassPointerIsMine = true;
			};"
if { $Extended } {
	puts $OutFd "	public $Class\( long XdmfPointer ) { 
			super();
			this.$ClassPointer = XdmfPointer;
			this.XdmfInitialize();
			this.$ClassPointerIsMine = false;
			super.XdmfInitDerived( XdmfPointer, \"$Class\"); }"
	puts $OutFd ""
	puts $OutFd "	public void finalize() {
				if( this.$ClassPointerIsMine ) {
				if( this.$ClassPointer > 0 ) Xdmf.delete_$Class\( this.$ClassPointer \);
				this.$ClassPointer = 0;
				}
			}"

	puts $OutFd "	public void XdmfInitDerived( long XdmfPointer, String ChildClass ) {
			String How;
			How = ChildClass+\"To\"+\"$Class\";
			this.$ClassPointer = Xdmf.XdmfCastPointer( XdmfPointer, How );
			super.XdmfInitDerived( XdmfPointer, \"$Class\"); }"
	puts $OutFd ""
	puts $OutFd "	public void XdmfInitialize() {
			super.XdmfInitDerived( this.$ClassPointer, \"$Class\");
			}"
} else {
	puts $OutFd "	public $Class\( long XdmfPointer ) { 
			super();
			this.XdmfInitialize();
			this.$ClassPointer = XdmfPointer;
			};"

	puts $OutFd "	public void XdmfInitDerived( long XdmfPointer, String ChildClass ) {
			String How;
			How = ChildClass+\"To\"+\"$Class\";
			this.$ClassPointer = Xdmf.XdmfCastPointer( XdmfPointer, How ); }"
	puts $OutFd ""
	puts $OutFd "	public void XdmfInitialize() {
			this.$ClassPointer = Xdmf.new_$Class\(\) ;
			}"
	puts $OutFd ""
	puts $OutFd "	public void finalize() {
				if( this.$ClassPointer > 0 ) Xdmf.delete_$Class\( this.$ClassPointer \);
				this.$ClassPointer = 0;
			}"
}
puts $OutFd " public long Get$ClassPointer\(\) { return( this.$ClassPointer ); }"
foreach Method $JavaClass(Methods) {
	regsub -all {\*} $Method {} Method
	set NativeMethod [ format "%s_%s" $JavaClass(Name) $Method ]
	set status [ catch "exec grep $NativeMethod\( Xdmf.java" Current ]
	if { $status } {
#		set status [ catch "exec grep $NativeMethod Xdmf.java" Current ]
	}
#	if { $status == 0  } {
#		if { [ regexp {\[\]}  $Current ] } {
#			puts "!! Ignoring Array Methods !!"
#			set status -1 
#			}
#		}
	if { $status == 0  } {
		regsub public $Current {} Current
		regsub static $Current {} Current
		regsub final $Current {} Current
		regsub native $Current {} Current
		set ReturnType [ lindex $Current 0 ]
		if { [ lindex $Current 1 ] == {[]} } {
			set ReturnType "$ReturnType \[\]"
		}
puts "+++++++++ ReturnType = $ReturnType from $Current"
		set JavaMethod $Current
		regsub ";" $JavaMethod {} JavaMethod
		regsub "long jarg0," $JavaMethod {} JavaMethod
		regsub "long jarg0" $JavaMethod {} JavaMethod
		regsub $NativeMethod $JavaMethod $Method JavaMethod
		set Nargs [ regexp -all jarg $Current ]

#		puts $OutFd "$Current"
#		puts "Wrapping Method $Method with $Nargs Arguments"
		WrapMethod $Method Xdmf.$NativeMethod $ReturnType $OutFd

#		if { $ReturnType == "void" } {
#			puts $OutFd "public $JavaMethod {"
#			puts -nonewline $OutFd "	Xdmf.$NativeMethod\( this.XdmfPointer  "
#			for { set i 1 } { $i < $Nargs } { incr i } {
#				puts -nonewline $OutFd ", jarg$i"
#				}
#			puts -nonewline $OutFd " \);"
#			puts $OutFd "		}"
#		} else {
#			puts $OutFd "public $JavaMethod {"
#			puts -nonewline $OutFd "	return\( Xdmf.$NativeMethod\( this.XdmfPointer  "
#			for { set i 1 } { $i < $Nargs } { incr i } {
#				puts -nonewline $OutFd ", jarg$i"
#				}
#			puts -nonewline $OutFd " \)\);"
#			puts $OutFd "		}"
#		}

	} else {
		puts ".............Method $Method has not been wrapped"
	}
#	puts $OutFd "$Method\(\)"
	}
puts $OutFd "public long $ClassPointer ;"
puts $OutFd "public boolean $ClassPointerIsMine ;"
puts $OutFd " }"
}

global JavaClass
set FileName [ lindex $argv end ]
set OutName [ format "%s.java" [ file rootname $FileName ] ]
JavaParse $FileName $OutName
set OutFd [ open $OutName "w" ]
puts ".................................."
JavaPrintClass $OutFd
close $OutFd
exit 0

