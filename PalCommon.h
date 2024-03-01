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

#ifndef _PALCOMMON_H
#define _PALCOMMON_H

#include "Common.h"

typedef LPBYTE      LPSPRITE, LPBITMAPRLE;
typedef LPCBYTE     LPCSPRITE, LPCBITMAPRLE;

PAL_C_LINKAGE_BEGIN

WORD
PAL_SpriteGetNumFrames(
   LPCSPRITE      lpSprite
);

LPCBITMAPRLE
PAL_SpriteGetFrame(
   LPCSPRITE      lpSprite,
   INT            iFrameNum
);

INT
PAL_SpriteGetSizeOfFrame(
   LPBYTE         lpBuffer,
   WORD           wChunkNum,
   WORD           wSMKFSize
);

INT
PAL_MKFGetNumChunks(
   LPBYTE         lpBuffer
);

INT
PAL_MKFGetSizeOfChunk(
   LPBYTE         lpBuffer,
   UINT           uiChunkNum
);

INT
PAL_MKFGetChunkData(
   LPBYTE         lpSrcBuf,
   LPBYTE         lpDestBuf,
   UINT           uiBufferSize,
   UINT           uiChunkNum
);

INT
PAL_MKFGetChunkCount(
   FILE          *fp
);

INT
PAL_MKFGetChunkSize(
   UINT           uiChunkNum,
   FILE          *fp
);

INT
PAL_MKFReadChunk(
   LPBYTE         lpBuffer,
   UINT           uiBufferSize,
   UINT           uiChunkNum,
   FILE          *fp
);

INT
PAL_MKFGetDecompressedSize(
   BOOL           fIsWIN95,
   UINT           uiChunkNum,
   FILE          *fp
);

INT
PAL_MKFDecompressChunk(
   LPBYTE         lpBuffer,
   UINT           uiBufferSize,
   UINT           uiChunkNum,
   FILE          *fp
);

INT
YJ1_Decompress(
   LPCVOID        Source,
   LPVOID         Destination,
   INT            DestSize
);

INT
YJ2_Decompress(
   LPCVOID        Source,
   LPVOID         Destination,
   INT            DestSize
);

INT (*Decompress)(LPCVOID, LPVOID, INT);

PAL_C_LINKAGE_END

#endif // _PALCOMMON_H
