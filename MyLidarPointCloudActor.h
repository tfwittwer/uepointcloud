// Fill out your copyright notice in the Description page of Project Settings.

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
