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

#ifndef MAIN_H
#define MAIN_H

#include "Common.h"
#include "Util.h"
#include "PalUtil.h"

typedef enum tagERRORTYPE
{
   ERRORTYPE_INVALIDCMD,   // The user entered an invalid command.
   ERRORTYPE_MULTIPLECMD,  // The user entered multiple commands.
   ERRORTYPE_INVALIDPARAM, // The user entered an invalid parameter.
   ERRORTYPE_MISSPARAM,    // The user did not provide all necessary parameters.
} ERRORTYPE;

typedef enum tagFILETYPE
{
   FILETYPE_ALL,    // All type......
   FILETYPE_MKF,    // List file......
   FILETYPE_YJ_1,   // DOS versi.on efficient compression encoding
   FILETYPE_YJ_2,   // WIN version efficient compression encoding
   FILETYPE_SMKF,   // The list of raw file of BIN and RLE.
   FILETYPE_RLE,    // In the old days, Microsoft's 8-bit image compression encoding.
   FILETYPE_BIN,    // Raw file of DATA.MKF, SSS.MKF......
   FILETYPE_ACT,    // Raw file of PAT.MKF, Color table file for Adobe Photoshop.
   FILETYPE_COLORS, // FBP.MKF's raw file.
   FILETYPE_RNG,    // RNG.MKF's list of raw file.
} FILETYPE;

VOID
PAL_CmdError(
   FILETYPE          ftFileType,
   ERRORTYPE         etErrorType
);

VOID
PAL_CmdHelp(
   FILETYPE          ftFileType
);

#endif // MAIN_H
