// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"

//components:
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"

#include "Kismet/KismetSystemLibrary.h"//for close the AUT

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	//AddEvents:

	//Main Pause Events:
	//I dont delete widget, so if statmens check....
	if (!m_SwitchBtn->OnClicked.IsBound())
	{
		m_SwitchBtn->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnSwitchPressed);
	}
	if (!m_PlayBtn->OnClicked.IsBound())
	{
		m_PlayBtn->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnPlayPressed);
	}
	if (!m_ExitBtn->OnClicked.IsBound())
	{
		m_ExitBtn->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnExitPressed);
	}

	//Options menu:
	if (!m_BackBtn->OnClicked.IsBound())
	{
		m_BackBtn->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnBackPressed);
	}
	if (!m_NameInput->OnTextChanged.IsBound())
	{
		m_NameInput->OnTextChanged.AddUniqueDynamic(this, &UPauseMenu::OnNameInputChanged);
	}
	if (!m_ShowNameCheckBox->OnCheckStateChanged.IsBound())
	{
		m_ShowNameCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UPauseMenu::OnCheckStateUpdate);
	}

	/*m_SwitchBtn->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnSwitchPressed);
	m_PlayBtn->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnPlayPressed);
	m_ExitBtn->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnExitPressed);

	m_BackBtn->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnBackPressed);
	m_NameInput->OnTextCommitted.AddUniqueDynamic(this, &UPauseMenu::OnNameInputChanged);
	m_ShowNameCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UPauseMenu::OnCheckStateUpdate);*/
}

void UPauseMenu::NativeDestruct()
{
	Super::NativeConstruct();

	//Reset Menu After closing:
	Switcher();

	// /// or clear all Bindings
	//Main Pause Events:
	//m_SwitchBtn->OnClicked.Clear();
	//m_PlayBtn->OnClicked.Clear();
	//m_ExitBtn->OnClicked.Clear();

	//Options menu:
	//m_BackBtn->OnClicked.Clear();
	//m_NameInput->OnTextChanged.Clear();
	//m_ShowNameCheckBox->OnCheckStateChanged.Clear();
}

void UPauseMenu::Switcher(int32 index)
{
	if (m_MenusSwitcher)
	{
		m_MenusSwitcher->SetActiveWidgetIndex(index);
	}
}

void UPauseMenu::OnSwitchPressed()
{
	//draw canvas with index 1
	Switcher(1);
}

void UPauseMenu::OnBackPressed()
{
	//draw canvas with index 0
	Switcher();
}

void UPauseMenu::OnPlayPressed()
{
	//Call OnPauseCall form Controller:
	EntryToActionPhase.ExecuteIfBound();
}

void UPauseMenu::OnExitPressed()
{
	//Close the AUT:
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UPauseMenu::OnNameInputChanged(const FText& text)
{
	//Set new Name for Character:
	NewCharacterName.ExecuteIfBound(text);
}

void UPauseMenu::OnCheckStateUpdate(bool bIsChecked)
{
	//Update Name visibility
	NameVisibility.ExecuteIfBound(bIsChecked);
}

void UPauseMenu::SetCheckState(bool bNameVisibility)
{
	//Set default CheckBox CheckedState from current bShowName in Character

	//only if widget has been
	if (m_ShowNameCheckBox->CheckedState == ECheckBoxState::Undetermined)
	{
		m_ShowNameCheckBox->CheckedState =
			bNameVisibility ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}
}