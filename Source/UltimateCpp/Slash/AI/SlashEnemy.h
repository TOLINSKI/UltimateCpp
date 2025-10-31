// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SlashEnemy.generated.h"

UCLASS()
class ULTIMATECPP_API ASlashEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

// Slash Interface:
public:

};
