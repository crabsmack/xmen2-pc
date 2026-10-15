/* Magneto_FrameInBank - MAGNETO: frame number frame as a frame of its bank (Magneto_FrameBank).
 *
 * frame: Magneto's frame number ($FF = none).
 * Returns $FF for $FF, else frame - $60 from $60 up, frame - $40 from $40 up, frame below. */
#include "xmen2.h"

short Magneto_FrameInBank(register short frame)
{
	if (frame == 0xFF)
		return 0xFF;
	else if (frame >= 0x60)
		return frame - 0x60;
	else if (frame >= 0x40)
		return frame - 0x40;
	else
		return frame;
}
