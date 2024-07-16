#include "ArcResponseBuilders.h"
#include "VerticesApiOperations.h"

namespace ArcResponseBuilders {
	
	void ArcResponseBuilders::buildArcAssetDetailsResponse(const Arc03& arc_asset, const FAPIArcAssetDetailsGetDelegate& delegate)
	{
		Vertices::VerticesArcAssetDetailsResponse response;
		
		response.Standard = 1;		// arc03 
		
		response.Unit_Name = arc_asset.asset.params.unit_name;
		response.Total = arc_asset.asset.params.total;
		response.Decimals = arc_asset.asset.params.decimals;
		response.Description = arc_asset.metadata.description;
		response.Clawback = arc_asset.asset.params.clawback;
		response.Creator = arc_asset.asset.params.creator;
		response.Freeze = arc_asset.asset.params.freeze;
		response.Manager = arc_asset.asset.params.manager;
		response.Reserve = arc_asset.asset.params.reserve;
		response.Media_Url = arc_asset.metadata.image;
		response.External_Url = arc_asset.metadata.external_url;
		response.Animation_Url = arc_asset.metadata.animation_url;
		response.Properties = arc_asset.metadata.properties; 
		
		AsyncTask(ENamedThreads::GameThread, [delegate, response]()
		{
			delegate.ExecuteIfBound(response);
		});
	}

	void ArcResponseBuilders::buildArcAssetDetailsResponse(const Arc19& arc_asset, const FAPIArcAssetDetailsGetDelegate& delegate)
	{
		Vertices::VerticesArcAssetDetailsResponse response;
		
		response.Standard = 2;			// arc19

		response.Unit_Name = arc_asset.asset.params.unit_name;
		response.Total = arc_asset.asset.params.total;
		response.Decimals = arc_asset.asset.params.decimals;
		response.Description = arc_asset.metadata.description;
		response.Clawback = arc_asset.asset.params.clawback;
		response.Creator = arc_asset.asset.params.creator;
		response.Freeze = arc_asset.asset.params.freeze;
		response.Manager = arc_asset.asset.params.manager;
		response.Reserve = arc_asset.asset.params.reserve;
		response.Media_Url = arc_asset.metadata.image;
		response.Properties = arc_asset.metadata.properties; 
		
		AsyncTask(ENamedThreads::GameThread, [delegate, response]()
		{
			delegate.ExecuteIfBound(response);
		});
	}

	void ArcResponseBuilders::buildArcAssetDetailsResponse(const Arc69& arc_asset, const FAPIArcAssetDetailsGetDelegate& delegate)
	{
		Vertices::VerticesArcAssetDetailsResponse response;
		
		response.Standard = 3;	// arc69

		response.Unit_Name = arc_asset.asset.params.unit_name;
		response.Total = arc_asset.asset.params.total;
		response.Decimals = arc_asset.asset.params.decimals;
		response.Description = arc_asset.metadata.description;
		response.Clawback = arc_asset.asset.params.clawback;
		response.Creator = arc_asset.asset.params.creator;
		response.Freeze = arc_asset.asset.params.freeze;
		response.Manager = arc_asset.asset.params.manager;
		response.Reserve = arc_asset.asset.params.reserve;
		response.Media_Url = arc_asset.metadata.media_url;
		response.External_Url = arc_asset.metadata.external_url;
		response.Properties = arc_asset.metadata.properties;

		response.SetSuccessful(true);
		
		AsyncTask(ENamedThreads::GameThread, [delegate, response]()
		{
			delegate.ExecuteIfBound(response);
		});
	}

	void ArcResponseBuilders::buildAccountInformationResponse(const AccountAsset& arc_asset, const FAPIAccountInfoGetDelegate& delegate)
	{
		Vertices::VerticesAccountInformationResponse response;

		if(arc_asset.info.Num() != 0)
		{
			for(int i = 0; i < arc_asset.info.Num(); i++)
			{
				response.AssetIDs.Add(FString::Printf(TEXT("%llu"), arc_asset.info[i].id));
				response.AssetNames.Add(arc_asset.info[i].name);
			}
				
			response.SetSuccessful(true);		
		}
		else
		{
			response.SetSuccessful(false);
			response.SetResponseString("can't fetch account Info for assets.");
		}
		
		AsyncTask(ENamedThreads::GameThread, [delegate, response]()
		{
			delegate.ExecuteIfBound(response);
		});
	}
}
