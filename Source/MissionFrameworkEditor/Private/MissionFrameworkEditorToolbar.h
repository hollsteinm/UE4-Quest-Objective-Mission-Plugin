#pragma once

struct FMissionFrameworkEditorToolbar
{
	static TSharedRef<class SWidget> GenerateMissionMenuContent(TSharedRef<class FUICommandList> InCommandList);

	static void OnMissionActorPicked(class AActor* Actor);
};