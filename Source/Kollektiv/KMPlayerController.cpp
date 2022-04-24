																							// Fill out your copyright notice in the Description page of Project Settings.
#include "KMPlayerController.h"
#include <iostream>
#include <fstream>
#include <sstream>

void AKMPlayerController::BeginPlay() {
	Super::BeginPlay();
	//
	
	LoadAllPieces();
}

void AKMPlayerController::LoadAllPieces() {
	//Load all avatar pieces
	UTexture2D* tmp;
	FString path;

	if (path.IsEmpty()) {
		return;
	}

	tmp = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *path));

}

bool AKMPlayerController::CanLogIn(FString name)
{
	return true;
}

bool AKMPlayerController::SignIn(FString memberName, FString kollektivName)
{
	mSignedInMember = GetMember(memberName);
	mActiveKollektiv = GetKollektiv(kollektivName);
	//Set the base bodt texture for the avatar
	/*UTexture2D* tmp;
	FString path = "../Content/AvatarClothing/BASEBODY.png";
	tmp = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *path));
	mSignedInMember.mAvatar.mBaseTexture = tmp;*/
	SingedIn();
	return false;
}

void AKMPlayerController::SignOut()
{
	
	OpenSignInScreen();
}

void AKMPlayerController::DivideOutTasks() {

	for (int i = 0; i < mActiveKollektiv.mTasks.Num(); i++) {
		//Give to random member fo the kollektiv
		int memberIndex = rand() % mActiveKollektiv.mMembers.Num();

		mActiveKollektiv.mTasks[i].mMember = mActiveKollektiv.mMembers[memberIndex];
	}

}

FMember AKMPlayerController::GetMember(FString name) {
	//Keep original name
	FString memberName = name;
	//read file to get points
	name.Append(".txt");
	std::ifstream file;
	//Add content directory to the member name
	FString fileLoc = FPaths::ProjectContentDir();
	//Add the specific file ending
	fileLoc.Append(name);
	//Open the file, converts fstring to std::string
	file.open(std::string(TCHAR_TO_UTF8(*fileLoc)));
	//Could not open the file
	if (!file) {
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file"));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *fileLoc);
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Creating new file  %s"), *fileLoc);

		FMember member;
		member.mName = memberName;
		member.mPoints = 0;
		UE_LOG(LogTemp, Warning, TEXT("Created member with name = %s"), *member.mName);
		return member;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did load text from file %s"), *name);
	}
	//Parse the file
	std::string oneLine;
	std::string oneWord;
	std::string tmp;

	FMember m;
	m.mName = memberName;
	while (std::getline(file, oneLine, '\n')) {
		//To store one word
		std::stringstream sstream;
		sstream << oneLine;
		//Reset the word for each line
		oneWord = "";
		//Put it into oneWord
		sstream >> oneWord;

		//check for member to load
		if (oneWord == "k") {
			//Store member name
			sstream >> tmp;
			FKollektiv k = GetKollektiv(tmp.c_str());
			//This is a kollektiv about the be declared
			UE_LOG(LogTemp, Warning, TEXT("Found kollektiv %s"), *k.mKollektivName);
			continue;
		}
		//Points
		if (oneWord == "p") {
			sstream >> tmp;
			m.mPoints = std::stoi(tmp);
		}
		//Avatar pieces name
		if (oneWord == "a") {
			sstream >> tmp;
			m.mUnlockedAvatarPieces.Add(tmp.c_str());
		}
	}


	return m;
}


FKollektiv AKMPlayerController::GetKollektiv(FString name) {


	return FKollektiv{};
}

bool AKMPlayerController::AddMemberToKollektiv(FMember m) {
	for (int i = 0; i < mActiveKollektiv.mMembers.Num(); i++) {
		if (mActiveKollektiv.mMembers[i].mName== m.mName) {
			return false;
		}
	}


	mActiveKollektiv.mMembers.Add(m);
	return true;
}


bool AKMPlayerController::AddTaskToKollektiv(FTask t) {
	for (int i = 0; i < mActiveKollektiv.mTasks.Num(); i++) {
		if (mActiveKollektiv.mTasks[i].mTaskName == t.mTaskName) {
			return false;
		}
	}

	mActiveKollektiv.mTasks.Add(t);
	return true;
}

TArray<FTask> AKMPlayerController::GetAllTasks() {
	TArray<FTask> t;

	for (int i = 0; i < mActiveKollektiv.mTasks.Num(); i++) {
		t.Add(mActiveKollektiv.mTasks[i]);
	}

	return t;
}

TArray<FTask> AKMPlayerController::GetSignedInMembersTasks() {
	TArray<FTask> t;

	for (int i = 0; i < mActiveKollektiv.mTasks.Num(); i++) {
		//Check all tasks, if the name the member name matches, add to t
		if (mActiveKollektiv.mTasks[i].mMember.mName == mSignedInMember.mName) {
			t.Add(mActiveKollektiv.mTasks[i]);
		}
		
	}

	return t;
}

TArray<FAvatarPiece> AKMPlayerController::GetAllAvatarUnlockables()
{
	return mAllAvatarUnlockables;
}

TArray<FAvatarPiece> AKMPlayerController::GetSignedInMembersUnlockedPieces() {
	//Refresh the owned
	TArray<FAvatarPiece> t;
	for (int i = 0; i < mSignedInMember.mUnlockedAvatarPieces.Num(); i++) {
		for (int j = 0; j < mAllAvatarUnlockables.Num(); i++) {
			if (mSignedInMember.mUnlockedAvatarPieces[i] == mAllAvatarUnlockables[j].mName) {
				t.Add(mAllAvatarUnlockables[j]);
			}
		}
	}

	return t;
}

bool AKMPlayerController::EquipAvatarPiece(FAvatarPiece p) {
	switch (p.mType) {
	case AvatarPieceType::BODY:
		mSignedInMember.mAvatar.mBodyTexture = p.mTexture;
		break;
	case AvatarPieceType::HEAD:
		mSignedInMember.mAvatar.mHeadTexture = p.mTexture;
		break;
	case AvatarPieceType::HAT:
		mSignedInMember.mAvatar.mHatTexture = p.mTexture;
		break;
	case AvatarPieceType::FEET:
		mSignedInMember.mAvatar.mFeetTexture = p.mTexture;
		break;
	case AvatarPieceType::GLASSES:
		mSignedInMember.mAvatar.mGlassesTexture = p.mTexture;
		break;
	}

	return true;
}

bool AKMPlayerController::BuyAvatarPiece(FAvatarPiece p) {

	if (mSignedInMember.mPoints > p.mCost) {

		mSignedInMember.mUnlockedAvatarPieces.Add(p.mName);
		return true;
	}
	else {
		return false;
	}

	return true;
}