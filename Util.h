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

#ifndef UTIL_H
#define UTIL_H

#include "PalCommon.h"

PAL_C_LINKAGE_BEGIN

VOID
UTIL_StrToUpper(
   LPSTR             lpszSource
);

LONG
UTIL_StrToLongInt(
   LPCSTR            lpszSource
);

const char *
UTIL_CombinePath(
	char       *buffer,
	size_t      buflen,
	int         numentry,
	...
);

FILE *
UTIL_OpenFileAtPathForMode(
   LPCSTR              lpszPath,
   LPCSTR              lpszFileName,
   LPCSTR              szMode
);

BOOL
UTIL_IsAbsolutePath(
	const char *lpszFileName
);

const char *
UTIL_GetFullPathName(
	char       *buffer,
	size_t      buflen,
	const char *basepath,
	const char *subpath
);

void
TerminateOnError(
   const char *fmt,
   ...
);

FILE *
UTIL_OpenRequiredFile(
   LPCSTR            lpszFileName
);

FILE *
UTIL_OpenRequiredFileForMode(
   LPCSTR            lpszFileName,
   LPCSTR            szMode
);

FILE *
UTIL_OpenFileForMode(
   LPCSTR               lpszFileName,
   LPCSTR               szMode
);

PAL_C_LINKAGE_END

#endif // UTIL_H
