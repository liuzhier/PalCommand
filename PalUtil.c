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

#include "Main.h"

INT
PAL_DeMKF(
   LPSTR          lpszSourcePath,
   INT            iSubMKFIndex,
   LPSTR          lpszDestinationPath
)
/*++
  Purpose:

    Get the number of chunks in an MKF archive.

  Parameters:

    [IN]  lpszSourcePath - MKF source file path.

    [IN]  iSubMKFIndex - subfile number of MKF.

    [IN]  lpszDestinationPath - Extract the destination path of the file.

  Return value:

    Integer value which indicates the number of chunks in the specified MKF file.

--*/
{
   FILE         *fp;

   if (!lpszSourcePath || !lpszDestinationPath) PAL_CmdError(FILETYPE_MKF, ERRORTYPE_INVALIDPARAM);

   fp = UTIL_OpenRequiredFile("abc.mkf");

   return 0;
}
