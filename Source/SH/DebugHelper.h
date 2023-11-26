#pragma once
#include "DrawDebugHelpers.h"


namespace Debug
{
	static void Print(const FString& Msg, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 6.f, Color, Msg);
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	}

	static void DrawDebugArrow(UWorld* World, const FVector& Start, const FVector& Direction, const FColor& color, float time = -1.f)
	{
		DrawDebugDirectionalArrow(World, Start, Start+ Direction * 100.f, 50.f, color,false, time);
	}

	static void DrawSphere(UWorld* World, const FVector& Center, const FColor& color)
	{
		DrawDebugSphere(World, Center, 20.f, 20, color, false, 5.f);
	}
}