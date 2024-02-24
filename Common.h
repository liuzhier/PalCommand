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

#ifndef _COMMON_H
#define _COMMON_H 

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#ifdef __cplusplus
# define PAL_C_LINKAGE       extern "C"
# define PAL_C_LINKAGE_BEGIN PAL_C_LINKAGE {
# define PAL_C_LINKAGE_END   }
#else
# define PAL_C_LINKAGE
# define PAL_C_LINKAGE_BEGIN
# define PAL_C_LINKAGE_END
#endif

/* When porting SDLPAL to a new platform, please make a separate directory and put a file
   named 'pal_config.h' that contains marco definitions & header includes into the directory.
   The example of this file can be found in directories of existing portings.
 */
#include "pal_config.h"

#define PAL_fread(buf, elem, num, fp) if (fread((buf), (elem), (num), (fp)) < (num)) return -1

#ifdef _WIN32

# define PAL_FILESYSTEM_IGNORE_CASE 1

# define PAL_PATH_SEPARATORS "\\/"

# define SDL_SwapLE32(x) (x)
#endif

#ifndef PAL_FATAL_OUTPUT
# define PAL_FATAL_OUTPUT(s)
#endif

#define PAL_MAX_GLOBAL_BUFFERS 4
#define PAL_GLOBAL_BUFFER_SIZE 1024

#define PAL_DEBUG

#endif // _COMMON_H