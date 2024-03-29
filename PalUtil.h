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

#ifndef _PALUTIL_H
#define _PALUTIL_H

#include "PalCommon.h"

PAL_C_LINKAGE_BEGIN

INT
PAL_DeMKF(
   LPBYTE         lpBuffer,
   INT            iSubMKFIndex,
   BOOL           fInitBuf,
   FILE          *fpSource
);

INT
PAL_DeYJ_1(
   LPSTR          lpszSourcePath,
   LPSTR          lpszDestinationPath
);

PAL_C_LINKAGE_END

#endif // _PALUTIL_H
