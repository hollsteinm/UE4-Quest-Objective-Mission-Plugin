//// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
//
//#pragma once
//TODO: Implement
//
//#include "Toolkits/AssetEditorToolkit.h"
//#include "Toolkits/AssetEditorManager.h"
//#include "Editor/EditorWidgets/Public/ITransportControl.h"
//#include "MissionData.h"
//
//class SMissionAssetEditor;
//
////////////////////////////////////////////////////////////////////////////
//// FFlipbookEditor
//
//class FMissionAssetEditor : public FAssetEditorToolkit, public FGCObject
//{
//public:
//	FMissionAssetEditor();
//
//	// IToolkit interface
//	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
//	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
//	// End of IToolkit interface
//
//	// FAssetEditorToolkit
//	virtual FName GetToolkitFName() const override;
//	virtual FText GetBaseToolkitName() const override;
//	virtual FText GetToolkitName() const override;
//	virtual FText GetToolkitToolTipText() const override;
//	virtual FLinearColor GetWorldCentricTabColorScale() const override;
//	virtual FString GetWorldCentricTabPrefix() const override;
//	virtual FString GetDocumentationLink() const override;
//	// End of FAssetEditorToolkit
//
//	// FSerializableObject interface
//	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
//	// End of FSerializableObject interface
//};