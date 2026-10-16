/* CloneA5_CamSetHooks - the hook CAMERA's New calls on the boss arena's camera (CloneA5_InitCamera):
 * its update is CloneA5_CamUpdate and its apply CloneA5_CamApply.
 *
 * cam: the new camera. */
#include "xmen2.h"

void CloneA5_CamSetHooks(Camera *cam)
{
	cam->update = CloneA5_CamUpdate;
	cam->apply = CloneA5_CamApply;
}
