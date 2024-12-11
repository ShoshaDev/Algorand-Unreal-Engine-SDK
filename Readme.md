# Algorand SDK for Unreal Engine
Official Unreal Engine Plugin for Algorand Blockchain Platform.

## 📙 1. Overview:
Algorand Unreal Engine Plugin

For indepth documentation on the overview, setup and usage of this plugin check here: [Doc](https://github.com/ShoshaDev/Algorand-Unreal-Engine-SDK/blob/master/Algorand%20Unreal%20SDK%20Documentation.md)

This UE plugin includes:
- Root Folder: Unreal-Plugin - Put this plugin in your new or existing project to utilize.
- Example Folder: Unreal Plugin Example Project - Use this project to start your new game or demo/test Algorand System

This sdk contains a setting option to set RPC Info.
* RPC URL : You should set the Algorand node you want to work on with this url.
  ` setAlgoRpc("https://testnet-api.algonode.cloud"); `
* RPC Port : Set port which can connect into server supporting algorand node with.
  ` setAlgoPort(0); `
* RPC TokenHeader : You should set Header to request apis according to algorand node type.
  ` setAlgoTokenHeader("x-api-key:bLcs4F2SyGY0InF9M6Vl9piFTIZ8Ww281OjKXyE1"); `
  This param has a pair made of key and value.

RPC types you can select are like following:
* Rest API
  + mainnet: https://mainnet-api.algonode.cloud
  + testnet: https://testnet-api.algonode.cloud
  + betanet: https://betanet-api.algonode.cloud

* Sandbox Algorand Node
  + algod: http://localhost:4001 
  + kmd: http://localhost:4002
  + indexer: http://localhost:8980
* Local Algorand Node, (You should install new algorand node on your local)

 👔 You should add token for above some urls and can reference this [link](https://github.com/Wisdom-Labs/Algorand-Unreal-Engine-SDK/tree/master/example/unreal-algorand-demo#how-to-set-rpc-info).

## 📑 2. Prerequisites:

To use this UE plugin : ✅
- Unreal Engine 5.x is supported.
  https://docs.unrealengine.com/5.1/en-US/installing-unreal-engine/
- Windows: Visual Studio 2019 or newer, JetBrains Rider 2020.x
- macOS: Xcode 13.2.1
> In order to build project using Unreal Engine 5.0+, use a branch `master`.

## ⚙️ 3. Installation:
1. In this Github repository, clone this repo with `git` command or download as a zip file type.
2. You should check the plugin name (Algorand) into Build.cs on the `Source/UnrealAlgorandPlugin` folder
    ```csharp  
    public UnrealAlgorandPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Algorand" });

        PrivateDependencyModuleNames.AddRange(new string[] {  });
    }
    ```
3. Right click on UnrealAlgorandPlugin.uproject on root folder, and "Generate Visual Studio project files" - This process may take some time.
4. Once finished, open your *.sln project or *.uproject file with Visual Studio or JetBrains Rider.

## 🧭 4. Modules Reference:
* **Algorand** - entrypoint module, provides `C++`- & `Blueprints`- friendly interfaces for plugin's users. This module should be considered as the only module you need to depend on.
* **AlgorandAPI** - This module is one to make api request and process its result and send it to entrypoint module.
* **Blockchain** - This module contains core logic for building and signing txs from tx types and params.
* **Wallet** - This module is one related to manage wallet and wallet connect provider.
* **Vertices** - This module play an important role as an interface and router in connecting to our algorand c++ sdk (Vertices shared library).

## 🚀 5. Get Started

Read [Getting Started](docs/getting_started.md) to learn the basic workflows for developing on Algorand.
For understanding the SKD see [Algorand Unreal SDK Documentation](https://github.com/ShoshaDev/Algorand-Unreal-Engine-SDK/blob/master/Algorand%20Unreal%20SDK%20Documentation.md)

### Demo Project

Among the builds are some demos ready to be used. Currently, the following builds are available for Unreal:
[5.0](https://github.com/ShoshaDev/Algorand-Unreal-Engine-SDK/tree/5.0)
[5.2](https://github.com/ShoshaDev/Algorand-Unreal-Engine-SDK/tree/5.2)
[5.4](https://github.com/ShoshaDev/Algorand-Unreal-Engine-SDK/tree/5.4)
