// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KMPlayerController.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FAvatar {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
	UTexture2D* mBaseTexture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
	UTexture2D* mHatTexture;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
	UTexture2D* mBodyTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
	UTexture2D* mGlassesTexture;
};

USTRUCT(BlueprintType)
struct FAvatarPiece {
	GENERATED_BODY()
public:

};

USTRUCT(BlueprintType)
struct FMember {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		FString mName = "Adam";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		int mPoints = 0;

	FMember() {
		mName = "";
		mPoints = 0;
	}
};

USTRUCT(BlueprintType)
struct FCompletedTask {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		FString mTaskName = "Gang";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		int mCompletedDate = 10;
};

USTRUCT(BlueprintType)
struct FTask {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		FString mTaskName = "kasse";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		FString mTaskDesc = "Vask ting";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		FMember mMember;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		int mDateDeadline = 10;
	FTask() {
		
	}
};

USTRUCT(BlueprintType)
struct FKollektiv {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kollektiv")
		FString mKollektivName = "Kollektiv";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kollektiv")
		TArray<FMember> mMembers;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kollektiv")
		TArray<FTask> mTasks;

	FKollektiv() {
	}

};

UCLASS()
class KOLLEKTIV_API AKMPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	FMember mSignedInMember;
	FKollektiv mActiveKollektiv;

	void DivideOutTasks();

	FMember GetMember(FString name);
	FKollektiv GetKollektiv(FString name);

	UFUNCTION(BlueprintCallable)
	bool AddTaskToKollektiv(FTask t);
	UFUNCTION(BlueprintCallable)
	bool AddMemberToKollektiv(FMember m);


	UFUNCTION(BlueprintCallable)
		TArray<FTask> GetAllTasks();
	UFUNCTION(BlueprintCallable)
		TArray<FTask> GetSignedInMembersTasks();
	UFUNCTION(BlueprintCallable)
	bool CanLogIn(FString name);
	UFUNCTION(BlueprintCallable)
	bool SignIn(FString memberName, FString kollektivName);
	UFUNCTION(BlueprintImplementableEvent)
	void SingedIn();
	UFUNCTION(BlueprintImplementableEvent)
		void OpenSignInScreen();
	UFUNCTION(BlueprintCallable)
	void SignOut();
};
