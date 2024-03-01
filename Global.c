/* -*- mode: c; tab-width: 4; c-basic-offset: 4; c-file-style: "linux" -*- */
//
// Copyright (c) 2009-2011, Wei Mingzhi <whistler_wmz@users.sf.net>.
// Copyright (c) 2011-2024, SDLPAL development team.
// All rights reserved.
//
// This file is part of SDLPAL.
//
// SDLPAL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 3
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "main.h"

static GLOBALVARS _gGlobals;
GLOBALVARS* const  gpGlobals = &_gGlobals;

VOID
PAL_FreeGlobals(
   VOID
)
/*++
  Purpose:

    Free global data.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   //
   // Close all opened files
   //
   UTIL_CloseFile(gpGlobals->fpSource);

   //
   // Free the global data
   //
   //UTIL_FreeBuffer(gpGlobals->lpMKFBuf);
   UTIL_FreeBuffer(gpGlobals->lpYJ_1Buf);
   UTIL_FreeBuffer(gpGlobals->lpYJ_2Buf);
   //UTIL_FreeBuffer(gpGlobals->lpSMKFBuf);
   UTIL_FreeBuffer(gpGlobals->lpRLEBuf);
   UTIL_FreeBuffer(gpGlobals->lpACTBuf);
   UTIL_FreeBuffer(gpGlobals->lpCOLORSBuf);
   UTIL_FreeBuffer(gpGlobals->lpRNGBuf);

   //
   // Clear the instance
   //
   memset(gpGlobals, 0, sizeof(GLOBALVARS));
}
