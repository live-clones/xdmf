#!/bin/sh
# \
grep "static void \*SwigXdmf" XdmfJava.cxx | sed -e "1,$ s/static//g" | sed -e "1,$ s/void \*//g" | sed -e "1,$ s/{//g" > PointerConverters
# \
exec tclsh8.4 $0 $@


set Code {
	if ( strcmp( Request, "SWIGSTRING" ) == 0 ){
		NewPtr = SWIGSUBROUTINE;
		*( void **)&ReturnValue = NewPtr;
		}
}

# set ofd stdout
set ofd [ open XdmfJava.cxx a+ ]
set fd [ open PointerConverters r ]


puts $ofd {extern "C"}
puts $ofd "JNIEXPORT jlong Java_Xdmf_XdmfCastPointer(JNIEnv *jenv, jclass jcls, jlong jarg0, jstring jarg1) {"
puts $ofd {	void 		*NewPtr, *ptr = *( void **)&jarg0;}
puts $ofd {	XdmfString	Request;}	
puts $ofd {	jlong		ReturnValue = 0;}
puts $ofd {}
puts $ofd {	Request = (jarg1) ? (char *)jenv->GetStringUTFChars(jarg1, 0) : NULL;}
while { [ eof $fd ] == 0 } {

	set SwigRoutine [ string trim [ gets $fd ] ]
	if { [ eof $fd ] == 0 } {
		set SwigString $SwigRoutine
		regsub -all Swig $SwigString {} SwigString
		regsub -all {\(ptr\)} $SwigString {} SwigString
#	puts $SwigString
		regsub -all SWIGSTRING $Code $SwigString CodeOut
		regsub -all SWIGSUBROUTINE $CodeOut $SwigRoutine CodeOut
		puts $ofd $CodeOut
		}
	}
puts $ofd { if(Request) jenv->ReleaseStringUTFChars(jarg1, Request); }
puts $ofd {  return( ReturnValue );}
puts $ofd "}"
close $ofd

set ifd [ open Xdmf.java r ]
set txt [ read $ifd ]
close $ifd
regsub "}" $txt "public final static native long XdmfCastPointer( long jarg0, String jarg1);
}" otxt

set ofd [ open Xdmf.java w ]
puts $ofd $otxt
close $ofd
