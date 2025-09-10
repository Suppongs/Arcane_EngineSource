// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorUtilityWidget.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "DesktopPlatformModule.h"
#include "EditorDirectories.h"
#include "EditorModeManager.h"
#include "EditorModes.h"
#include "IContentBrowserSingleton.h"
#include "LandscapeEdModeHelper.h"
#include "Internationalization/Internationalization.h"
#include "Misc/AssertionMacros.h"
#include "ScopedTransaction.h"
#include "Factories/TextureFactory.h"
#include "UObject/Script.h"

/////////////////////////////////////////////////////
#define LOCTEXT_NAMESPACE "EditorUtility"

void UEditorUtilityWidget::ExecuteDefaultAction()
{
	check(bAutoRunDefaultAction);

	FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "BlutilityAction", "Blutility Action"));
	FEditorScriptExecutionGuard ScriptGuard;

	Run();
}

// YSJ : Custom Edit for landscape material change ~
void UEditorUtilityWidget::OnLandscapeMaterialChangeButtonClicked(UMaterialInterface* _material)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;
	
	ULandscapeEdModeHelper::SetLandscapeMaterial(_material);
}
// ~ Custom Edit for landscape sculpt brush selection

// YSJ : Custom Edit for landscape material change ~
void UEditorUtilityWidget::ReadyForSelectedTool(FName _toolName)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::ChangeToSelectedTool(_toolName);
}

// ~ Custom Edit for landscape sculpt brush selection

// YSJ : Custom Edit for common Toolbox ~
ULandscapeEditorObject* UEditorUtilityWidget::RequestLandscapeEditorUI()
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return nullptr;

	return ULandscapeEdModeHelper::GetLandscapeEditorUI();
}

float UEditorUtilityWidget::GetInitToolStrength()
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return 0.f;

	return ULandscapeEdModeHelper::GetToolStrength();
}

float UEditorUtilityWidget::GetInitBrushSize()
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return 0.f;

	return ULandscapeEdModeHelper::GetBrushSize();
}

float UEditorUtilityWidget::GetInitTextureRotation()
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return 0.f;

	return ULandscapeEdModeHelper::GetTextureRotation();
}

void UEditorUtilityWidget::OnToolStrengthSliderMoved(float _value)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::ChangeToolStrength(_value);
}

void UEditorUtilityWidget::OnBrushSizeSliderMoved(float _value)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::ChangeBrushSize(_value);
}

// ~ Custom Edit for common Toolbox

// YSJ : Custom Edit for sculpt Toolbox ~
void UEditorUtilityWidget::OnBasicBrushButtonClicked(FName _brushSetName)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::ChangeBrushSet(_brushSetName);
}
// ~ Custom Edit for sculpt Toolbox

// YSJ : Custom Edit for Image Toolbox ~
int32 UEditorUtilityWidget::GetInitTextureChannel()
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return 0;

	return ULandscapeEdModeHelper::GetTextureChannel();
}

bool UEditorUtilityWidget::GetInitAutoRotate()
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return true;

	return ULandscapeEdModeHelper::GetIsAutoRotate();
}

void UEditorUtilityWidget::SetImageBrushTexture(UTexture2D* _texture)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::SetTexture(_texture);
}

void UEditorUtilityWidget::OnTextureSelectionChanged(UTexture2D* _texture)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	SetImageBrushTexture(_texture);
}

void UEditorUtilityWidget::OnImportButtonClicked()
{
	IDesktopPlatform* desktopPlatform = FDesktopPlatformModule::Get();
	if (!desktopPlatform)
		return;

	TArray<FString> filePaths; //selected files paths
	FString dialogTitle = TEXT("가져올 파일 선택"); //file browser window title
	FString defaultPath = FPaths::ProjectContentDir(); //default path when file browser opened
	FString defaultFile = TEXT("");
	FString fileTypes = TEXT("All Files (*.*)|*.*|PNG Files (*.png)|*.png|FBX Files (*.fbx)|*.fbx|JPG Files (*.jpg)|*.jpg|JPEG Files (*.jpeg)|*.jpeg"); //allowed file types
	uint32 flags = 0; //dialog flag (EFileDialogFlags::None)

	bool bOpen = desktopPlatform->OpenFileDialog(
			nullptr,
			dialogTitle,
			defaultPath,
			defaultFile,
			fileTypes,
			flags,
			filePaths
		);

	if (bOpen && filePaths.Num() > 0)
	{
		FAssetToolsModule& assetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

		FContentBrowserModule& contentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		FString targetContentBrowserPath;
		TArray<FString> currentContentBrowserPaths;
		contentBrowserModule.Get().GetSelectedPathViewFolders(currentContentBrowserPaths);
		if (currentContentBrowserPaths.Num() > 0)
		{
			targetContentBrowserPath = currentContentBrowserPaths[0];
			targetContentBrowserPath = targetContentBrowserPath.RightChop(4);
			if (!targetContentBrowserPath.StartsWith(TEXT("/")))
			{
				targetContentBrowserPath = TEXT("/") + targetContentBrowserPath;
			}
		}
		else
		{
			targetContentBrowserPath = "/Game";
		}
		
		UTextureFactory* textureFactory = NewObject<UTextureFactory>(); //import option setting
		textureFactory->SuppressImportOverwriteDialog();

		assetToolsModule.Get().ImportAssets(filePaths, targetContentBrowserPath, textureFactory, true, nullptr);
	}
}

void UEditorUtilityWidget::OnTextureChannelSelectionChanged(int32 _enumIndex)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::SetTextureChannel(_enumIndex);
}

void UEditorUtilityWidget::OnAutoRotateCheckboxChanged(bool _isChecked)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::SetAutoRotate(_isChecked);
}

void UEditorUtilityWidget::OnTextureRotationSliderMoved(float _value)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::ChangeTextureRotation(_value);
}

// ~ Custom Edit for Image Toolbox

TWeakObjectPtr<UEditorUtilityWidget> UEditorUtilityWidget::EUWInstance = nullptr;

void UEditorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	EUWInstance = this;
}

UEditorUtilityWidget* UEditorUtilityWidget::GetEUWInstance()
{
	return EUWInstance.Get();
}

void UEditorUtilityWidget::AddSpawnedBlueprintBrush(ALandscapeBlueprintBrush* _blueprintBrush)
{
	SpawnedBlueprintBrushes.Add(_blueprintBrush);
}

// YSJ : Custom Edit for blueprint brush ~
void UEditorUtilityWidget::ReadyForBluePrintBrush(TSubclassOf<ALandscapeBlueprintBrush> _bpBrushClass)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;
	
	ULandscapeEdModeHelper::SetBluePrintBrushActor(_bpBrushClass);
}

void UEditorUtilityWidget::UpdateLandscape()
{
	ULandscapeEdModeHelper::UpdateLandscape();
}

void UEditorUtilityWidget::OnEditConfirmButtonClicked()
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::MergeLayersToBase();
}

void UEditorUtilityWidget::CustomUndo()
{
	GEditor->UndoTransaction();
}

// ~ Custom Edit for blueprint brush

// YSJ : Custom Edit for Spline Toolbox ~
void UEditorUtilityWidget::OnAllSplinesButtonClicked()
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::DeformLandscapeForAllSplines();
}

void UEditorUtilityWidget::OnSelectedOnlyButtonClicked()
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::DeformLandscapeForSelectedSplines();
}

void UEditorUtilityWidget::OnControlPointsButtonClicked()
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::SelectAllConnectedControlPoints();
}

void UEditorUtilityWidget::OnSegmentsButtonClicked()
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::SelectAllConnectedSegments();
}

void UEditorUtilityWidget::OnUseAutoRotateControlPointCheckboxChanged(bool _isChecked)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::SetUseAutoRotateControlPoint(_isChecked);
}

void UEditorUtilityWidget::OnUAlwaysAutoRotateForwardCheckboxChanged(bool _isChecked)
{
	if (!GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Landscape))
		return;

	ULandscapeEdModeHelper::SetAlwaysAutoRotateForward(_isChecked);
}
// ~ Custom Edit for Spline Toolbox

#undef LOCTEXT_NAMESPACE
