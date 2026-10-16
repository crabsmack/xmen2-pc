/* BaniMazaA1_InitTablePtrs - point four scene pointers at the Moving Blocks' colour sequences
 * (Tbl_BaniMazaA1ColourSeq1-4, into g_BmA1ColourSeqs[0..3] in reverse). Nothing found reads
 * them. */
#include "xmen2.h"

#define g_BmA1ColourSeq	g_BmA1ColourSeqs

void BaniMazaA1_InitTablePtrs(void)
{
	g_BmA1ColourSeq[3] = CODEPTR(Tbl_BaniMazaA1ColourSeq1);
	g_BmA1ColourSeq[2] = CODEPTR(Tbl_BaniMazaA1ColourSeq2);
	g_BmA1ColourSeq[1] = CODEPTR(Tbl_BaniMazaA1ColourSeq3);
	g_BmA1ColourSeq[0] = CODEPTR(Tbl_BaniMazaA1ColourSeq4);
}
