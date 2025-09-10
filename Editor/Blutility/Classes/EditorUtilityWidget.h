// Copyright Epic Games, Inc. All Rights Reserved.

/**
 * Widget for editor utilities
 */

#pragma once

#include "Blueprint/UserWidget.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"
#include "UObject/UObjectGlobals.h"

#include "EditorUtilityWidget.generated.h"

class ULandscapeEditorObject;
class ALandscapeBlueprintBrush;
class AActor;
class UEditorPerProjectUserSettings;
class UObject;

//YSJ : Custom Edit ~
USTRUCT(BlueprintType)
struct FBrushifyAlphaTexture
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	FString ExposedName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	TObjectPtr<UTexture2D> Texture;
};
// ~ Custom Edit


UCLASS(Abstract, meta = (ShowWorldContextPin), config = Editor)
class BLUTILITY_API UEditorUtilityWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// The default action called when the widget is invoked if bAutoRunDefaultAction=true (it is never called otherwise)
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Editor")
	void Run();

	// Run the default action
	void ExecuteDefaultAction();

	bool ShouldAlwaysReregisterWithWindowsMenu() const
	{
		return bAlwaysReregisterWithWindowsMenu;
	}

	bool ShouldAutoRunDefaultAction() const
	{
		return bAutoRunDefaultAction;
	}

	/** Returns the default desired tab display name that was specified for this widget */
	FText GetTabDisplayName() const
	{
		return TabDisplayName;
	}

	virtual bool IsEditorUtility() const override { return true; }

	//YSJ : Custom Edit

	/* Custom Edit for landscape material change */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnLandscapeMaterialChangeButtonClicked(UMaterialInterface* _material);
	
	
	/* Custom Edit for landscape sculpt brush selection */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void ReadyForSelectedTool(FName _toolName);


	/* Custom Edit for common Toolbox */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	ULandscapeEditorObject* RequestLandscapeEditorUI();
	
	UFUNCTION(BlueprintCallable, Category = "Custom")
	float GetInitToolStrength();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	float GetInitBrushSize();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	float GetInitTextureRotation();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnToolStrengthSliderMoved(float _value);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnBrushSizeSliderMoved(float _value);
	

	/* Custom Edit for sculpt Toolbox */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnBasicBrushButtonClicked(FName _brushSetName);


	/* Custom Edit for Image Toolbox */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	int32 GetInitTextureChannel();
	
	UFUNCTION(BlueprintCallable, Category = "Custom")
	bool GetInitAutoRotate();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void SetImageBrushTexture(UTexture2D* _texture);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnTextureSelectionChanged(UTexture2D* _texture);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnImportButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnTextureChannelSelectionChanged(int32 _enumIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnAutoRotateCheckboxChanged(bool _isChecked);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnTextureRotationSliderMoved(float _value);
	

	/* Custom Edit for blueprint brush */
protected:
	static TWeakObjectPtr<UEditorUtilityWidget> EUWInstance;

	UPROPERTY(BlueprintReadWrite, Category = "Custom")
	TArray<TObjectPtr<ALandscapeBlueprintBrush>> SpawnedBlueprintBrushes;
	
	void NativeConstruct() override;
	
public:
	static UEditorUtilityWidget* GetEUWInstance();
	void AddSpawnedBlueprintBrush(ALandscapeBlueprintBrush* _blueprintBrush);
	
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void ReadyForBluePrintBrush(TSubclassOf<ALandscapeBlueprintBrush> _bpBrushClass);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void UpdateLandscape();
	
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnEditConfirmButtonClicked();


	/* Custom Edit for Erase Toolbox */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void CustomUndo();


	/* Custom Edit for Spline Toolbox */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnAllSplinesButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnSelectedOnlyButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnControlPointsButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnSegmentsButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnUseAutoRotateControlPointCheckboxChanged(bool _isChecked);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void OnUAlwaysAutoRotateForwardCheckboxChanged(bool _isChecked);

	//~ Custom Edit End

protected:
	/** The display name for tabs spawned with this widget */
	UPROPERTY(Category = Config, EditDefaultsOnly, BlueprintReadWrite, AssetRegistrySearchable)
	FText TabDisplayName;

	UPROPERTY(Category = Config, EditDefaultsOnly, BlueprintReadWrite, AssetRegistrySearchable)
	FString HelpText;

	// Should this widget always be re-added to the windows menu once it's opened
	UPROPERTY(Config, Category = Settings, EditDefaultsOnly)
	bool bAlwaysReregisterWithWindowsMenu;

	// Should this blueprint automatically run OnDefaultActionClicked, or should it open up a details panel to edit properties and/or offer multiple buttons
	UPROPERTY(Category = Settings, EditDefaultsOnly, BlueprintReadOnly)
	bool bAutoRunDefaultAction;
};
