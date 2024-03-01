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
   LPBYTE         lpBuffer,
   INT            iSubMKFIndex,
   BOOL           fInitBuf,
   FILE          *fpSource
)
/*++
  Purpose:

    Extract the MKF subfile.

  Parameters:

    [IN]  buf - pointer to the destination buffer.

    [IN]  iSubMKFIndex - subfile number of MKF, -1 is to extract all.

    [IN]  fpSource - pointer to the fopen'ed MKF file.

  Return value:

    Integer value which indicates the size of the chunk.
    -1 if the chunk does not exist.
    -2 if there are error in parameters.

--*/
{
   INT            iChunkSize;
   UINT           uiChunkCount;
   LPSTR          lpszSubFormat;

   //
   // Get the total number of chunks.
   //
   uiChunkCount = PAL_MKFGetChunkCount(fpSource);
   if (iSubMKFIndex >= uiChunkCount)
   {
      //
      // Chunk does not exist......
      //
      return -1;
   }

   //
   // Load the sub file.
   //
   iChunkSize = PAL_MKFGetChunkSize(iSubMKFIndex, fpSource);

   //
   // Allocate all the needed memory at once for simplification
   //
   if (!fInitBuf) lpBuffer = (LPBYTE)UTIL_malloc(iChunkSize);

   PAL_MKFReadChunk(lpBuffer, iChunkSize, iSubMKFIndex, fpSource);

   return iChunkSize;
}

INT
PAL_DeYJ_1(
   LPSTR          lpszSourcePath,
   LPSTR          lpszDestinationPath
)
/*++
  Purpose:

    Extract the YJ_1 file.

  Parameters:

    [IN]  lpszSourcePath - MKF source file path.

    [IN]  lpszDestinationPath - Extract the destination path of the file.

  Return value:

    Integer value which indicates the size of the chunk.
    -1 if there are error in parameters.

--*/
{
   FILE          *fpSource;

   if (!lpszSourcePath || !lpszDestinationPath) PAL_CmdError(FILETYPE_MKF, ERRORTYPE_INVALIDPARAM);

   fpSource = UTIL_OpenRequiredFile(lpszSourcePath);


}
