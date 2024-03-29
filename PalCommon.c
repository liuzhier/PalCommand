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

WORD
PAL_SpriteGetNumFrames(
   LPCSPRITE      lpSprite
)
/*++
  Purpose:

    Get the total number of frames of a sprite.

  Parameters:

    [IN]  lpSprite - pointer to the sprite.

  Return value:

    Number of frames of the sprite.

--*/
{
   if (lpSprite == NULL)
   {
      return 0;
   }

   return (lpSprite[0] | (lpSprite[1] << 8)) - 1;
}

LPCBITMAPRLE
PAL_SpriteGetFrame(
   LPCSPRITE      lpSprite,
   INT            iFrameNum
)
/*++
  Purpose:

    Get the pointer to the specified frame from a sprite.

  Parameters:

    [IN]  lpSprite - pointer to the sprite.

    [IN]  iFrameNum - number of the frame.

  Return value:

    Pointer to the specified frame. NULL if the frame does not exist.

--*/
{
   int imagecount, offset;

   if (lpSprite == NULL)
   {
      return NULL;
   }

   //
   // Hack for broken sprites like the Bloody-Mouth Bug
   //
//   imagecount = (lpSprite[0] | (lpSprite[1] << 8)) - 1;
   imagecount = (lpSprite[0] | (lpSprite[1] << 8));

   if (iFrameNum < 0 || iFrameNum >= imagecount)
   {
      //
      // The frame does not exist
      //
      return NULL;
   }

   //
   // Get the offset of the frame
   //
   iFrameNum <<= 1;
   offset = ((lpSprite[iFrameNum] | (lpSprite[iFrameNum + 1] << 8)) << 1);
   if (offset == 0x18444) offset = (WORD)offset;
   return &lpSprite[offset];
}

INT
PAL_SpriteGetSizeOfFrame(
   LPBYTE         lpBuffer,
   WORD           wChunkNum,
   WORD           wSMKFSize
)
/*++
  Purpose:

    Get the size of a chunk in an SMKF archive.

  Parameters:

    [IN]  lpBuffer - pointer to the destination buffer.

    [IN]  wChunkNum - the number of the chunk in the SMKF archive.

    [IN]  wSMKFSize - the size of SMKF archive.

  Return value:

    Integer value which indicates the size of the chunk.
    -1 if the chunk does not exist.

--*/
{
   WORD    wOffset = 0;
   WORD    wNextOffset = 0;
   WORD    wChunkCount;

   //
   // Check whether the wChunkNum is out of range..
   //
   wChunkCount = PAL_SpriteGetNumFrames(lpBuffer);
   if (wChunkNum >= (wChunkCount + 1)) return -1;

   //
   // Get the offset of the specified chunk and the next chunk.
   //
   wOffset     = SDL_SwapLE32(((WORD*)lpBuffer)[wChunkNum])     << 1;
   wNextOffset = SDL_SwapLE32(((WORD*)lpBuffer)[wChunkNum + 1]) << 1;

   if (!wOffset) return -1;
   if (!wNextOffset || wChunkNum == wChunkCount || wNextOffset > wSMKFSize) wNextOffset = wSMKFSize;

   //
   // Return the length of the chunk.
   //
   return wNextOffset - wOffset;
}

INT
PAL_MKFGetNumChunks(
   LPBYTE         lpBuffer
)
/*++
  Purpose:

    Get the total number of frames of MKF.

  Parameters:

    [IN]  lpBuffer - pointer to the destination buffer.

  Return value:

    Number of frames of the sprite.

--*/
{
   INT iNumChunk;

   if (lpBuffer == NULL)
   {
      return 0;
   }

   iNumChunk = SDL_SwapLE32(((INT*)lpBuffer)[0]);

   return (iNumChunk >> 2) - 1;
}

INT
PAL_MKFGetSizeOfChunk(
   LPBYTE         lpBuffer,
   UINT           uiChunkNum
)
/*++
  Purpose:

    Get the size of a chunk in an MKF archive.

  Parameters:

    [IN]  lpBuffer - pointer to the destination buffer.

    [IN]  uiChunkNum - the number of the chunk in the MKF archive.

  Return value:

    Integer value which indicates the size of the chunk.
    -1 if the chunk does not exist.

--*/
{
   UINT    uiOffset     = 0;
   UINT    uiNextOffset = 0;

   //
   // Check whether the uiChunkNum is out of range..
   //
   if (uiChunkNum >= PAL_MKFGetNumChunks(lpBuffer)) return -1;

   //
   // Get the offset of the specified chunk and the next chunk.
   //
   uiOffset     = SDL_SwapLE32(((UINT*)lpBuffer)[uiChunkNum + 1]);
   uiNextOffset = SDL_SwapLE32(((UINT*)lpBuffer)[uiChunkNum + 2]);

   //
   // Return the length of the chunk.
   //
   return uiNextOffset - uiOffset;
}

INT
PAL_MKFGetChunkData(
   LPBYTE         lpSrcBuf,
   LPBYTE         lpDestBuf,
   UINT           uiBufferSize,
   UINT           uiChunkNum
)
/*++
  Purpose:

    Read a chunk from an MKF archive into lpBuffer.

  Parameters:

    [IN] lpBuffer - pointer to the source buffer.

    [OUT] lpBuffer - pointer to the destination buffer.

    [IN]  uiBufferSize - size of the destination buffer.

    [IN]  uiChunkNum - the number of the chunk in the MKF archive to read.

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

   if (lpSrcBuf == NULL || lpDestBuf == NULL || uiBufferSize == 0) return -1;

   //
   // Get the total number of chunks.
   //
   uiChunkCount = PAL_MKFGetNumChunks(lpSrcBuf);
   if (uiChunkNum >= uiChunkCount) return -1;

   //
   // Get the length of the chunk.
   //
   uiChunkLen = PAL_MKFGetSizeOfChunk(lpSrcBuf, uiChunkNum);

   if (!uiChunkLen) return -1;
   if (uiChunkLen > uiBufferSize) return -2;

   memcpy(lpDestBuf, lpSrcBuf + SDL_SwapLE32(((UINT*)lpSrcBuf)[uiChunkNum + 1]), uiBufferSize);

   return uiChunkCount;
}

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

INT
PAL_MKFGetDecompressedSize(
   BOOL           fIsWIN95,
   UINT           uiChunkNum,
   FILE          *fp
)
/*++
  Purpose:

    Get the decompressed size of a compressed chunk in an MKF archive.

  Parameters:

    [IN]  uiChunkNum - the number of the chunk in the MKF archive.

    [IN]  fp - pointer to the fopen'ed MKF file.

  Return value:

    Integer value which indicates the size of the chunk.
    -1 if the chunk does not exist.

--*/
{
   DWORD         buf[2];
   UINT          uiOffset;
   UINT          uiChunkCount;

   if (fp == NULL)
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
   uiOffset = SDL_SwapLE32(uiOffset);

   //
   // Read the header.
   //
   fseek(fp, uiOffset, SEEK_SET);
   if (fIsWIN95)
   {
      PAL_fread(buf, sizeof(DWORD), 1, fp);
      buf[0] = SDL_SwapLE32(buf[0]);

      return (INT)buf[0];
   }
   else
   {
      PAL_fread(buf, sizeof(DWORD), 2, fp);
      buf[0] = SDL_SwapLE32(buf[0]);
      buf[1] = SDL_SwapLE32(buf[1]);

      return (buf[0] != 0x315f4a59) ? -1 : (INT)buf[1];
   }
}

INT
PAL_MKFDecompressChunk(
   LPBYTE         lpBuffer,
   UINT           uiBufferSize,
   UINT           uiChunkNum,
   FILE          *fp
)
/*++
  Purpose:

    Decompress a compressed chunk from an MKF archive into lpBuffer.

  Parameters:

    [OUT] lpBuffer - pointer to the destination buffer.

    [IN]  uiBufferSize - size of the destination buffer.

    [IN]  uiChunkNum - the number of the chunk in the MKF archive to read.

    [IN]  fp - pointer to the fopen'ed MKF file.

  Return value:

    Integer value which indicates the size of the chunk.
    -1 if there are error in parameters, or buffer size is not enough.
    -3 if cannot allocate memory for decompression.

--*/
{
   LPBYTE          buf;
   int             len;

   len = PAL_MKFGetChunkSize(uiChunkNum, fp);

   if (len <= 0)
   {
      return len;
   }

   buf = (LPBYTE)malloc(len);
   if (buf == NULL)
   {
      return -3;
   }

   PAL_MKFReadChunk(buf, len, uiChunkNum, fp);

   len = Decompress(buf, lpBuffer, uiBufferSize);
   free(buf);

   return len;
}

