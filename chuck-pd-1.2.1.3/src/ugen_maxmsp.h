/*----------------------------------------------------------------------------
    ChucK Concurrent, On-the-fly Audio Programming Language
      Compiler and Virtual Machine

    Copyright (c) 2004 Ge Wang and Perry R. Cook.  All rights reserved.
      http://chuck.cs.princeton.edu/
      http://soundlab.cs.princeton.edu/

	This file was coded by Brad Garton for the [chuck~] object, 7/2006

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
// file: ugen_maxmsp.h
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Brad Garton (brad@music.columbia.edu)
// date: July, 2006
//-----------------------------------------------------------------------------
#ifndef __UGEN_MAXMSP_H__
#define __UGEN_MAXMSP_H__

#include "chuck_dl.h"

extern "C" {
	extern int bang_ready;
	extern int vals_ready;
	extern float maxmsp_vals[]; // maximum of 1000 elements
	extern float inletvals[]; // maximum of 20 inlets (checked in chuck_main.cpp)
}

// query
DLL_QUERY maxmsp_query( Chuck_DL_Query * query );

// MaxBang
CK_DLL_CTOR( MaxBang_ctor );
CK_DLL_DTOR( MaxBang_dtor );
CK_DLL_CTRL( MaxBang_ctrl_bangout );

// MaxMessage
CK_DLL_CTOR( MaxMessage_ctor );
CK_DLL_DTOR( MaxMessage_dtor );
CK_DLL_CTRL( MaxMessage_ctrl_floatout );

// MaxInlet
CK_DLL_CTOR( MaxInlet_ctor );
CK_DLL_DTOR( MaxInlet_dtor );
CK_DLL_CTRL( MaxInlet_ctrl_inletval );

#endif
