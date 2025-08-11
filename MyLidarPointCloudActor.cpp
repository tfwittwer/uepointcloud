/*    Copyright (C) 2025 Tobias Wittwer
    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/. */
	
#include "MyLidarPointCloudActor.h"
#include "CoreGlobals.h"
#include "HAL/FileManagerGeneric.h"

AMyLidarPointCloudActor::AMyLidarPointCloudActor()
{
	// uncomment if you want to do things in the tick event
	//PrimaryActorTick.bCanEverTick = true;
}

void AMyLidarPointCloudActor::OnSelected(AActor* Target, FKey ButtonPressed)
{
	if (UWorld* World = GetWorld())
	{
		APlayerCameraManager* PlayerCamera = World->GetFirstPlayerController()->PlayerCameraManager;
		FVector camLoc = PlayerCamera->GetCameraLocation();


		// this is camera direction, but we want the mouse click
		//FRotator camRot = PlayerCamera->GetCameraRotation();
		//FVector camDir = camRot.Vector();
		// display camera position
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(camLoc.ToString()));
		
		FVector worldLocation;
		FVector worldDirection;
		World->GetFirstPlayerController()->DeprojectMousePositionToWorld(worldLocation, worldDirection);

		FLidarPointCloudPoint pointHit;

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(camLoc.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(worldDirection.ToString()));

		// keep in mind that UE units are centimeters
		if (pc->LineTraceSingle(camLoc, worldDirection, 10.0, true, pointHit))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(pointHit.Location.ToString()));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(((FVector(pointHit.Location)+offset)*0.01).ToString()));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString("no hit"));
		}

	}
}

void AMyLidarPointCloudActor::BeginPlay()
{
	OnClicked.AddUniqueDynamic(this, &AMyLidarPointCloudActor::OnSelected);
	
	// uncomment if you want to use ticks
	//Super::BeginPlay();

	FString cloudpath = "C:\\work\\UE";
	LoadClouds(cloudpath.GetCharArray().GetData());
}

void AMyLidarPointCloudActor::Tick(float DeltaTime)
{
	// uncomment if you want to use ticks
	// Super::Tick(DeltaTime);

	// tick stuff goes here

}

void AMyLidarPointCloudActor::Progress(float perc)
{
	
}

void AMyLidarPointCloudActor::Loaded(bool success)
{
	// leads to crashes
	//UE_LOG(LogTemp, Display, TEXT("Loaded, cloud bounds: %s"), *pc->GetBounds().ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(*pc->GetBounds().ToString()));
}

void AMyLidarPointCloudActor::LoadClouds(TCHAR* folder)
{
	FFileManagerGeneric fm;
	TArray<FString> filenames;
	FString extension = ".laz";
	fm.FindFiles(filenames, folder, extension.GetCharArray().GetData());

	TFunction<void(float progress)> fp = [this](float progress) { Progress(progress); };
	TFunction<void(bool success)> fp2 = [this](bool success) { Loaded(success); };

	FLidarPointCloudAsyncParameters loadParams(false, fp, fp2);

	TArray<ULidarPointCloud*> clouds;
	
	pc = ULidarPointCloud::CreateFromFile(filenames[0], loadParams);
	for (int32 i = 1; i < filenames.Num(); i++)
	{
		UE_LOG(LogTemp, Display, TEXT("Loading %s"), *FString(filenames[i]));
		clouds.Emplace(ULidarPointCloud::CreateFromFile(FPaths::Combine(folder, filenames[i]), loadParams));
	}

	pc->AlignClouds(clouds);
	pc->Merge(clouds);

	pc->BuildCollision();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("Clouds ready"));
	SetPointCloud(pc);

	// compute offset between original (world) and view coordinates
	FBox origBounds = pc->GetBounds(true);
	FBox viewBounds = pc->GetBounds(false);

	FVector origCenter = origBounds.GetCenter();
	FVector viewCenter = viewBounds.GetCenter();

	offset = origCenter - viewCenter;

	UE_LOG(LogTemp, Display, TEXT("Point cloud offset: %s"), *FString(offset.ToString()));
}

