/* Spawn_InitRecords - build the level's spawn records: one 14-byte SpawnRec per placement entry,
 * which the entry's class fills (InitSpawnRec, class slot +$0A), and start the spawn scan.
 *
 * The records (g_SpawnRecCount of them, from Mem_StackAlloc with an empty tag) go to
 * g_SpawnRecs; record i gets the class g_PlacementList[i].classIndex selects in g_ClassList.
 * The scan (Level_FrameLoop) starts from the end of the records, 5 records a frame (the 10 just
 * before is overwritten); a level without records gets Spawn_Nop as its spawn pass. */
#include "xmen2.h"

void Spawn_InitRecords(void)
{
	register long i;
	register SpawnRec *rec;
	register Placement *pl;

	g_SpawnRecs = (SpawnRec *)Mem_StackAlloc(g_SpawnRecCount * 14, STR(Str_Empty_15FA2A, ""));
	for (i = 0; i < g_SpawnRecCount; i++) {
		rec = &g_SpawnRecs[i];
		pl = &g_PlacementList[i];
		rec->cls = ((ClassList *)g_ClassList)->cls[pl->classIndex];
		(*rec->cls->method[1])(rec, pl);		/* InitSpawnRec */
	}
	g_SpawnScanLeft = 0;
	g_SpawnScanPtr = g_SpawnRecs;
	g_SpawnRecEnd = g_SpawnRecs + g_SpawnRecCount;
	g_SpawnScanPerFrame = 10;
	g_SpawnScanPerFrame = 5;
	if (g_SpawnRecCount > 0)
		g_SpawnPassHook = 0;
	else
		g_SpawnPassHook = Spawn_Nop;
}
