/* BaniMazaA1_InitTablePtrs - point four scene pointers at the Moving Blocks' colour sequences
 * (Tbl_BaniMazaA1ColourSeq1-4). Nothing found reads them.
 * The four pointers sit just below g_DiscBandReverse ($FFC558-$FFC567) and have no names in the
 * disassembly (-$3CA8..-$3C9C): the macro reaches them from there. */
#include "xmen2.h"

#define g_BmA1ColourSeq	(&((PTR(void) *)g_DiscBandReverse)[-4])	/* [0] $FFC558 .. [3] $FFC564 */

void BaniMazaA1_InitTablePtrs(void)
{
	g_BmA1ColourSeq[3] = CODEPTR(Tbl_BaniMazaA1ColourSeq1);
	g_BmA1ColourSeq[2] = CODEPTR(Tbl_BaniMazaA1ColourSeq2);
	g_BmA1ColourSeq[1] = CODEPTR(Tbl_BaniMazaA1ColourSeq3);
	g_BmA1ColourSeq[0] = CODEPTR(Tbl_BaniMazaA1ColourSeq4);
}
