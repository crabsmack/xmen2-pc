/* Magneto_FrameBank - MAGNETO: the sprite definition (bank) that holds frame number frame.
 *
 * frame: Magneto's frame number: 0-$3F are in the first bank (Art_EvilMagnetoSpr), $40-$5F in
 *        the second (Art_MagnetoBossSpr2), $60 and up in the third (Art_MagnetoBossSpr3); the
 *        EVILMAGNETO clone and the MAGNETOBOSS share them.
 * Returns the bank's sprite definition. Magneto_FrameInBank gives the frame inside it. */
#include "xmen2.h"

void *Magneto_FrameBank(register short frame)
{
	if (frame >= 0x60)
		return CODEPTR(Art_MagnetoBossSpr3);
	else if (frame >= 0x40)
		return CODEPTR(Art_MagnetoBossSpr2);
	else
		return CODEPTR(Art_EvilMagnetoSpr);
}
