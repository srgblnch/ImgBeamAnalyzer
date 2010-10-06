static const char *RcsId = "$Header: /users/chaize/newsvn/cvsroot/Calculation/ImgBeamAnalyzer/src/main.cpp,v 1.6 2010-10-06 21:25:43 vince_soleil Exp $";
//+=============================================================================
//
// file :        main.cpp
//
// description : C++ source for a TANGO device server main.
//               The main rule is to initialise (and create) the Tango
//               system and to create the DServerClass singleton.
//               The main should be the same for every Tango device server.
//
// project :     TANGO Device Server
//
// $Author: vince_soleil $
//
// $Revision: 1.6 $ $
//
// $Log: not supported by cvs2svn $
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================

#ifndef IBA_DLL_EXPORT

#ifdef WIN32
#  pragma warning (disable: 4786)
#  pragma warning( disable: 4250 ) // 'x' inherits 'y' via dominance
#  pragma warning( push )
#  pragma warning( disable: 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data 
#  pragma warning( disable: 4311 ) // 'variable' : pointer truncation from 'type' to 'type' 
#  pragma warning( disable: 4312 ) // 'operation' : conversion from 'type1' to 'type2' of greater size 
#endif

#include <tango.h>

#ifdef WIN32
#  pragma warning( pop )
#endif

#if defined(ENABLE_CRASH_REPORT)
# include <crashreporting/crash_report.h>
#else
# define DECLARE_CRASH_HANDLER
# define INSTALL_CRASH_HANDLER
#endif

DECLARE_CRASH_HANDLER;

int main(int argc, char *argv[])
{
  INSTALL_CRASH_HANDLER;


	Tango::Util *tg = 0;
	try
	{
		// Initialise the device server
		//----------------------------------------
		tg = Tango::Util::init(argc,argv);

		// Create the device server singleton 
		//	which will create everything
		//----------------------------------------
		tg->server_init(false);

		// Run the endless loop
		//----------------------------------------
		cout << "Ready to accept request" << endl;
		tg->server_run();
	}
	catch (bad_alloc)
	{
		cout << "Can't allocate memory to store device object !!!" << endl;
		cout << "Exiting" << endl;
	}
	catch (CORBA::Exception &e)
	{
		Tango::Except::print_exception(e);
		
		cout << "Received a CORBA_Exception" << endl;
		cout << "Exiting" << endl;
	}
	tg->server_cleanup();
	return(0);
}

#endif // ifndef IBA_DLL_EXPORT
