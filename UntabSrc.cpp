//
//
//
//


#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"
#include <GnuArg.h>

int iTABSIZE   = 3;
int bNO_BACKUP = FALSE;

PSZ MakeFileName (PSZ pszBuff, PSZ pszBaseName, PSZ pszExt)
   {
   sprintf (pszBuff, "%s.%s", pszBaseName, pszExt);
   return pszBuff;
   }


int Untab (PSZ pszFile)
   {
   FILE* fpIn;
   FILE* fpOut;
   char szTmpFile [MAX_PATH];
   char szBakFile [MAX_PATH];

   MakeFileName (szTmpFile, pszFile, "tmp");
   MakeFileName (szBakFile, pszFile, "bak");

   if ((fpIn = fopen (pszFile, "rt")) == NULL)
      return printf ("Unable to open input file %s\n", pszFile);

   if ((fpOut = fopen (szTmpFile, "wt")) == NULL)
      return printf ("Unable to open output file %s\n", szTmpFile);

   printf ("Untabbing %s ...", pszFile);

   int iCol = 0;
   int iXlations = 0;
   char c;
   while ((c = getc (fpIn)) != EOF)
      {
      if (c == '\n')
         {
         putc (c, fpOut);
         iCol = 0;
         continue;
         }
      if (c == '\t')
         {
         int iSpaces = iTABSIZE - iCol % iTABSIZE;
         for (int i=0; i<iSpaces; i++)
            putc (' ', fpOut);
         iCol += iSpaces;
         iXlations++;
         continue;
         }
      putc (c, fpOut);
      iCol++;
      }
   fclose (fpIn);
   fclose (fpOut);

   if (iXlations)
      {
      unlink (szBakFile);
      if (rename (pszFile, szBakFile))
         return printf ("Could not rename %s to %s\n", pszFile, szBakFile);
      if (rename (szTmpFile, pszFile))
         return printf ("Could not rename %s to %s\n", szTmpFile, pszFile);
      if (bNO_BACKUP)
         unlink (szBakFile);
      }
   else
      {
      unlink (szTmpFile);
      }
   printf ("(%d) translations\n", iXlations);
   return 0;
   }



int Usage (void)
   {
   printf ("UntabSrc  remove tabs from source code utility                            v0.1  %s       Info Tech Inc. cf\n\n", __DATE__);
   printf ("USAGE: UntabSrc {options} <files>\n\n");
   printf ("WHERE: <files> is a list of filenames (wildcards ok).\n");
   printf ("       {options} may be 0 or more of:\n");
   printf ("        /TabSize=# ... Tabstop ammount.  default is 3.\n");
   printf ("        /NoBackup  ... Do not leave backup files behind.\n");
   printf ("        /? ........... This help.\n\n");
   printf ("EXAMPLE:  UntabSrc *.cpp\n");
   return 0;
   }


int main(int argc, char* argv[])
   {
   if (ArgBuildBlk ("? *^TabSize% *^NoBackup"))
      return printf ("%s", ArgGetErr ());
   
   if (ArgFillBlk (argv))
      return printf ("%s", ArgGetErr ());

   int iFileCount = ArgIs (NULL);
   if (!iFileCount)
      return Usage ();

   iTABSIZE   = ArgIs ("TabSize") ? atoi (ArgGet ("TabSize", 0)) : iTABSIZE;
   bNO_BACKUP = ArgIs ("NoBackup");

   for (int i=0; i<iFileCount; i++)
      Untab (ArgGet (NULL, i));

	printf("Done.\n");
	return 0;
   }

