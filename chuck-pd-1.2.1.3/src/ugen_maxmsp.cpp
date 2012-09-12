/*----------------------------------------------------------------------------
    ChucK Concurrent, On-the-fly Audio Programming Language
      Compiler and Virtual Machine

    Copyright (c) 2004 Ge Wang and Perry R. Cook.  All rights reserved.
      http://chuck.cs.princeton.edu/
      http://soundlab.cs.princeton.edu/

   This file was coded by Brad Garton for the [chuck~] object, summer 2006

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
    U.S.A.
-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// file: ugen_maxmsp.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Brad Garton (brad@music.columbia.edu)
// date: July, 2006
//-----------------------------------------------------------------------------
#include "ugen_maxmsp.h"
#include "chuck_type.h"
#include "chuck_ugen.h"
#include <stdio.h>
#include <stdlib.h>
using namespace std;

//-----------------------------------------------------------------------------
// name: maxmsp_query()
// desc: set up the max/msp functions
//-----------------------------------------------------------------------------
DLL_QUERY maxmsp_query( Chuck_DL_Query * QUERY )
{
    // get the env
    Chuck_Env * env = Chuck_Env::instance();

    Chuck_DL_Func * func = NULL;

    //! \section max i/o stuff
    
    // add MaxBang
    //! MaxBang -- send out a bang
    /*! \example
			MaxBang m;

			3.4::second

			1.0 => m.bangout;
    */
    //---------------------------------------------------------------------
    // init as base class: MaxBang
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "MaxBang", "UGen", env->global(), 
                                        MaxBang_ctor, MaxBang_dtor,
                                        NULL, NULL ) )
        return FALSE;

    // add ctrl: bangout
    func = make_new_mfun( "float", "bangout", MaxBang_ctrl_bangout );
    func->add_arg( "float", "bangout" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;


	 // add MaxMessage
	 //! MaxMessage -- send out a float
	 /*! \example
			MaxMessage m;
	 
			3.4::second
	 
			1.0 => m.floatout;
	 */
	 //---------------------------------------------------------------------
	 // init as base class: MaxMessage
	 //---------------------------------------------------------------------
	 if( !type_engine_import_ugen_begin( env, "MaxMessage", "UGen", env->global(), 
												MaxMessage_ctor, MaxMessage_dtor,
												NULL, NULL ) )
		  return FALSE;

	 // add ctrl: floatout
	 func = make_new_mfun( "float", "floatout", MaxMessage_ctrl_floatout );
	 func->add_arg( "float", "value" );
	 if( !type_engine_import_mfun( env, func ) ) goto error;

	 // end import
	 if( !type_engine_import_class_end( env ) )
		  return FALSE;


    // add MaxInlet
    //! MaxInlet -- get and return a value from a max object inlet
    //!		(up to 20 allowed)
    /*! \example
		MaxInlet mi;
		float val1, val2;

		1 => mi.inletval => val; // gets current value from inlet #1
		2 => mi.inletval => val; // gets current value from inlet #2
    */
    //---------------------------------------------------------------------
    // init as base class: MaxInlet
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "MaxInlet", "UGen", env->global(), 
													MaxInlet_ctor, MaxInlet_dtor,
													NULL, NULL ) )
        return FALSE;

    // add ctrl: inletval
    func = make_new_mfun( "float", "inletval", MaxInlet_ctrl_inletval );
    func->add_arg( "int", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;


	 return TRUE;

error:

	 // end the class import
	 type_engine_import_class_end( env );
	 
	return FALSE;
}


//-----------------------------------------------------------------------------
// name: MaxBang_ctor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MaxBang_ctor )
{
}

//-----------------------------------------------------------------------------
// name: MaxBang_dtor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_DTOR( MaxBang_dtor )
{
}

//-----------------------------------------------------------------------------
// name: MaxBang_ctrl_bangout()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( MaxBang_ctrl_bangout )
{
// BGG mm -- "bang_ready" is declared in chuck_main.cpp
// max/msp checks it each vector to determine if a bang should be sent
	bang_ready = 1;
}

//-----------------------------------------------------------------------------
// name: MaxMessage_ctor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MaxMessage_ctor )
{
}

//-----------------------------------------------------------------------------
// name: MaxMessage_dtor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_DTOR( MaxMessage_dtor )
{
}

//-----------------------------------------------------------------------------
// name: MaxMessage_ctrl_floatout()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( MaxMessage_ctrl_floatout )
{  
	t_CKFLOAT f = GET_CK_FLOAT(ARGS); 

	// BGG mm -- "vals_ready" and "maxmsp_vals[]" declared in chuck_main.cpp
	// max/msp checks and resets these.  "maxmsp_vals[]" holds the values

	maxmsp_vals[vals_ready] = f;

	if (vals_ready++ >= 999) {
		fprintf(stderr, "ruh roh! only 1000 vals allowed for each max/msp vector!\n");
		fprintf(stderr, "   I will overwrite the last element until the next vector...\n");
	}
}


//-----------------------------------------------------------------------------
// name: MaxInlet_ctor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MaxInlet_ctor )
{
}


//-----------------------------------------------------------------------------
// name: MaxInlet_dtor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_DTOR( MaxInlet_dtor )
{
}


//-----------------------------------------------------------------------------
// name: MaxInlet_ctrl_inletval()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( MaxInlet_ctrl_inletval )
{
	t_CKINT inletno = GET_CK_INT(ARGS);

	// BGG mm -- "inletvals" is declared in chuck_main.cpp
	RETURN->v_float = inletvals[inletno-1];
}
