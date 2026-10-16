/* Apocalypse_SelectBombPattern - APOCALYPSE: point the bomb-pattern cursor at the start of bomb
 * pattern patternIndex (Tbl_ApocalypseBombPattern0-5; pattern 0 for any other value).
 *
 * self: the boss.
 * The cases are Apocalypse_SelectBombPattern_part1 (0-4), _part2 (5) and _part3 (the default,
 * then the store).
 *
 * Inline here, separate blocks in the disassembly: Apocalypse_SelectBombPattern_part2, Apocalypse_SelectBombPattern_part3.
 */
#include "xmen2.h"

void Apocalypse_SelectBombPattern(register Apocalypse *self)
{
	register ApocBombDrop *p;

	switch (self->patternIndex) {
	case 0:
		p = (ApocBombDrop *)CODEPTR(Tbl_ApocalypseBombPattern0);
		break;
	case 1:
		p = (ApocBombDrop *)CODEPTR(Tbl_ApocalypseBombPattern1);
		break;
	case 2:
		p = (ApocBombDrop *)CODEPTR(Tbl_ApocalypseBombPattern2);
		break;
	case 3:
		p = (ApocBombDrop *)CODEPTR(Tbl_ApocalypseBombPattern3);
		break;
	case 4:
		p = (ApocBombDrop *)CODEPTR(Tbl_ApocalypseBombPattern4);
		break;
	case 5:
		p = (ApocBombDrop *)CODEPTR(Tbl_ApocalypseBombPattern5);
		break;
	default:
		p = (ApocBombDrop *)CODEPTR(Tbl_ApocalypseBombPattern0);
		break;
	}
	self->pattern = p;
}
