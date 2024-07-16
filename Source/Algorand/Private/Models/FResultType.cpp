#include "Models/FResultType.h"

FResultBoolean::FResultBoolean() { this->Result = false; }
FResultBoolean::FResultBoolean(const bool& Result)
{
	this->Result = Result;
}
FResultBoolean FResultBoolean::ToResultBoolean(const bool& Result)
{
	return FResultBoolean(Result);
}

FResultString::FResultString() { this->Result = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ"; }
FResultString::FResultString(const FString& Result)
{
	this->Result = Result;
}
FResultString FResultString::ToResultString(const FString& Result)
{
	return FResultString(Result);
}

FResultUInt64::FResultUInt64() { this->Result = 0; }
FResultUInt64::FResultUInt64(const FUInt64& Result)
{
	this->Result = Result;
}
FResultUInt64 FResultUInt64::ToResultUInt64(const FUInt64& Result)
{
	return FResultUInt64(Result);
}

FResultAccount::FResultAccount()
{
	this->Address = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ";
	this->Name = "UNKNOWN";
}
FResultAccount::FResultAccount(const FString& Address, const FString& Name)
{
	this->Address = Address;
	this->Name = Name;
}
FResultAccount FResultAccount::ToResultAccount(const FString& Address, const FString& Name)
{
	return FResultAccount(Address, Name);
}

FResultAcfgTx::FResultAcfgTx()
{
	this->TxID = "INVALID";
	this->AssetID = 0;
}
FResultAcfgTx::FResultAcfgTx(const FString& TxID, const FUInt64& AssetID)
{
	this->TxID = TxID;
	this->AssetID = AssetID;
}
FResultAcfgTx FResultAcfgTx::ToResultAcfgTx(const FString& TxID, const FUInt64& AssetID)
{
	return FResultAcfgTx(TxID, AssetID);
}

FResultApplTx::FResultApplTx()
{
	this->TxID = "INVALID";
	this->Result = "INVALID";
}
FResultApplTx::FResultApplTx(const FString& TxID, const FString& Result)
{
	this->TxID = TxID;
	this->Result = Result;
}
FResultApplTx FResultApplTx::ToResultApplTx(const FString& TxID, const FString& Result)
{
	return FResultApplTx(TxID, Result);
}

FResultAccAssets::FResultAccAssets()
{
	this->AssetIDs.Empty();
	this->AssetNames.Empty();
}
FResultAccAssets::FResultAccAssets(const TArray<FString>& AssetIDs, const TArray<FString>& AssetNames)
{
	this->AssetIDs = AssetIDs;
	this->AssetNames = AssetNames;
}
FResultAccAssets FResultAccAssets::ToResultAccAssets(const TArray<FString>& AssetIDs, const TArray<FString>& AssetNames)
{
	return FResultAccAssets(AssetIDs, AssetNames);
}
