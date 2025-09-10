#include "LandscapeEdModeHelper.h"

#if WITH_EDITOR

#include "LandscapeEdMode.h"
#include "EditorModeManager.h"
#include "EditorModes.h"
#include "LandscapeBlueprintBrush.h"
#include "LandscapeEditLayer.h"
#include "LandscapeEditorObject.h"

#define LOCTEXT_NAMESPACE "LandscapeEditor.Tools"

void ULandscapeEdModeHelper::SetLandscapeMaterial(UMaterialInterface* _material)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (!landscapeEdMode)
		return;
	
	ALandscape* landscape = landscapeEdMode->GetLandscape();
	if (!landscape)
		return;
	
	FProperty* MaterialProperty = FindFProperty<FProperty>(ALandscapeProxy::StaticClass(), "LandscapeMaterial");
	landscape->LandscapeMaterial = _material;
	FPropertyChangedEvent PropertyChangedEvent(MaterialProperty);
	landscape->PostEditChangeProperty(PropertyChangedEvent);
}

void ULandscapeEdModeHelper::ChangeToSelectedTool(FName _toolName)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
		landscapeEdMode->SetCurrentTool(_toolName);
}

ULandscapeEditorObject* ULandscapeEdModeHelper::GetLandscapeEditorUI()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (!landscapeEdMode)
		return nullptr;

	return landscapeEdMode->UISettings;
}

float ULandscapeEdModeHelper::GetToolStrength()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (!landscapeEdMode)
		return 0.f;

	return landscapeEdMode->UISettings->GetCurrentToolStrength();
}

float ULandscapeEdModeHelper::GetBrushSize()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (!landscapeEdMode)
		return 0.f;

	return landscapeEdMode->UISettings->GetCurrentToolBrushRadius();
}

float ULandscapeEdModeHelper::GetTextureRotation()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (!landscapeEdMode)
		return 0.f;

	return landscapeEdMode->UISettings->AlphaBrushRotation;
}

void ULandscapeEdModeHelper::ChangeToolStrength(float _value)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
		landscapeEdMode->UISettings->SetCurrentToolStrength(_value);
}

void ULandscapeEdModeHelper::ChangeBrushSize(float _value)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
		landscapeEdMode->UISettings->SetCurrentToolBrushRadius(_value);
}

void ULandscapeEdModeHelper::ChangeBrushSet(FName _brushSetName)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
		landscapeEdMode->SetCurrentBrushSet(_brushSetName);
}

int32 ULandscapeEdModeHelper::GetTextureChannel()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (!landscapeEdMode)
		return 0;

	int32 enumIndex = static_cast<int32>(landscapeEdMode->UISettings->AlphaTextureChannel);
	return enumIndex;
}

bool ULandscapeEdModeHelper::GetIsAutoRotate()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (!landscapeEdMode)
		return true;

	return landscapeEdMode->UISettings->bAlphaBrushAutoRotate;
}

void ULandscapeEdModeHelper::SetTexture(UTexture2D* _texture)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
	{
		ELandscapeTextureColorChannel alphaTextureChannel = landscapeEdMode->UISettings->AlphaTextureChannel;
		landscapeEdMode->UISettings->AlphaTexture = _texture;
		/* YSJ :
			If you change the code above to the code below,
			you can load source data of the texture and Set it accurately to the horizontal and vertical sizes.
			But there is a brief delay.
		*/
		//landscapeEdMode->UISettings->SetAlphaTexture(_texture, alphaTextureChannel);
	}
}

void ULandscapeEdModeHelper::SetTextureChannel(int32 _enumIndex)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
		landscapeEdMode->UISettings->AlphaTextureChannel = static_cast<ELandscapeTextureColorChannel>(_enumIndex);
}

void ULandscapeEdModeHelper::SetAutoRotate(bool _isChecked)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
		landscapeEdMode->UISettings->bAlphaBrushAutoRotate = _isChecked;
}

void ULandscapeEdModeHelper::ChangeTextureRotation(float _value)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
		landscapeEdMode->UISettings->AlphaBrushRotation = _value;
}

void ULandscapeEdModeHelper::ChangeToBlueprintBrush()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
		landscapeEdMode->SetCurrentTool("BlueprintBrush");
}

void ULandscapeEdModeHelper::SetBluePrintBrushActor(TSubclassOf<ALandscapeBlueprintBrush> _bpBrushClass)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
	{
		TSubclassOf<ALandscapeBlueprintBrushBase> brushBaseClass = _bpBrushClass;
		landscapeEdMode->UISettings->BlueprintBrush = brushBaseClass;
	}
}

void ULandscapeEdModeHelper::UpdateLandscape()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (!landscapeEdMode)
		return;
	
	ALandscape* Landscape = landscapeEdMode->GetLandscape();
	if (!Landscape)
		return;
	
	Landscape->RequestLayersContentUpdateForceAll();
}

void ULandscapeEdModeHelper::MergeLayersToBase()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (!landscapeEdMode)
		return;
	
	ALandscape* Landscape = landscapeEdMode->GetLandscape();
	if (!Landscape)
		return;
	
	Landscape->CopyOldDataToCustomLayer();
	landscapeEdMode->OnCanHaveLayersContentChanged();
}

FReply ULandscapeEdModeHelper::DeformLandscapeForAllSplines()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
	{
		FScopedTransaction Transaction(LOCTEXT("LandscapeSpline_ApplyAllSplines", "Apply All Splines to Landscape"));
		landscapeEdMode->UpdateLandscapeSplines(false);
	}

	return FReply::Handled();
}

FReply ULandscapeEdModeHelper::DeformLandscapeForSelectedSplines()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode)
	{
		FScopedTransaction Transaction(LOCTEXT("LandscapeSpline_ApplySelectedSplines", "Apply Selected Splines to Landscape"));
		landscapeEdMode->UpdateLandscapeSplines(true);
	}

	return FReply::Handled();
}

FReply ULandscapeEdModeHelper::SelectAllConnectedControlPoints()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode && landscapeEdMode->CurrentToolTarget.LandscapeInfo.IsValid())
		landscapeEdMode->SelectAllConnectedSplineControlPoints();

	return FReply::Handled();
}

FReply ULandscapeEdModeHelper::SelectAllConnectedSegments()
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode && landscapeEdMode->CurrentToolTarget.LandscapeInfo.IsValid())
		landscapeEdMode->SelectAllConnectedSplineSegments();

	return FReply::Handled();
}

void ULandscapeEdModeHelper::SetUseAutoRotateControlPoint(bool _isChecked)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode && landscapeEdMode->CurrentToolTarget.LandscapeInfo.IsValid())
		landscapeEdMode->SetbUseAutoRotateOnJoin(_isChecked);
}

void ULandscapeEdModeHelper::SetAlwaysAutoRotateForward(bool _isChecked)
{
	FEdModeLandscape* landscapeEdMode = static_cast<FEdModeLandscape*>(GLevelEditorModeTools().GetActiveMode(FBuiltinEditorModes::EM_Landscape));
	if (landscapeEdMode && landscapeEdMode->CurrentToolTarget.LandscapeInfo.IsValid())
		landscapeEdMode->SetbAlwaysRotateForward(_isChecked);
}

#endif
