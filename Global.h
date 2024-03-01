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

#ifndef GLOBAL_H
#define GLOBAL_H

#include "Common.h"
#include "PalCommon.h"

typedef struct tagGLOBALVARS
{
   FILE          *fpSource;
   LONG           lSrcFileSize;
   LPBYTE         lpFileBuf;
   LPBYTE        *lpBuffer;

   LPBYTE         lpMKFBuf;
   LPBYTE         lpYJ_1Buf;
   LPBYTE         lpYJ_2Buf;
   LPCBITMAPRLE   lpSMKFBuf;
   WORD           wSMKFSize;
   LPBYTE         lpRLEBuf;

   LPBYTE         lpACTBuf;    // Subfiles of PAT.MKF / PALETTE.MKF
   LPBYTE         lpCOLORSBuf; // Subfiles of FBP.MKF | YJ_1 / YJ_2
   LPBYTE         lpRNGBuf;    // Subfiles of RNG.MKF
} GLOBALVARS, * LPGLOBALVARS;

PAL_C_LINKAGE_BEGIN

extern GLOBALVARS* const gpGlobals;

VOID
PAL_FreeGlobals(
   VOID
);

PAL_C_LINKAGE_END

#endif // GLOBAL_H
