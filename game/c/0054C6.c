/* Scene_RegisterAll - register every scene, by name and function, in g_SceneTable
 * (Scene_Register), keeping the ids in the g_Scn* globals: the Cerebro screens and the stage
 * parts. "CerebroScreen PlayerSelect" is registered 16 times; g_ScnPlayerSelect keeps the
 * last id. */
#include "xmen2.h"

void Scene_RegisterAll(void)
{
	g_ScnSiberiaB1 = Scene_Register(STR(Str_SiberiaB1Blizzard, "SiberiaB1 Blizzard"), SiberiaB1_RunScene);
	g_ScnCerebroSentinels = Scene_Register(STR(Str_CerebroScreenSentinels, "CerebroScreen Sentinels"), CerebroSentinels_RunScene);
	g_ScnSentinelsD1 = Scene_Register(STR(Str_SentinelsD1Exterior1, "SentinelsD1 Exterior_1"), SentinelsD1_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnSentinelsB1 = Scene_Register(STR(Str_SentinelsB1Exterior3, "SentinelsB1 Exterior_3"), SentinelsB1_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_005BE8, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnSentinelsA1 = Scene_Register(STR(Str_SentinelsA1CoolantCore, "SentinelsA1 CoolantCore"), SentinelsA1_RunScene);
	g_ScnSentinelsB2 = Scene_Register(STR(Str_SentinelsB2ExteriorEscape, "SentinelsB2 Exterior_Escape"), SentinelsB2_RunScene);
	g_ScnCerebroAvalon = Scene_Register(STR(Str_CerebroScreenAvalon, "CerebroScreen Avalon"), CerebroAvalon_RunScene);
	g_ScnAvalonB1 = Scene_Register(STR(Str_AvalonB1Exterior, "AvalonB1 Exterior"), AvalonB1_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_005B70, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnAvalonB2 = Scene_Register(STR(Str_AvalonB2InnerShell, "AvalonB2 InnerShell"), AvalonB2_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_005B40, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnAvalonA2 = Scene_Register(STR(Str_AvalonA2FallingRoom, "AvalonA2 Falling_Room"), AvalonA2_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_005B0E, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnAvalonA3 = Scene_Register(STR(Str_AvalonA3Exodus, "AvalonA3 Exodus"), AvalonA3_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_005AE2, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnAvalonA4 = Scene_Register(STR(Str_AvalonA4ThroneRoom, "AvalonA4 Throne_Room"), AvalonA4_RunScene);
	g_ScnCerebroBaniMaza = Scene_Register(STR(Str_CerebroScreenBaniMaza, "CerebroScreen BaniMaza"), CerebroBaniMaza_RunScene);
	g_ScnBaniMazaB2 = Scene_Register(STR(Str_BaniMazaB2Exterior, "BaniMazaB2 Exterior"), BaniMazaB2_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_005A84, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnBaniMazaA1 = Scene_Register(STR(Str_BaniMazaA1MovingBlocks, "BaniMazaA1 Moving_Blocks"), BaniMazaA1_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_005A4E, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnBaniMazaA2 = Scene_Register(STR(Str_BaniMazaA2ApocalypseBoss, "BaniMazaA2 ApocalypseBoss"), BaniMazaA2_RunScene);
	g_ScnCerebroSavage = Scene_Register(STR(Str_CerebroScreenSavage, "CerebroScreen Savage"), CerebroSavage_RunScene);
	g_ScnSavageC1 = Scene_Register(STR(Str_SavageC1Jungle1, "SavageC1 Jungle_1"), SavageC1_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_0059F0, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnSavageB1 = Scene_Register(STR(Str_SavageB1Jungle2, "SavageB1 Jungle_2"), SavageB1_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_0059C2, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnSavageA2 = Scene_Register(STR(Str_SavageA2Perimeter1, "SavageA2 Perimeter_1"), SavageA2_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_005990, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnSavageA3 = Scene_Register(STR(Str_SavageA3Perimeter2, "SavageA3 Perimeter_2"), SavageA3_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_00595E, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnSavageA4 = Scene_Register(STR(Str_SavageA4Chamber, "SavageA4 Chamber"), SavageA4_RunScene);
	g_ScnCerebroSpace = Scene_Register(STR(Str_CerebroScreenSpace, "CerebroScreen Space"), CerebroSpace_RunScene);
	g_ScnSpaceB1 = Scene_Register(STR(Str_SpaceB1MaintenanceShaft, "SpaceB1 Maintenance_Shaft"), SpaceB1_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_005902, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnSpaceA1 = Scene_Register(STR(Str_SpaceA1SpaceElevator, "SpaceA1 Space_Elevator"), SpaceA1_RunScene);
	g_ScnCerebroClone = Scene_Register(STR(Str_CerebroScreenClone, "CerebroScreen Clone"), CerebroClone_RunScene);
	g_ScnCloneA1 = Scene_Register(STR(Str_CloneA1FactoryFloor1, "CloneA1 Factory_Floor_1"), CloneA1_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_0058A2, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnCloneA2 = Scene_Register(STR(Str_CloneA2VerticalCorridor1, "CloneA2 Vertical_Corridor_1"), CloneA2_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_00586A, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnCloneA5 = Scene_Register(STR(Str_CloneA5BroodBoss, "CloneA5 BroodBoss"), CloneA5_RunScene);
	g_ScnFinalText = Scene_Register(STR(Str_CerebroScreenFinalText, "CerebroScreen FinalText"), FinalText_RunScene);
	g_ScnCredits = Scene_Register(STR(Str_CerebroScreenCredits, "CerebroScreen Credits"), Credits_RunScene);
	g_ScnFailure = Scene_Register(STR(Str_CerebroScreenFailure, "CerebroScreen Failure"), Failure_RunScene);
	g_ScnPlayerSelect = Scene_Register(STR(Str_CerebroScreenPlayerSelect_0057F8, "CerebroScreen PlayerSelect"), PlayerSelect_RunScene);
	g_ScnLogoAndDemo = Scene_Register(STR(Str_CerebroScreenLogoAndDemo, "CerebroScreen LogoAndDemo"), LogoAndDemo_RunScene);
}
