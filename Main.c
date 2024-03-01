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

#include "main.h"
#include <setjmp.h>

static jmp_buf g_exit_jmp_buf;
static int g_exit_code = 0;

VOID
PAL_Shutdown(
   int               exit_code
)
/*++
  Purpose:

    Free everything needed by the game.

  Parameters:

    exit_code -  The exit code return to OS.

  Return value:

    None.

--*/
{
   PAL_FreeGlobals();

   g_exit_code = exit_code;
   //longjmp(g_exit_jmp_buf, 1);
}

VOID
PAL_CmdError(
   FILETYPE          ftFileType,
   ERRORTYPE         etErrorType
)
/*++
  Purpose:

    Prompt users for errors in commands or parameters.

  Parameters:

    [IN]  ftFileType - the file type where the error occurred.

    [IN]  etErrorType - error Type.

  Return value:

    None.

--*/
{
   LPSTR title;
   LPSTR message;

   switch (ftFileType)
   {
   case FILETYPE_MKF:
      title = "MKF command: ";
      break;

   case FILETYPE_YJ_1:
      title = "YJ_1 command: ";
      break;

   case FILETYPE_YJ_2:
      title = "YJ_2 command: ";
      break;

   case FILETYPE_SMKF:
      title = "SMKF command: ";
      break;

   case FILETYPE_RLE:
      title = "RLE command: ";
      break;

   case FILETYPE_BIN:
      title = "BIN command: ";
      break;

   case FILETYPE_ACT:
      title = "ACT command: ";
      break;

   case FILETYPE_COLORS:
      title = "RNG command: ";
      break;

   default:
      title = "PalCMD Error: ";
      break;
   }

   switch (etErrorType)
   {
   case ERRORTYPE_INVALIDCMD:
      message = "Ah Oh......\n"                                                  \
                "Sorry, You seem to have entered one or more invalid command~\n";
      break;

   case ERRORTYPE_MULTIPLECMD:
      message = "Ah Oh......\n"                                                 \
                "Sorry, You seems to have entered multiple commands at once~\n";
      break;

   case ERRORTYPE_INVALIDPARAM:
      message = "Ah Oh......\n"                                                     \
                "Sorry, You seem to have entered one or more invalid parameters~\n";
      break;

   case ERRORTYPE_MISSPARAM:
      message = "Ah Oh......\n"                                                              \
                "Sorry, It seems that you have not provided all the necessary parameters~\n";
      break;

   case ERRORTYPE_NOTSUPPORTED:
      message = "Ah Oh......\n"                                                                                              \
                "Sorry, It seems that you are performing an operation that is not supported by the program......\n"          \
                "\n"                                                                                                         \
                "    1.  Currently, it is not supported to unpack all sub files and grandson files at the same time......\n";
      break;

   default:
      message = "What Fuck......?\n"                      \
                "Sorry, Unknown error occurred ???!!!~\n";
      break;

   }

   MessageBox(NULL, message, title, MB_OK | MB_ICONINFORMATION);

   PAL_Shutdown(255);
}

VOID
PAL_CmdHelp(
   FILETYPE          ftFileType
)
/*++
  Purpose:

    Prompt user how to use this program.

  Parameters:

    [IN]  ftFileType - file type.

  Return value:

    None.

--*/
{
   LPCSTR title   = "All command: ";
   LPCSTR message = "Please pass in at least one parameter to the program.\n"             \
                    "You can use the following command: \n"                               \
                    "\n"                                                                  \
                    "(\"Example: PMD.EXE -DE MKF YJ_1 SMKF RLE -IN H:/Pal/SSS.MKF -ID 4 -SID 0 -OUT H:/Pal/\")\n"     \
                    "\n"                                                                  \
                    "For more help, please refer to README TXT.\n";

   MessageBox(NULL, message, title, MB_OK | MB_ICONINFORMATION);
}

int
main(
   int      argc,
   char    *argv[]
)
/*++
  Purpose:

    Program entry.

  Parameters:

    argc - Number of arguments.

    argv - Array of arguments.

  Return value:

    Integer value.

--*/
{
   INT      i, iExist = -3, iSubID = -2, iGrandSubID = -2, iThisSubID = 0, iThisGrandSubID = 0;
   UINT     uiBufferSize;
   FILETYPE ftFileType          = FILETYPE_ALL;
   FILETYPE ftFileTypeReal      = FILETYPE_ALL;
   BOOL     fIsDeBin            = TRUE;
   LPSTR    lpszSourcePath      = NULL;
   LPSTR    lpszDestinationPath = NULL;
   LPSTR    lpszSubFormat       = NULL;

   if (!(argc - 1))
   {
      //
      // If the user does not pass any commands or parameters,
      // the user is notified and the program exits.
      //
      PAL_CmdHelp(FILETYPE_ALL);
      return;
   }

   //
   // Read necessary parameters......
   //
   for (i = 1; i < argc - 1; i++)
   {
      //
      // Translate the next parameter......
      //
      if (i > argc - 1) PAL_CmdError(ftFileType, ERRORTYPE_MISSPARAM);
      UTIL_StrToUpper(argv[i]);

      //
      // Translate the next parameter value......
      //
      i++;
      if (i > argc - 1) PAL_CmdError(ftFileType, ERRORTYPE_MISSPARAM);
      UTIL_StrToUpper(argv[i]);

      i--;

      if (  !strcmp(argv[i], "-H")
         || !strcmp(argv[i], "--HELP"))
      {
         PAL_CmdHelp(ftFileType); // Ths user need help.
         return;
      }
      else if (!strcmp(argv[i], "-DE")
            || !strcmp(argv[i], "--DECODE"))
      {
         //
         // DECODE command:
         //
         fIsDeBin = TRUE;
      }
      else if (!strcmp(argv[i], "-EN")
            || !strcmp(argv[i], "--ENCODE"))
      {
         //
         // ENCODE command:
         //
         fIsDeBin = FALSE;
      }
      else if (!strcmp(argv[i], "-IN")
            || !strcmp(argv[i], "--SRCPATH"))
      {
         i++;
         lpszSourcePath = argv[i]; // Set resource path.
      }
      else if (!strcmp(argv[i], "-ID")
            || !strcmp(argv[i], "--SUBFILEID"))
      {
         i++;
         iSubID = UTIL_StrToLongInt(argv[i]); // Set the sub file number of the resource to be extracted.
         if (iSubID < -1) PAL_CmdError(ftFileType, ERRORTYPE_INVALIDPARAM);
      }
      else if (!strcmp(argv[i], "-SID")
            || !strcmp(argv[i], "--GRANDSUBFILEID"))
      {
         i++;
         iGrandSubID = UTIL_StrToLongInt(argv[i]); // Set the grand sub file number of the resource to be extracted.
         if (iGrandSubID < -1) PAL_CmdError(ftFileType, ERRORTYPE_INVALIDPARAM);
      }
      else if (!strcmp(argv[i], "-OUT")
            || !strcmp(argv[i], "--DESTDIR"))
      {
         i++;
         lpszDestinationPath = argv[i]; // Set the extraction path for sub files.
      }
   }

   if (!lpszSourcePath || !lpszDestinationPath || iSubID == -2)
   {
      //
      // Missing input/output directory parameter......
      //
      goto tagMissing;
   }
   else
   {
      //
      // Open resource file.
      //
      gpGlobals->fpSource     = UTIL_OpenRequiredFile(lpszSourcePath);
      gpGlobals->lSrcFileSize = UTIL_GetFileSize(gpGlobals->fpSource);
      gpGlobals->lpFileBuf    = UTIL_malloc(gpGlobals->lSrcFileSize);

      if (fread(gpGlobals->lpFileBuf, sizeof(BYTE), gpGlobals->lSrcFileSize, gpGlobals->fpSource) < gpGlobals->lSrcFileSize) goto tagInvalid;

      if (iSubID      > -1) iThisSubID      = iSubID;
      if (iGrandSubID > -1) iThisGrandSubID = iGrandSubID;
      if (iSubID == -1 && iGrandSubID == -1) goto tagNotSupported; // Currently, it is not supported to unpack all sub files and grandson files at the same time......
   }

   //
   // Read unpacking/packaging steps......
   //
   do
   {
      gpGlobals->lpBuffer = &gpGlobals->lpFileBuf;

      for (i = 1; i < argc - 1; i++)
      {
         //
         // Convert parameter text to size for easy parameter resolution.
         //
         UTIL_StrToUpper(argv[i]);

         if (!strcmp(argv[i], "MKF"))
         {
            //
            // MKF command:
            //
            ftFileType = FILETYPE_MKF;

            if (iSubID > -1)
            {
               //
               // Extract once.
               // 
               if (iSubID >= PAL_MKFGetNumChunks(*gpGlobals->lpBuffer)) goto tagInvalid; // Check whether the ID is out of range.

               //
               // Get the size of the chunk.
               //
               uiBufferSize = PAL_MKFGetSizeOfChunk(*gpGlobals->lpBuffer, iThisSubID); // Get the total number of chunks.
            }
            else
            {
               //
               // Extract all.
               //
               if (iSubID >= PAL_MKFGetNumChunks(*gpGlobals->lpBuffer)) break; // Check whether the ID is out of range.
               
               //
               // Get the size of the chunk.
               //
               iExist = PAL_MKFGetSizeOfChunk(*gpGlobals->lpBuffer, iThisSubID);

               //
               // Check if the returned size is correct, -1 indicates that all files have been extracted.
               //
               if (iExist < 0) goto tagEnd;
               else uiBufferSize = iExist;
            }

            //
            // Allocate memory.
            //
            gpGlobals->lpMKFBuf = *gpGlobals->lpBuffer + SDL_SwapLE32(((UINT*)*gpGlobals->lpBuffer)[iThisSubID + 1]);

            //
            // Rebind the actual target memory.
            //
            gpGlobals->lpBuffer = &gpGlobals->lpMKFBuf;

            lpszSubFormat = "sMKF"; // Determine sub file format.
         }
         else if (!strcmp(argv[i], "YJ_1"))
         {
            //
            // YJ_1 command:
            //
            ftFileType = FILETYPE_YJ_1;

            //
            // Gets the actual size of the file after it is unpacked.
            //
            uiBufferSize = SDL_SwapLE32(((DWORD*)*gpGlobals->lpBuffer)[1]);
            gpGlobals->wSMKFSize = uiBufferSize;

            if (gpGlobals->lpYJ_1Buf == NULL)
            {
               //
               // Is obtaining the package signature legal.
               //
               if (SDL_SwapLE32(((DWORD*)*gpGlobals->lpBuffer)[0]) != 0x315f4a59) goto tagInvalid;

               //
               // Allocate memory.
               //
               gpGlobals->lpYJ_1Buf = (LPBYTE)UTIL_malloc(uiBufferSize);

               //
               // If unboxing fails, it is possible that the user provided incorrect or invalid parameters.
               //
               if (YJ1_Decompress(*gpGlobals->lpBuffer, gpGlobals->lpYJ_1Buf, uiBufferSize) == -1) goto tagInvalid;
            }

            //
            // Rebind the actual target memory.
            //
            gpGlobals->lpBuffer = &gpGlobals->lpYJ_1Buf;

            lpszSubFormat = "sYJ_1"; // Determine sub file format.
         }
         else if (!strcmp(argv[i], "YJ_2"))
         {
            //
            // YJ_2 command:
            //
            ftFileType = FILETYPE_YJ_2;

            //
            // Gets the actual size of the file after it is unpacked.
            //
            uiBufferSize = SDL_SwapLE32(((DWORD*)*gpGlobals->lpBuffer)[0]);
            gpGlobals->wSMKFSize = uiBufferSize;

            if (gpGlobals->lpYJ_2Buf == NULL)
            {
               //
               // Allocate memory.
               //
               gpGlobals->lpYJ_2Buf = (LPBYTE)UTIL_malloc(uiBufferSize);

               //
               // If unboxing fails, it is possible that the user provided incorrect or invalid parameters.
               //
               if (YJ2_Decompress(*gpGlobals->lpBuffer, gpGlobals->lpYJ_2Buf, uiBufferSize) == -1) goto tagInvalid;
            }

            //
            // Rebind the actual target memory.
            //
            gpGlobals->lpBuffer = &gpGlobals->lpYJ_2Buf;

            lpszSubFormat = "sYJ_2"; // Determine sub file format.
         }
         else if (!strcmp(argv[i], "SMKF"))
         {
            //
            // SMKF command:
            //
            ftFileType = FILETYPE_SMKF;

            if (iGrandSubID < -1) goto tagMissing;

            //
            // Start extracting sub files of MKF.
            //
            if (iGrandSubID > -1)
            {
               //
               // Extract once.
               //
               if (iGrandSubID >= (PAL_SpriteGetNumFrames(*gpGlobals->lpBuffer) + 1)) goto tagInvalid; // Check whether the SID is out of range.

               //
               // Get the size of the chunk.
               //
               uiBufferSize = PAL_SpriteGetSizeOfFrame(*gpGlobals->lpBuffer, iThisGrandSubID, gpGlobals->wSMKFSize); // Get the total number of chunks.
            }
            else
            {
               //
               // Extract all.
               //
               if (iGrandSubID >= (PAL_SpriteGetNumFrames(*gpGlobals->lpBuffer) + 1)) break; // Check whether the SID is out of range.
               
               //
               // Get the size of the chunk.
               //
               iExist = PAL_SpriteGetSizeOfFrame(*gpGlobals->lpBuffer, iThisGrandSubID, uiBufferSize);

               //
               // Check if the returned size is correct, -1 indicates that all files have been extracted.
               //
               if (iExist < 0) goto tagEnd;
               else uiBufferSize = iExist;
            }

            //
            // Start extracting sub files of SMKF.
            //
            gpGlobals->lpSMKFBuf = PAL_SpriteGetFrame(*gpGlobals->lpBuffer, iThisGrandSubID);

            gpGlobals->lpBuffer = &gpGlobals->lpSMKFBuf;

            lpszSubFormat = "sSMKF"; // Determine sub file format.
         }
         else if (!strcmp(argv[i], "RLE"))
         {
            //
            // RLE command:
            //
            ftFileType = FILETYPE_RLE;

            *gpGlobals->lpBuffer = gpGlobals->lpRLEBuf;

            lpszSubFormat = "sRLE"; // Determine sub file format.
         }
         else
         {
            //goto tagInvalid; // The user entered an invalid command......
         }
      }

      if (!ftFileTypeReal) ftFileTypeReal = ftFileType;

      //
      // Save extracted data to a file......
      //
      UTIL_SaveDataFile(lpszDestinationPath, PAL_va(1, "%d-%d.%s", iThisSubID, iThisGrandSubID, lpszSubFormat), *gpGlobals->lpBuffer, uiBufferSize);

      if (iSubID      == -1) iThisSubID++;
      if (iGrandSubID == -1) iThisGrandSubID++;
   } while (iExist > -1);

   while (FALSE)
   {
tagInvalid:
      //
      // The user entered an invalid command......
      //
      PAL_CmdError(ftFileType, ERRORTYPE_INVALIDPARAM);
      PAL_CmdHelp(ftFileType);
      break;

tagMissing:
      //
      // Missing important parameter......
      //
      PAL_CmdError(ftFileType, ERRORTYPE_MISSPARAM);
      PAL_CmdHelp(ftFileType);
      break;

tagNotSupported:
      //
      // Sorry, the program currently does not support this operation......
      //
      PAL_CmdError(ftFileType, ERRORTYPE_NOTSUPPORTED);
      break;

   }

tagEnd:
   //
   // End program.
   //
   PAL_Shutdown(0);

   return 0;
}
