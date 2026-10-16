/* ExodusBlast_StateLaunch - EXODUSBLAST state: start the blast animation (Tbl_ExodusBlastAnim_2),
 * clear flags6 $4000, and from the next update fly by kind: 1 right (ExodusBlast_LaunchRight),
 * 2 down-left, 3 down-right, 4 down, 5 and 6 the swoop drops, 7 left.
 *
 * self: the blast. Returns self->flags & 8; no value for another kind.
 * The cases are the fragments ExodusBlast_StateLaunch_part1 (kinds 1, 2), _part2 (3), _part3
 * (4-7); _part4 is the shared exit. */
#include "xmen2.h"

short ExodusBlast_StateLaunch(register ExodusBlast *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_ExodusBlastAnim_2));
	self->flags6 &= ~0x4000;
	switch (self->kind) {
	case 1:		/* ExodusBlast_StateLaunch_part1 */
		SET_STATE(self, ExodusBlast_LaunchRight);
		return self->flags & 8;
	case 2:
		SET_STATE(self, ExodusBlast_LaunchDownLeft);
		return self->flags & 8;
	case 3:		/* ExodusBlast_StateLaunch_part2 */
		SET_STATE(self, ExodusBlast_LaunchDownRight);
		return self->flags & 8;
	case 4:		/* ExodusBlast_StateLaunch_part3 */
		SET_STATE(self, ExodusBlast_LaunchDown);
		return self->flags & 8;
	case 5:
		SET_STATE(self, ExodusBlast_LaunchDropA);
		return self->flags & 8;
	case 6:
		SET_STATE(self, ExodusBlast_LaunchDropB);
		return self->flags & 8;
	case 7:
		SET_STATE(self, ExodusBlast_LaunchLeft);
		return self->flags & 8;
	}
	/* ExodusBlast_StateLaunch_part4 */
	NO_RETURN_VALUE
}
