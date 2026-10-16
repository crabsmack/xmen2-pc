/* Map_RefreshBlock - redraw FG map block (col, row) on screen (Map_RedrawBlock), after its tiles
 * changed. */
#include "xmen2.h"

void Map_RefreshBlock(short col, short row)
{
	Map_RedrawBlock(col, row, 0);
}
