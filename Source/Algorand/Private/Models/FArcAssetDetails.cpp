#include "Models/FArcAssetDetails.h"
#include "VerticesApiOperations.h"

FArcAssetDetails::FArcAssetDetails()
{
	Standard = EArcType::Arc00;
	Unit_Name = "";
	Total = 0;
	Decimals = 0;
	Description = "";
	Clawback = "";
	Creator = "";
	Freeze = "";
	Manager = "";
	Reserve = "";
	Media_Url = "";
	External_Url = "";
	Animation_Url = "";
	Properties = TMap<FString, FString>();
}

FArcAssetDetails::FArcAssetDetails(const algorand::vertices::VerticesSDK::VerticesArcAssetDetailsResponse& response)
{
	Standard = EArcType(response.Standard);
	Unit_Name = response.Unit_Name;
	Total = response.Total;
	Decimals = response.Decimals;
	Description = response.Description;
	Clawback = response.Clawback;
	Creator = response.Creator;
	Freeze = response.Freeze;
	Manager = response.Manager;
	Reserve = response.Reserve;
	Media_Url = response.Media_Url;
	External_Url = response.External_Url;
	Animation_Url = response.Animation_Url;
	Properties = response.Properties;
}


