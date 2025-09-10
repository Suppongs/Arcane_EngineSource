#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LandscapeEdModeHelper.generated.h"

class ULandscapeEditorObject;
class ALandscapeBlueprintBrush;

UCLASS()
class LANDSCAPEEDITOR_API ULandscapeEdModeHelper : public UObject
{
	GENERATED_BODY()

public:
	//Landscape Material Change
	static void SetLandscapeMaterial(UMaterialInterface* _material);
	
	//Landscape Tool
	static void ChangeToSelectedTool(FName _toolName);

	//Common Toolbox
	static ULandscapeEditorObject* GetLandscapeEditorUI();
	static float GetToolStrength();
	static float GetBrushSize();
	static float GetTextureRotation();
	static void ChangeToolStrength(float _value);
	static void ChangeBrushSize(float _value);

	//Sculpt Toolbox
	static void ChangeBrushSet(FName _brushSetName);

	//Image Toolbox
	static int32 GetTextureChannel();
	static bool GetIsAutoRotate();
	static void SetTexture(UTexture2D* _texture);
	static void SetTextureChannel(int32 _enumIndex);
	static void SetAutoRotate(bool _isChecked);
	static void ChangeTextureRotation(float _value);

	//BluePrint Brush
	static void ChangeToBlueprintBrush();
	static void SetBluePrintBrushActor(TSubclassOf<ALandscapeBlueprintBrush> _bpBrushClass);
	static void UpdateLandscape();
	static void MergeLayersToBase();

	//Spline Toolbox
	static FReply DeformLandscapeForAllSplines();
	static FReply DeformLandscapeForSelectedSplines();
	static FReply SelectAllConnectedControlPoints();
	static FReply SelectAllConnectedSegments();
	static void SetUseAutoRotateControlPoint(bool _isChecked);
	static void SetAlwaysAutoRotateForward(bool _isChecked);
};
