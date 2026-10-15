/* Scaler_Resume - go on with a scaling pass that yielded, with a new row budget.
 *
 * Returns 0 when the pass is done, 1 when it yields again. The 68000 reloads the row loop's
 * registers from g_ScalerRegSave, sets g_ScalerBudget from g_ScalerParams.rowsPerCall and
 * returns into the loop through g_ScalerResumePC, which only ever holds the loop's one resume
 * point (SCALER_RESUME_PC): the C goes on in Scaler_RowBlocks there (SCALER_AT_RESUME) with the
 * saved registers. Hand-written 68000. */
#include "xmen2.h"

long Scaler_Resume(void)
{
	ScalerRegs r;

	r = g_ScalerRegSave;
	g_ScalerBudget = g_ScalerParams.rowsPerCall;
	return Scaler_RowBlocks(&r, SCALER_AT_RESUME);
}
