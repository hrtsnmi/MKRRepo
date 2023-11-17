// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPSwitcherWidget.generated.h"



class UButton;
class UWidgetSwitcher;
class UCanvasPanel;
class UEditableTextBox;
class UCheckBox;
/**
 *
 */

 // delegate to Change PlayerName
DECLARE_DELEGATE_OneParam(UpdateName, const FText&);

// delegate to Hide/Show 3D Widget
DECLARE_DELEGATE_OneParam(UpdateWidgetName, bool);

// delegate to unpause
DECLARE_DELEGATE(Unpause);

UCLASS()
class SH_API UPPSwitcherWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//Switch pressed:
	void Switcher(int32 index = 0);
	UFUNCTION()
		void OnSwitchPressed();
	UFUNCTION()
		void OnBackPressed();

	UFUNCTION()
		void OnPlayPressed();
	UFUNCTION()
		void OnExitPressed();
	UFUNCTION()		// to send new Name to character		when user Update text
		void OnNameInputChanged(const FText& text);
	UFUNCTION() // to send new bNameVisibility param
		void OnCheckStateUpdate(bool bIsChecked);

protected:
	//Main Pause
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (BindWidget), Category = "UI|Main Pause")
		UWidgetSwitcher* m_MenusSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (BindWidget), Category = "UI|Main Pause")
		UButton* m_PlayBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (BindWidget), Category = "UI|Main Pause")
		UButton* m_ExitBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (BindWidget), Category = "UI|Main Pause")
		UButton* m_SwitchBtn;

	//Options:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (BindWidget), Category = "UI|Options Pause")
		UEditableTextBox* m_NameInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (BindWidget), Category = "UI|Options Pause")
		UButton* m_BackBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (BindWidget), Category = "UI|Options Pause")
		UCheckBox* m_ShowNameCheckBox;

public:
	//Delegates
	UpdateName NewCharacterName;
	UpdateWidgetName NameVisibility;
	Unpause EntryToActionPhase;

	//Set Check State for created UI
	void SetCheckState(bool bNameVisibility);
};
