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

#include "PalCommon.h"

INT
PAL_MKFGetChunkCount(
   FILE          *fp
)
/*++
  Purpose:

    Get the number of chunks in an MKF archive.

  Parameters:

    [IN]  fp - pointer to an fopen'ed MKF file.

  Return value:

    Integer value which indicates the number of chunks in the specified MKF file.

--*/
{
   INT iNumChunk;
   if (fp == NULL)
   {
      return 0;
   }

   fseek(fp, 0, SEEK_SET);
   if (fread(&iNumChunk, sizeof(INT), 1, fp) == 1)
      return (SDL_SwapLE32(iNumChunk) - 4) >> 2;
   else
      return 0;
}

INT
PAL_MKFGetChunkSize(
   UINT           uiChunkNum,
   FILE          *fp
)
/*++
  Purpose:

    Get the size of a chunk in an MKF archive.

  Parameters:

    [IN]  uiChunkNum - the number of the chunk in the MKF archive.

    [IN]  fp - pointer to the fopen'ed MKF file.

  Return value:

    Integer value which indicates the size of the chunk.
    -1 if the chunk does not exist.

--*/
{
   UINT    uiOffset     = 0;
   UINT    uiNextOffset = 0;
   UINT    uiChunkCount = 0;

   //
   // Get the total number of chunks.
   //
   uiChunkCount = PAL_MKFGetChunkCount(fp);
   if (uiChunkNum >= uiChunkCount)
   {
      return -1;
   }

   //
   // Get the offset of the specified chunk and the next chunk.
   //
   fseek(fp, 4 * uiChunkNum, SEEK_SET);
   PAL_fread(&uiOffset, sizeof(UINT), 1, fp);
   PAL_fread(&uiNextOffset, sizeof(UINT), 1, fp);

   uiOffset = SDL_SwapLE32(uiOffset);
   uiNextOffset = SDL_SwapLE32(uiNextOffset);

   //
   // Return the length of the chunk.
   //
   return uiNextOffset - uiOffset;
}

INT
PAL_MKFReadChunk(
   LPBYTE         lpBuffer,
   UINT           uiBufferSize,
   UINT           uiChunkNum,
   FILE          *fp
)
/*++
  Purpose:

    Read a chunk from an MKF archive into lpBuffer.

  Parameters:

    [OUT] lpBuffer - pointer to the destination buffer.

    [IN]  uiBufferSize - size of the destination buffer.

    [IN]  uiChunkNum - the number of the chunk in the MKF archive to read.

    [IN]  fp - pointer to the fopen'ed MKF file.

  Return value:

    Integer value which indicates the size of the chunk.
    -1 if there are error in parameters.
    -2 if buffer size is not enough.

--*/
{
   UINT     uiOffset     = 0;
   UINT     uiNextOffset = 0;
   UINT     uiChunkCount;
   UINT     uiChunkLen;

   if (lpBuffer == NULL || fp == NULL || uiBufferSize == 0)
   {
      return -1;
   }

   //
   // Get the total number of chunks.
   //
   uiChunkCount = PAL_MKFGetChunkCount(fp);
   if (uiChunkNum >= uiChunkCount)
   {
      return -1;
   }

   //
   // Get the offset of the chunk.
   //
   fseek(fp, 4 * uiChunkNum, SEEK_SET);
   PAL_fread(&uiOffset, 4, 1, fp);
   PAL_fread(&uiNextOffset, 4, 1, fp);
   uiOffset = SDL_SwapLE32(uiOffset);
   uiNextOffset = SDL_SwapLE32(uiNextOffset);

   //
   // Get the length of the chunk.
   //
   uiChunkLen = uiNextOffset - uiOffset;

   if (uiChunkLen > uiBufferSize)
   {
      return -2;
   }

   if (uiChunkLen != 0)
   {
      fseek(fp, uiOffset, SEEK_SET);
      return (int)fread(lpBuffer, 1, uiChunkLen, fp);
   }

   return -1;
}
