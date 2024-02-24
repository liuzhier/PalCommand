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

#include "util.h"

static char internal_buffer[PAL_MAX_GLOBAL_BUFFERS + 1][PAL_GLOBAL_BUFFER_SIZE];
#define INTERNAL_BUFFER_SIZE_ARGS internal_buffer[PAL_MAX_GLOBAL_BUFFERS], PAL_GLOBAL_BUFFER_SIZE

VOID
UTIL_StrToUpper(
   LPSTR             lpszSource
)
/*++
  Purpose:

    Convert string to uppercase.

  Parameters:

    [OUT]  lpszSource - target string.

  Return value:

    None.

--*/
{
   while (*lpszSource) {
      if (islower(*lpszSource)) { // 检查当前字符是否是小写字母
         *lpszSource = toupper((unsigned char)*lpszSource); // 转换为大写
      }
      lpszSource++;
   }
}

LONG
UTIL_StrToLongInt(
   LPCSTR            lpszSource
)
/*++
  Purpose:

    Convert strings to long integers.

  Parameters:

    [IN]  lpszSource - target string.

  Return value:

    Conversion results.

--*/
{
   LPSTR lpszEndPtr;
   LONG  lRet = strtol(lpszSource, &lpszEndPtr, 10);

   if (lpszEndPtr == lpszSource) return -2;

   return lRet;
}

const char *
UTIL_CombinePath(
	char       *buffer,
	size_t      buflen,
	int         numentry,
	...
)
{
	if (buffer && buflen > 0 && numentry > 0)
	{
		const char *retval = buffer;
		va_list argptr;

		va_start(argptr, numentry);
		for (int i = 0; i < numentry && buflen > 1; i++)
		{
			const char *path = va_arg(argptr, const char *);
			int path_len = path ? strlen(path) : 0;
			int append_delim = (i < numentry - 1 && path_len > 0 && !PAL_IS_PATH_SEPARATOR(path[path_len - 1]));
			
			for (int is_sep = 0, j = 0; j < path_len && buflen > (size_t)append_delim + 1; j++)
			{
				//
				// Skip continuous path separators
				// 
				if (PAL_IS_PATH_SEPARATOR(path[j]))
				{
					if (is_sep)
						continue;
					else
						is_sep = 1;
				}
				else
				{
					is_sep = 0;
				}
				*buffer++ = path[j];
				buflen--;
			}
			//
			// Make sure a path delimeter is append to the destination if this is not the last entry
			// 
			if (append_delim)
			{
				*buffer++ = PAL_PATH_SEPARATORS[0];
				buflen--;
			}
		}
		va_end(argptr);

		*buffer = '\0';

		return retval;
	}
	else
	{
		return NULL;
	}
}

FILE *
UTIL_OpenFileAtPathForMode(
	LPCSTR              lpszPath,
	LPCSTR              lpszFileName,
	LPCSTR              szMode
)
{
	if (!lpszPath || !lpszFileName || !szMode) return NULL;

	//
	// Construct full path according to lpszPath and lpszFileName
	//
	const char *path = UTIL_GetFullPathName(INTERNAL_BUFFER_SIZE_ARGS, lpszPath, lpszFileName);

	//
	// If no matching path, check the open mode
	//
	if (path)
	{
		return fopen(path, szMode);
	}
	else if (szMode[0] != 'r')
	{
		return fopen(UTIL_CombinePath(INTERNAL_BUFFER_SIZE_ARGS, 2, lpszPath, lpszFileName), szMode);
	}
	else
	{
		return NULL;
	}
}

const char *
UTIL_GetFullPathName(
	char       *buffer,
	size_t      buflen,
	const char *basepath,
	const char *subpath
)
{
	if (!buffer || !basepath || !subpath || buflen == 0) return NULL;

	int sublen = strlen(subpath);
	if (sublen == 0) return NULL;

	char *_base = strdup(basepath), *_sub = strdup(subpath);
	const char *result = NULL;

	if (access(UTIL_CombinePath(INTERNAL_BUFFER_SIZE_ARGS, 2, _base, _sub), 0) == 0)
	{
		result = internal_buffer[PAL_MAX_GLOBAL_BUFFERS];
	}

#ifndef __EMSCRIPTEN__
#if !defined(PAL_FILESYSTEM_IGNORE_CASE) || !PAL_FILESYSTEM_IGNORE_CASE
	if (result == NULL)
	{
		size_t pos = strspn(_sub, PAL_PATH_SEPARATORS);

		if (pos < sublen)
		{
			char *start = _sub + pos;
			char *end = strpbrk(start, PAL_PATH_SEPARATORS);
			if (end) *end = '\0';

			//
			// try to find the matching file in the directory.
			//
			struct dirent **list;
			int n = scandir(_base, &list, 0, alphasort);
			while (n-- > 0)
			{
				if (!result && SDL_strcasecmp(list[n]->d_name, start) == 0)
				{
					result = UTIL_CombinePath(INTERNAL_BUFFER_SIZE_ARGS, 2, _base, list[n]->d_name);
					if (end)
						result = UTIL_GetFullPathName(INTERNAL_BUFFER_SIZE_ARGS, result, end + 1);
					else if (access(result, 0) != 0)
						result = NULL;
				}
				free(list[n]);
			}
			free(list);
		}
	}
#endif
#endif
	if (result != NULL)
	{
		size_t dstlen = min(buflen - 1, strlen(result));
		result = (char *)memmove(buffer, result, dstlen);
		buffer[dstlen] = '\0';
	}

	free(_base);
	free(_sub);

	return result;
}

void
TerminateOnError(
   const char *fmt,
   ...
)
// This function terminates the game because of an error and
// prints the message string pointed to by fmt both in the
// console and in a messagebox.
{
   va_list argptr;
   char string[256];
   extern VOID PAL_Shutdown(int);

   // concatenate all the arguments in one string
   va_start(argptr, fmt);
   vsnprintf(string, sizeof(string), fmt, argptr);
   va_end(argptr);

   fprintf(stderr, "\nFATAL ERROR: %s\n", string);
   PAL_FATAL_OUTPUT(string);

#ifdef _DEBUG
   assert(!"TerminateOnError()"); // allows jumping to debugger
#endif

   PAL_Shutdown(255);
}

FILE *
UTIL_OpenRequiredFile(
   LPCSTR            lpszFileName
)
/*++
  Purpose:

    Open a required file. If fails, quit the program.

  Parameters:

    [IN]  lpszFileName - file name to open.

  Return value:

    Pointer to the file.

--*/
{
   return UTIL_OpenRequiredFileForMode(lpszFileName, "rb");
}

FILE *
UTIL_OpenRequiredFileForMode(
   LPCSTR            lpszFileName,
   LPCSTR            szMode
)
/*++
  Purpose:

    Open a required file. If fails, quit the program.

  Parameters:

    [IN]  lpszFileName - file name to open.
    [IN]  szMode - file open mode.

  Return value:

    Pointer to the file.

--*/
{
   FILE *fp = UTIL_OpenFileForMode(lpszFileName, szMode);

   if (fp == NULL)
   {
       fp = fopen(lpszFileName, szMode);
   }

   if (fp == NULL)
   {
	   TerminateOnError("File open error(%d): %s!\n", errno, lpszFileName);
   }

   return fp;
}

FILE *
UTIL_OpenFileForMode(
   LPCSTR            lpszFileName,
   LPCSTR            szMode
)
/*++
  Purpose:

    Open a file. If fails, return NULL.

  Parameters:

    [IN]  lpszFileName - file name to open.
    [IN]  szMode - file open mode.

  Return value:

    Pointer to the file.

--*/
{
	//
	// If lpszFileName is an absolute path, use its last element as filename
	//
	if (UTIL_IsAbsolutePath(lpszFileName))
	{
		char *temp = strdup(lpszFileName), *filename = temp;
		FILE *fp = NULL;
		for (char *next = strpbrk(filename, PAL_PATH_SEPARATORS); next; next = strpbrk(filename = next + 1, PAL_PATH_SEPARATORS));
		if (*filename)
		{
			filename[-1] = '\0';
			fp = UTIL_OpenFileAtPathForMode(*temp ? temp : "/", filename, szMode);
		}
		free(temp);
		return fp;
	}

   //
   // The user entered a relative path......
   //
   TerminateOnError("File open error(%d): %s!\nPlease enter an absolute path!!!\n", errno, lpszFileName);

   return NULL;
	//return UTIL_OpenFileAtPathForMode(gConfig.pszGamePath, lpszFileName, szMode);
}
