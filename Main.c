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
   int exit_code
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
   g_exit_code = exit_code;
   longjmp(g_exit_jmp_buf, 1);
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
   LPSTR title;
   LPSTR message;

   switch (ftFileType)
   {
   case FILETYPE_ALL:
      title = "All command: ";
      message = "Please pass in at least one parameter to the program.\n"             \
                "You can use the following command: \n"                               \
                "\n"                                                                  \
                "mkf: (\"Example: mkf -s H:/Pal/SSS.MKF -i 4 -d H:/Pal/SSS4.MKF\")\n" \
                "\n"                                                                  \
                "bin: (\"Example: mkf -s H:/Pal/SSS.MKF -i 4 -d H:/Pal/SSS4.MKF\")\n" \
                "\n"                                                                  \
                "More help: (Example: \"mkf -h\" / \"mkf --help\")\n";
      break;

   case FILETYPE_MKF:
      title = "MKF command: ";
      message = "You can use the following parameters: \n"                                   \
                "\n"                                                                         \
                "mkf: (\"Example: mkf -s H:/Pal/SSS.MKF -i 4 -d H:/Pal/SSS4.MKF\")\n"        \
                "    Specifies the MKF file path: (Example: \"-s H:/Pal/SSS.MKF\")\n"        \
                "        -s / --SourcePath\n"                                                \
                "    Subfile ID, Minimum: 0, If ignored, extract all: (Example: \"-i 4\")\n" \
                "        -i / --SubMKFIndex\n"                                               \
                "    Save path for sub files: (Example: \"-d H:/Pal/SSS4.MKF\")\n"           \
                "        -d / --DestinationPath\n";
      break;

   default:
      return;
      break;
   }

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
   LPSTR    lpszSourcePath = NULL;
   INT      iSubMKFIndex = -1;
   LPSTR    lpszDestinationPath = NULL;
   FILETYPE ftFileType = FILETYPE_ALL;

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
   // Read all commands and parameters.
   //
   for (INT i = 1; i < argc - 1; i++)
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
         if (ftFileType >= FILETYPE_ALL)
         {
            ftFileType = FILETYPE_MKF;
         }
         else
         {
            //
            // The user entered a duplicate command.
            //
            PAL_CmdError(ftFileType, ERRORTYPE_INVALIDCMD);
         }
      }
      else if (i == 1)
      {
         //
         // The user entered multiple commands......
         //
         PAL_CmdHelp(ftFileType);
         return;
      }

      //
      // Read the next parameter......
      //
      i++;
      if (i >= argc - 1) PAL_CmdError(ftFileType, ERRORTYPE_MISSPARAM);
      UTIL_StrToUpper(argv[i]);

      switch (ftFileType)
      {
      case FILETYPE_MKF:
      {
         //
         // MKF command:
         //
         if (  !strcmp(argv[i], "-H")
            || !strcmp(argv[i], "--HELP"))
         {
            //
            // Ths user need help.
            //
            PAL_CmdHelp(FILETYPE_MKF);
            return;
         }
         else if (!strcmp(argv[i], "-S")
               || !strcmp(argv[i], "--SOURCEPATH"))
         {
            //
            // Set resource path.
            //
            lpszSourcePath = argv[i];
         }
         else if (!strcmp(argv[i], "-I")
               || !strcmp(argv[i], "--SUBMKFINDEX"))
         {
            //
            // Set the sub file number of the resource to be extracted.
            //
            iSubMKFIndex = UTIL_StrToLongInt(argv[i]);
            if (iSubMKFIndex == -2) PAL_CmdError(ftFileType, ERRORTYPE_INVALIDPARAM);
         }
         else if (!strcmp(argv[i], "-D")
               || !strcmp(argv[i], "--DESTINATIONPATH"))
         {
            //
            // Set the extraction path for sub files.
            //
            lpszDestinationPath = argv[i];
         }
         else
         {
            //
            // The user entered an invalid command......
            //
            PAL_CmdError(ftFileType, ERRORTYPE_INVALIDPARAM);
            PAL_CmdHelp(FILETYPE_MKF);
         }
      }
      break;

      default:
         break;

      }
   }

   //
   // All parameters have been read.
   // Start executing command.
   //
   switch (ftFileType)
   {
   case FILETYPE_MKF:
   {
      if (lpszSourcePath
         && iSubMKFIndex != -1
         && lpszDestinationPath)
      {
         //
         // Start extracting sub files of MKF.
         //
         PAL_DeMKF(lpszSourcePath, iSubMKFIndex, lpszDestinationPath);
      }
   }
   break;

   default:
      break;

   }

   return 0;
}
