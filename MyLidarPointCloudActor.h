/*    Copyright (C) 2025 Tobias Wittwer
    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/. */

#pragma once

#include "CoreMinimal.h"
#include "LidarPointCloud.h"
#include "LidarPointCloudActor.h"
#include "MyLidarPointCloudActor.generated.h"

/**
 * 
 */
UCLASS()
class HAIVIZ_API AMyLidarPointCloudActor : public ALidarPointCloudActor
{
	GENERATED_BODY()
	
public:
	AMyLidarPointCloudActor();
	FVector offset;

	void Progress(float perc);
	void Loaded(bool success);
	void LoadClouds(TCHAR* folder);

	UPROPERTY(VisibleAnywhere)
	ULidarPointCloud* pc;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSelected(AActor* Target, FKey ButtonPressed);


protected:
	virtual void BeginPlay() override;

};

