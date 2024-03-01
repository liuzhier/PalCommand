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
// Portions based on PalLibrary by Lou Yihua <louyihua@21cn.com>.
// Copyright (c) 2006-2007, Lou Yihua.
//
// Ported to C from C++ and modified for compatibility with Big-Endian
// by Wei Mingzhi <whistler_wmz@users.sf.net>.
//

#include "Common.h"

typedef struct _YJ1_TreeNode
{
	unsigned char   value;
	unsigned char   leaf;
	unsigned short  level;
	unsigned int    weight;

	struct _YJ1_TreeNode *parent;
	struct _YJ1_TreeNode *left;
	struct _YJ1_TreeNode *right;
} YJ1_TreeNode;

typedef struct _YJ1_TreeNodeList
{
	YJ1_TreeNode *node;
	struct _YJ1_TreeNodeList *next;
} YJ1_TreeNodeList;

typedef struct _YJ_1_FILEHEADER
{
	unsigned int   Signature;          // 'YJ_1'
	unsigned int   UncompressedLength; // size before compression
	unsigned int   CompressedLength;   // size after compression
	unsigned short BlockCount;       // number of blocks
	unsigned char Unknown;
	unsigned char HuffmanTreeLength; // length of huffman tree
} YJ_1_FILEHEADER, *PYJ_1_FILEHEADER;

typedef struct _YJ_1_BLOCKHEADER
{
	unsigned short UncompressedLength; // maximum 0x4000
	unsigned short CompressedLength;   // including the header
	unsigned short LZSSRepeatTable[4];
	unsigned char LZSSOffsetCodeLengthTable[4];
	unsigned char LZSSRepeatCodeLengthTable[3];
	unsigned char CodeCountCodeLengthTable[3];
	unsigned char CodeCountTable[2];
} YJ_1_BLOCKHEADER, *PYJ_1_BLOCKHEADER;

static unsigned int
	yj1_get_bits(
	const void *src,
	unsigned int *bitptr,
	unsigned int count
	)
{
	unsigned char *temp = ((unsigned char *)src) + ((*bitptr >> 4) << 1);
	unsigned int bptr = *bitptr & 0xf;
	unsigned short mask;
	*bitptr += count;
	if (count > 16 - bptr)
	{
		count = count + bptr - 16;
		mask = 0xffff >> bptr;
		return (((temp[0] | (temp[1] << 8)) & mask) << count) | ((temp[2] | (temp[3] << 8)) >> (16 - count));
	}
	else
		return (((unsigned short)((temp[0] | (temp[1] << 8)) << bptr)) >> (16 - count));
}

static unsigned short
	yj1_get_loop(
	const void *src,
	unsigned int *bitptr,
	PYJ_1_BLOCKHEADER header
	)
{
	if (yj1_get_bits(src, bitptr, 1))
		return header->CodeCountTable[0];
	else
	{
		unsigned int temp = yj1_get_bits(src, bitptr, 2);
		if (temp)
			return yj1_get_bits(src, bitptr, header->CodeCountCodeLengthTable[temp - 1]);
		else
			return header->CodeCountTable[1];
	}
}

static unsigned short
	yj1_get_count(
	const void *src,
	unsigned int *bitptr,
	PYJ_1_BLOCKHEADER header
	)
{
	unsigned short temp;
	if ((temp = yj1_get_bits(src, bitptr, 2)) != 0)
	{
		if (yj1_get_bits(src, bitptr, 1))
			return yj1_get_bits(src, bitptr, header->LZSSRepeatCodeLengthTable[temp - 1]);
		else
			return SDL_SwapLE16(header->LZSSRepeatTable[temp]);
	}
	else
		return SDL_SwapLE16(header->LZSSRepeatTable[0]);
}

INT
	YJ1_Decompress(
	LPCVOID       Source,
	LPVOID        Destination,
	INT           DestSize
	)
{
	PYJ_1_FILEHEADER hdr = (PYJ_1_FILEHEADER)Source;
	unsigned char *src = (unsigned char *)Source;
	unsigned char *dest;
	unsigned int i;
	YJ1_TreeNode *root, *node;

	if (Source == NULL)
		return -1;
	if (SDL_SwapLE32(hdr->Signature) != 0x315f4a59)
		return -1;
	if (SDL_SwapLE32(hdr->UncompressedLength) > (unsigned int)DestSize)
		return -1;

	do
	{
		unsigned short tree_len = ((unsigned short)hdr->HuffmanTreeLength) * 2;
		unsigned int bitptr = 0;
		unsigned char *flag = (unsigned char *)src + 16 + tree_len;

		if ((node = root = (YJ1_TreeNode *)malloc(sizeof(YJ1_TreeNode) * (tree_len + 1))) == NULL)
			return -1;
		root[0].leaf = 0;
		root[0].value = 0;
		root[0].left = root + 1;
		root[0].right = root + 2;
		for (i = 1; i <= tree_len; i++)
		{
			root[i].leaf = !yj1_get_bits(flag, &bitptr, 1);
			root[i].value = src[15 + i];
			if (root[i].leaf)
				root[i].left = root[i].right = NULL;
			else
			{
				root[i].left = root + (root[i].value << 1) + 1;
				root[i].right = root[i].left + 1;
			}
		}
		src += 16 + tree_len + (((tree_len & 0xf) ? (tree_len >> 4) + 1 : (tree_len >> 4)) << 1);
	} while (0);

	dest = (unsigned char *)Destination;

	for (i = 0; i < SDL_SwapLE16(hdr->BlockCount); i++)
	{
		unsigned int bitptr;
		PYJ_1_BLOCKHEADER header;

		header = (PYJ_1_BLOCKHEADER)src;
		src += 4;
		if (!SDL_SwapLE16(header->CompressedLength))
		{
			unsigned short hul = SDL_SwapLE16(header->UncompressedLength);
			while (hul--)
			{
				*dest++ = *src++;
			}
			continue;
		}
		src += 20;
		bitptr = 0;
		for (;;)
		{
			unsigned short loop;
			if ((loop = yj1_get_loop(src, &bitptr, header)) == 0)
				break;

			while (loop--)
			{
				node = root;
				for (; !node->leaf;)
				{
					if (yj1_get_bits(src, &bitptr, 1))
						node = node->right;
					else
						node = node->left;
				}
				*dest++ = node->value;
			}

			if ((loop = yj1_get_loop(src, &bitptr, header)) == 0)
				break;

			while (loop--)
			{
				unsigned int pos, count;
				count = yj1_get_count(src, &bitptr, header);
				pos = yj1_get_bits(src, &bitptr, 2);
				pos = yj1_get_bits(src, &bitptr, header->LZSSOffsetCodeLengthTable[pos]);
				while (count--)
				{
					*dest = *(dest - pos);
					dest++;
				}
			}
		}
		src = ((unsigned char *)header) + SDL_SwapLE16(header->CompressedLength);
	}
	free(root);

	return SDL_SwapLE32(hdr->UncompressedLength);
}
