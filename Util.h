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
#include "Global.h"

typedef enum tagDATATYPE
{
   DATATYPE_BYTE,
   DATATYPE_WORD,
   DATATYPE_DWORD,
} DATATYPE;

PAL_C_LINKAGE_BEGIN

VOID
UTIL_StrToUpper(
   LPSTR             lpszSource
);

LONG
UTIL_StrToLongInt(
   LPCSTR            lpszSource
);

VOID
UTIL_SaveDataFile(
   LPCSTR             lpszSavePath,
   LPCSTR             lpszFileName,
   LPBYTE             lpData,
   size_t             size
);

LONG
UTIL_GetFileSize(
	FILE          *fp
);

VOID
UTIL_FreeBuffer(
   LPBYTE         lpBuffer
);

DWORD
UTIL_ByteToTallBit(
   LPBYTE         lpBuffer,
   DATATYPE       dtDataType
);

char *
UTIL_GlobalBuffer(
	int         index
);
#define PAL_BUFFER_SIZE_ARGS(i) UTIL_GlobalBuffer(i), PAL_GLOBAL_BUFFER_SIZE

/*++
  Purpose:

    Does a varargs printf into the user-supplied buffer,
	so we don't need to have varargs versions of all text functions.

  Parameters:

    buffer - user-supplied buffer.
	buflen - size of the buffer, including null-terminator.
    format - the format string.

  Return value:

    The value of buffer if buffer is non-NULL and buflen > 0, otherwise NULL.

--*/
char *
UTIL_va(
	char       *buffer,
	int         buflen,
	const char *format,
	...
);
#define PAL_va(i, fmt, ...) UTIL_va(UTIL_GlobalBuffer(i), PAL_GLOBAL_BUFFER_SIZE, fmt, __VA_ARGS__)

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

VOID
UTIL_CloseFile(
   FILE             *fp
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

void *
UTIL_malloc(
   size_t               buffer_size
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
