# Algorand Unreal SDK Documentation

## 📑 Contents

### [Algorand C++ SDK Documentation](#algorand-c-sdk-documentation-1)

1. [Pre-requisites](#1-pre-requisites)
2. [Installation & Enabling](#2-installation--enabling)
    - [Installation](#🧰-installation)
    - [Enabling](#enabling)
3. [Creating a Demo project](#3-creating-a-demo-project)
    - [Configuration](#configuration)
    - [Setting unix_config.h](#setting-unix_configh)
    - [Running a Demo project](#running-a-demo-project)
4. [Creating static or shared libraries of Vertices](#4-creating-static-or-shared-libraries-of-vertices)
5. [Tests](#5-tests)

### [Algorand Unreal Plugin Documentation](#algorand-unreal-plugin-documentation-1)
1. [Pre-requisites](#1-pre-requisites-1)
2. [Installation & Enabling](#2-installation)
3. [Creating a Demo project](#3-creating-a-demo-project-1)

<br>

***

## Algorand C++ SDK Documentation
---
    The Unreal Algorand SDK provides developers with an easy way for devices to interact with the Algorand chain.
    We are doing our best to achieve those goals:

#### C library can be included in C++ projects.
- It can be easily imported as a third-party library.
- Examples provided:
    - Windows
    - Unix-based OS
    - Mac OS (not completed)

- Connect to any Algorand API (local or remote provider)
- Build with CMake and Make.
- Continuous Integration to maintain code quality:
    - Build static and shared libraries for several targets.
    - Unit-Testing

#### At this SDK, there are some to-do lists to add new features:
- Some bugs exist when compiling on some toolchains. (Mac OS)
- Add another transaction type (Application Call) and wallet connect provider with QR Link.

--- 
## 1. Pre-requisites
- C++ 14 or newer
- Optional: GNU make for mac and Linux, Visual Studio toolchain for windows
- Optional: CLion 2021 or newer for Windows, Linux, mac OS
- You referenced a basic sdk called c-vertices-sdk. https://github.com/vertices-network/c-vertices-sdk

## 2. Installation & Enabling

### 🧰 Installation
```shell
# clone into a directory
git clone <url> [path]

# clone into a directory including its submodules
git clone --recurse-submodules <url> [path]

# clone as submodule into an optionally specified location
git submodule add <url> [path]
```

### [Enabling](#2-installation--enabling-3)
- Compilation
    - CMake is currently used to build the library and examples(GNU Make and Visual Studio Make)
- CMake	
    - Windows
        - In order to build the source using CMake, you should select a toolchain.
        - CLion is supporting MinGW for Linux and Visual Studio for Windows (recommend).

        <img src="Resources/1.png">

- Linux
    - You should select MinGW (GNU) to build the project.
- Mac
    - You can enter at the command line.

```shell
mkdir build && cd build
cmake ..

# build static library: lib/libvertices.a
make vertices

# build Unix example
make unix_example 
```
> You can import dependencies by loading CMakeLists.

<img src="Resources/2.png">

Click Build > Build All or Rebuild All to build this project.

## 3. Creating a Demo project

### Configuration
    
A config file provides an easy way to configure the SDK: include/vertices_config.h and include/vertices_types.h.

### Setting unix_config.h

Now we are creating a new file called unix_config.h on examples/unix/config folder to let it set rpc info of algorand chain.

```c
#define TESTNET_ALGONODE_API                "https://testnet-api.algonode.cloud"
#define TESTNET_ALGOEXPLORER_API            "https://node.testnet.algoexplorerapi.io"
#define TESTNET_PURESTAKE_API               "https://testnet-algorand.api.purestake.io/ps2"
#define TESTNET_LOCAL_API                   "localhost"

#define TESTNET_ALGONODE_INDEXER_API                "https://testnet-idx.algonode.network"
#define TESTNET_ALGOEXPLORER_INDEXER_API            "https://node.testnet.algoexplorerapi.io"
#define TESTNET_PURESTAKE_INDEXER_API               "https://testnet-algorand.api.purestake.io/idx2"
#define TESTNET_LOCAL_INDEXER_API                   "localhost"

#define TESTNET_ALGONODE_PORT           443
#define TESTNET_ALGOEXPLORER_PORT       0
#define TESTNET_PURESTAKE_PORT          0
#define TESTNET_LOCAL_PORT              8080

#define TESTNET_ALGONODE_AUTH_HEADER        ""
#define TESTNET_ALGOEXPLORER_AUTH_HEADER    ""
#define TESTNET_PURESTAKE_AUTH_HEADER       "x-api-key:"
#define TESTNET_LOCAL_AUTH_HEADER           "X-Algo-API-Token:"
```
We are setting config information of algorand rpc. (rpc API, PORT, and AUTH 
HEADER).

In this file, you can find essential information to run a project as params.

```c
#define SERVER_NODE_URL              TESTNET_ALGONODE_API
#define SERVER_INDEXER_URL           TESTNET_ALGONODE_INDEXER_API
#define SERVER_PORT             TESTNET_ALGONODE_PORT
#define SERVER_TOKEN_HEADER     (TESTNET_ALGONODE_AUTH_HEADER TESTNET_ALGONODE_API_TOKEN)

#define ACCOUNT_RECEIVER "LCKVRVM2MJ7RAJZKPAXUCEC4GZMYNTFMLHJTV2KF6UGNXUFQFIIMSXRVM4"
#define ACCOUNT_MANAGER "NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE"
#define ACCOUNT_RESERVE "NBRUQXLMEJDQLHE5BBEFBQ3FF4F3BZYWCUBBQM67X6EOEW2WHGS764OQXE"
#define ACCOUNT_FREEZE "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ"
#define ACCOUNT_CLAWBACK "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ"
#define APP_ID      (16037129)
#define WALLET_PASSWORD "dragon"
#define ALICE_NAME "dragon_1"
#define BOB_NAME "dragon_2"
```

- **ACCOUNT_RECEIVER**, **ACCOUNT_MANAGER**, **ACCOUNT_RESERVE**, **ACCOUNT_FREEZE**, **ACCOUNT_CLAWBACK** is 32 bytes address and is used to build basic algorand transactions, and **APP_ID** is a unique identifier when a smart contract is deployed 
to the algorand chain.
- Also, **WALLET_PASSWORD** is used to to secure an algorand wallet.

### Running a Demo project

You should install libsodium and curl library for HTTP stack and Ed25519 Signing.

If you are using the Conda environment, you can skip this step.

- macOS
    ```shell
    brew install libsodium curl
    ```
- Linux
    ```shell
    apt-get install libsodium curl
    ```

*At first*, two account variables are declared to test transactions on the algorand chain and the action of creating and loading an account and also it creates a struct type variable to let vertices lib connect with the algorand chain.

```c
typedef struct
{
   provider_info_t *provider;
   ret_code_t
   (*vertices_evt_handler)(vtc_evt_t *evt);
} vertex_t;
—---------------------------------------------------------------------------------------------
typedef struct {
   unsigned char private_key[ADDRESS_LENGTH];  //!< 32-bytes private key
   account_info_t *vtc_account;               //!< pointer to Vertices account data
} account_t;

// Alice's account is used to send data, keys will be retrived from config/key_files.txt
static account_t alice_account;
// Bob is receiving the money 😎
static account_t bob_account;
```

Next, Main.cpp has 4 big functions. (vertices_evt_handler, create_new_account, load_existing_account, main ()) 

- Vertices_evt_handler

    This function is one to let the user handle vertices events (signed, needed to sign) outside of vertices, which should be inserted into vertices lib as a function parameter when running a project and defined outside of vertices lib.

- Create_new_account

    This action is one to let the user create new random key pair consisting of the private key and public key with 32 bytes and store them as external files.

    ```c
    unsigned char ed25519_sk[crypto_sign_ed25519_SECRETKEYBYTES];
    randombytes_buf(seed, sizeof(seed));

    crypto_sign_ed25519_seed_keypair(ed25519_pk, ed25519_sk, seed);
    ```

- Load_existing_account

    This action lets the user load an existing account from external files and update the account info using the vertices function.  (balance, status)

    ```c
    err_code = vertices_account_new_from_b32(public_b32, &alice_account.vtc_account);
    VTC_ASSERT(err_code);

    LOG_INFO("💳 Created Alice's account: %s", alice_account.vtc_account->public_b32);
    ```

- Main

    This function is an entry point to let users run a demo project and test all functions that a vertice lib provides to them.

    - Initialize variables.
        ```c
        action_t run_tx;
        run_tx.kind = ACC_TYPE;
        run_tx.action.acc_type = CREATE_MNEMONIC_ACC;

        // init provider
        init_provider();

        // init accounts for processing transaction
        init_accounts(run_tx);
        ```
            
    - We can test to create new accounts and save into the wallet or build one of algorand transactions with given accounts.
        > For creating algorand accounts
        ```c
        run_tx.kind = ACC_TYPE;
        run_tx.action.acc_type = CREATE_MNEMONIC_ACC;
        ```
        > For building algorand transactions
        ```c
        run_tx.kind = TX_TYPE;
        run_tx.action.tx_type = PAY_TX;
        ```
    - **Steps for building algo transactions** with vertices lib

        1. You should create new vertice lib and check the status and version of algorand provider defined on vertices lib.
        Also, it loads accounts from a saved wallet.
            ```c
            int ret = sodium_init();
            VTC_ASSERT_BOOL(ret == 0);

            // create new vertex
            err_code = vertices_new(&m_vertex, true);
            VTC_ASSERT(err_code);

            // load vertices wallet
            err_code = load_wallet(run_tx);
            VTC_ASSERT(err_code);

            // check health of vertex net
            err_code = vertex_health();
            VTC_ASSERT(err_code);
            ```
        2. Initialized accounts randomly or with specific mnemonics
            ```c
            case CREATE_RANDOM_ACC: {
                err_code = vertices_s_account_new_random(&alice_account);
                VTC_ASSERT(err_code);
                err_code = vertices_s_account_new_random(&bob_account);
                VTC_ASSERT(err_code);
                // Test mnemonic from account
                char *mnemonic;
                err_code = vertices_mnemonic_from_sk(alice_account.private_key, &mnemonic);
                VTC_ASSERT(err_code);
                printf("mnemonics of a random account: %s\n", mnemonic);
                err_code = vertices_mnemonic_from_sk(bob_account.private_key, &mnemonic);
                VTC_ASSERT(err_code);
                printf("mnemonics of a random account: %s\n", mnemonic);
                break;
            }

            case CREATE_MNEMONIC_ACC: {
                char *mnemonic_str = "rally relief lucky maple primary chair syrup economy tired hurdle slot upset clever chest curve bitter weekend prepare movie letter lamp alert then able taste";
                err_code = vertices_s_account_new_from_mnemonic(mnemonic_str, &alice_account, (const char*) ALICE_NAME);
                VTC_ASSERT(err_code);
                mnemonic_str = "base giraffe believe make tone transfer wrap attend typical dirt grocery distance outside horn also abstract slim ecology island alter daring equal boil absent carpet";
                err_code = vertices_s_account_new_from_mnemonic(mnemonic_str, &bob_account, (const char*) BOB_NAME);
                VTC_ASSERT(err_code);
                // Test mnemonic from account
                char *mnemonic;
                err_code = vertices_mnemonic_from_account((const char *) ALICE_NAME, &mnemonic);
                VTC_ASSERT(err_code);
                printf("mnemonic of %s account : ->%s\n", (const char *) ALICE_NAME, mnemonic);
                err_code = vertices_mnemonic_from_account((const char *) BOB_NAME, &mnemonic);
                VTC_ASSERT(err_code);
                printf("mnemonic of %s account : ->%s\n", (const char *) BOB_NAME, mnemonic);
                break;
            }
            ```
        3. Build transactions with tx info and type.
            - **Payment TX**

                This action is one to let users send algos from one account to another Account.
                ```c
                case PAY_TX: {
                // send assets from account 0 to account 1
                char *notes = (char *) "Alice sent 1 Algo to Bob";
                err_code =
                        vertices_transaction_pay_new(alice_account.vtc_account,
                                                     (char *) bob_account.vtc_account->public_key /* or ACCOUNT_RECEIVER Public Key */,
                                                     AMOUNT_SENT,
                                                     notes);
                VTC_ASSERT(err_code);
                }
                ```

            - **Asset Config TX**
                This action lets us to create new algorand assets.  ex: arc3, arc19, arc69
                ```c
                char *notes = (char *) "Create a new asset";
                err_code =
                        vertices_transaction_asset_cfg(alice_account.vtc_account,
                                                       (char *) alice_account.vtc_account->public_key, // (char *) manager_account.vtc_account->public_key,
                                                       (char *) reserve_account.vtc_account->public_key,
                                                       (char *) freeze_account.vtc_account->public_key,
                                                       (char *) clawback_account.vtc_account->public_key,
                                                       0,
                                                       10000,
                                                       8,
                                                       true,
                                                       (void *) "USD",
                                                       (void *) "SHOSHA",
                                                       (void *)"",  // (void *) "http://this.test.com"
                                                       notes
                                                       );
                VTC_ASSERT(err_code);
                ```

            - **Asset Transfer TX**
                This action lets us build opt-in or transfer assets
                > **opt-in**
                ```c
                char *notes = (char *) "Create an Opt-In transaction";
                err_code =
                        vertices_transaction_asset_xfer(bob_account.vtc_account,
                                                        (char *) bob_account.vtc_account->public_key,
                                                        (char *) bob_account.vtc_account->public_key,
                                                        715553268,
                                                        0,
                                                        notes
                        );
                VTC_ASSERT(err_code);
                ```
                > **asset transfer**
                ```c
                char *notes = (char *) "Transfer an algorand asset";
                err_code =
                        vertices_transaction_asset_xfer(alice_account.vtc_account,
                                                        (char *) alice_account.vtc_account->public_key,
                                                        (char *) bob_account.vtc_account->public_key,
                                                        715553268,
                                                        200,
                                                        notes
                                                        );
                VTC_ASSERT(err_code);
                ```

            - **Application TX**  (Not Completed)

                This action is one to let a user send an application call.
		        You can get info on the application call tx using the following function.
                ```c
                app_values_t app_kv = {0};
                err_code = vertices_application_get(APP_ID, &app_kv);
                VTC_ASSERT(err_code);
                for (uint32_t i = 0; i < app_kv.count; ++i) {
                    if (app_kv.values[i].type == VALUE_TYPE_INTEGER) {
                        LOG_INFO("%s: %llu", app_kv.values[i].name, (long long unsigned) app_kv.values[i].value_uint);
                    } else if (app_kv.values[i].type == VALUE_TYPE_BYTESLICE) {
                        LOG_INFO("%s: %s", app_kv.values[i].name, app_kv.values[i].value_slice);
                    }
                }
                ```

                Next, build an application call tx.
                ```c
                // send application call
                app_values_t kv = {0};
                kv.count = 1;
                kv.values[0].type = VALUE_TYPE_INTEGER;
                kv.values[0].value_uint = 32;

                err_code = vertices_transaction_app_call(alice_account.vtc_account, APP_ID, &kv);
                VTC_ASSERT(err_code);
                ```
        4. Send registered txs to algorand chain. This action is one to let vertices lib process events to get signed or pending ones for the transaction process.
            ```c
            unsigned char *txID = nullptr;
            txID = new unsigned char[TRANSACTION_HASH_STR_MAX_LENGTH];

            // processing
            size_t queue_size = 1;
            while (queue_size && err_code == VTC_SUCCESS) {
                err_code = vertices_event_process(&queue_size, txID);
                VTC_ASSERT(err_code);
            }
            ```
            You can get tx id using this function.
        5. Free memory of created new accounts.
            ```c
            vertices_wallet_save((const char*) WALLET_PASSWORD);

            // delete the created secret accounts from the Vertices wallet
            err_code = vertices_wallet_free();
            VTC_ASSERT(err_code);
            ```

## 4. Creating static or shared libraries of Vertices

There are some options to let users use the vertices library as static or shared libraries and recommended solutions when occurring issues.

- src/CMakeLists.txt

    You should add the following commands to use the vertices library as a static or shared type.
    - static
        ```c
        add_library(vertices STATIC ${VERTICES_SRC_FILES})
        ```

    - shared
        ```c
        add_library(vertices SHARED ${VERTICES_SRC_FILES})
        ```
    If you want to change the directory path of the output library, you should add this such commands.

- **Update the MPack module on the windows platform**

    To build as vertices which libtype has shared library on windows, you should remove the following commands on mpack/src/mpack/mpack-defaults.h file.

## 5. Tests

- CppUTest install

    Make sure you have cpputest installed:
    - Linux - sudo apt-get install cpputest lcov
    - OSX - brew install cpputest lcov

    After installing cpputest library on your OS system, you should set 

    CPPUTEST_HOME as an environment variable for CPPUTEST.
    ```c
    if(DEFINED CPPUTEST_HOME)
        message(STATUS "Using CppUTest home: ${CPPUTEST_HOME}")
        set(CPPUTEST_INCLUDE_DIRS ${CPPUTEST_HOME}/include)
        set(CPPUTEST_LIBRARIES ${CPPUTEST_HOME}/cpputest_build/lib)
        set(CPPUTEST_LDFLAGS CppUTest CppUTestExt)
    else()
        find_package(PkgConfig REQUIRED)
        pkg_search_module(CPPUTEST REQUIRED cpputest>=3.8)
        message(STATUS "Found CppUTest version ${CPPUTEST_VERSION}")
    endif()
    ```
    From **/tests** you will be able to launch the unique command to run all the test:
    ```shell
    make all
    ```
- Running Tests

    There are some test cases for the vertices library.
    - Accounts
    - Base32
    - Base64
    - BasicTX
    - Sha512_256


## Algorand Unreal Plugin Documentation
---
This plugin provides functionality for creating a wallet and building different types of transactions on the Algorand node.
This is a video link showing how to integrate our plugin with Algorand Blockchain.  
Demo Video Url:  https://youtu.be/zRjuzZP1ngY

## 1. Pre-requisites
- Unreal Engine 5.0 is supported, i.e this should be installed via the Epic Games Launcher. You can reference this link to install. 
https://docs.unrealengine.com/5.1/en-US/installing-unreal-engine/
- Windows: Visual Studio 2019 or newer, JetBrains Rider 2020.x
- macOS: Xcode 13.2.1

## 2. Installation
- 🧰 Installation

    You can use this guide for installing a SDK. [docs.unreal-sdk.installation](https://github.com/ShoshaDev/Algorand-Unreal-Engine-SDK/tree/5.0/Plugins/Algorand#%EF%B8%8F-3-installation)
- Enabling

    You should register the plugin name into Build.cs on the Source directory of your project and rebuild a demo project.

    <img src="Resources/3.png">

### 3. Creating a Demo project

Now we are going to create our new project. Run Unreal Engine Editor, find a project browser, and pick the Games category. Now we can see a list of available project templates we can use. Let’s just pick a Blank template, but if you are familiar with UE, you can go with any of them. On the next page, set the path and name for your project, and choose the programming language you want to go with. You can choose from C++ and Blueprint, and if you are not familiar with C++ languages, it’s better to go ahead with Blueprint. 

- **Working with C++**

    - Creating a C++ Project
    
    <img src="Resources/4.png">

    - Waiting for the project to be created

    <img src="Resources/5.png">

    - The Default map Blank is shown as below.

    <img src="Resources/6.png">

    - Install and enable the Algorand plugin as mentioned above in step 2.
    - After restarting Unreal Editor, you can check the loading status of our plugin. Select Edit > Plugins in the menu, check Algorand Plugin

    <img src="Resources/7.png">

    - Select Edit > Editor Preferences > Source Code > Accessor: Source Code Editor > Rider  (if you prefer Visual Studio 2019 or newer,  you can select it)

    <img src="Resources/8.png">

    - Click Play button

1. Creating an **AlgorandUnrealManager**

    You need to set up your project and Algorand Unreal Plugin within it.

    Setting up **AlgorandUnrealManager**

    First of all, we need to set up **AlgorandUnrealManager** to be able to use all of the API methods provided by the plugin.

    Let’s open the **Source path** of your project and create a new header named AlgorandHandler.h and AlgorandHandler.cpp

    <img src="Resources/9.png">

    Generate Demo project files with updates of AlgorandHandler files.

    <img src="Resources/10.png">

    ```c
    ….
    AlgorandHandler.h
    #pragma once

    #include "Models/FUInt64.h"
    #include "CoreMinimal.h"
    #include "UAlgorandUnrealManager.h"

    #include "AlgorandHandler.generated.h"

    UCLASS(minimalapi)
    class UAlgorandHandler : public UObject {
        GENERATED_BODY()

    public:
	UAlgorandHandler();

	UPROPERTY()
		UAlgorandUnrealManager* algorandManager;
	
	UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
	void RunSomeLogic();

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultBooleanCallback(const EResultType& ResultType, const FResultBoolean& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultStringCallback(const EResultType& ResultType, const FResultString& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultAccountCallback(const EResultType& ResultType, const FResultAccount& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultAccountsCallback(const EResultType& ResultType, const FResultAccounts& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultUInt64Callback(const EResultType& ResultType, const FResultUInt64& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultAcfgTxCallback(const EResultType& ResultType, const FResultAcfgTx& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultApplTxCallback(const EResultType& ResultType, const FResultApplTx& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultArcAssetDetailsCallback(const EResultType& ResultType, const FArcAssetDetails& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultAccAssetsCallback(const EResultType& ResultType, const FResultAccAssets& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnErrorCallback(const FError& Error);
    };   // UAlgorandHandler  class end
    ……….
    ```

    We’ll use this class to set up and hold the **UAlgorandUnrealManager** instance and run some code inside the RunSomeLogic method later.

    Now we are going to implement the **AlgorandHandler** class we defined above.
    ```c
    ……..
    #include "AlgorandHandler.h"

    UAlgorandHandler::UAlgorandHandler() { 
        algorandManager = CreateDefaultSubobject<UAlgorandUnrealManager>(TEXT(“AlgorandManager”));
        ...
    }

    UWorld* UAlgorandHandler::GetWorld() const { return GetOuter()->GetWorld(); }

    void UAlgorandHandler::RunSomeLogic() {
    //TODO: getbalance, payment tx , and so on…
    }

    ```
    Now, let’s rewrite something interesting inside the RunSomeLogic method.
    ```c
    …….
    void UAlgorandHandler::RunSomeLogic() {
        switch (Result_Type)
    	{
        case EResultType::InitWallet:
            algorandManager->initWallet();
            break;
        case EResultType::IsWalletExisted:
            algorandManager->isWalletExisted();
            break;
        case EResultType::LoadWallet:
            algorandManager->loadWallet(Password);
            break;
        case EResultType::SaveWallet:
            algorandManager->saveWallet(Password);
            break;
        case EResultType::GetMnemonics:
            algorandManager->getMnemonicsByAccountName(AccountName);
            break;
        case EResultType::GetAllAccounts:
            algorandManager->getAllAccounts();
            break;
        case EResultType::GenAccount:
            algorandManager->generateAccountFromMnemonics(Mnemonics, AccountName);
            break;
        case EResultType::GenRandomAccount:
            algorandManager->generateRandomAccount(AccountName);
            break;
        case EResultType::AddrBalance:
            algorandManager->getAddressBalance(Address);
            break;
        case EResultType::PayTx:
            algorandManager->sendPaymentTransaction(AccountName,
                                                    Address,
                                                    Amount, FString(
                                                        "Sent 100 algo to " + Address));
            break;
        case EResultType::AcfgTx:
            algorandManager->sendAssetConfigTransaction(AccountName,
                FString("Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA"),
                FString("Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA"),
                FString("Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA"),
                FString("Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA"),
                0,
                1000,
                8,
                "false",
                FString("USD"),
                FString("Shosha"),
                FString("https://myurl.com"),
                FString("fe"));
            break;
        case EResultType::AxferTx:
            algorandManager->sendAssetTransferTransaction(AccountName,
                Address,
                AssetID,
                Amount,
                FString("Sent tokens to " + Address));

            break;
        case EResultType::ApplTx:
            {
                // Application Call - Math Calculation through Smart Contract on algorand testnet node
                TArray<FAppArg> app_args;
                EAppOnCompleteTX app_complete_tx = EAppOnCompleteTX::NOOP;

                app_args.Add(UAppNoOpTXFactory::FAppArgFromString(EAppArgType::ARG_METHOD, "mul(uint64,uint64)uint64"));
                app_args.Add(UAppNoOpTXFactory::FAppArgFromString(EAppArgType::ARG_INT, "1000"));
                app_args.Add(UAppNoOpTXFactory::FAppArgFromString(EAppArgType::ARG_INT, "2"));

                algorandManager->sendApplicationCallTransaction(AccountName, 301624623, app_args, app_complete_tx);
                break;
            }
        case EResultType::ArcAsset:
            algorandManager->fetchArcAssetDetails(AssetID);
            break;
        case EResultType::AccAssets:
            algorandManager->fetchAccountInformation("6WII6ES4H6UW7G7T7RJX63CUNPKJEPEGQ3PTYVVU3JHJ652W34GCJV5OVY");
            break;
        case EResultType::RemAccount:
            algorandManager->removeAccountByName(AccountName);
            break;
        case EResultType::NONE:
            break;
        }
    }
    ……
    ```
2. Creating an AlgorandCharacter

    Tools > New C++ Class... > Common Classes > Search and select Character.

    <img src="Resources/11.png">

    Then create a new character named AlgorandCharacter.

    <img src="Resources/12.png">

    The last thing we need to do is to make use of our UAlgorandHandler. Open …Character.h and import AlgorandHandler.h and add lines below to the class declaration.
    ```c
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = 
    "true"))UAlgorandHandler* algorandHandler;

    And now add object initialization code into the character’s class constructor 
    definition:

    algorandHandler = 
    CreateDefaultSubobject<UAlgorandHandler>(TEXT("AlgorandHandler"));

    Add RunSomeLogic call to the end of BeginPlay event definition:

    algorandHandler->RunSomeLogic();
    ```
    Now we need to compile code and run the game. 

    <img src="Resources/13.png">

    The balance of our wallet will be printed on Output Logs.

    **Working with Blueprint**

    After removing algorandCharacter, let’s continue to work on the above project.

    Create an empty blueprint class named ***BP_Algorand***.

    <img src="Resources/14.png">

    On the Pick Parent Class panel, select the Actor item and create a blank blueprint class.

    <img src="Resources/15.png">

    Now we see the blueprint editor, let’s create a variable `algorandManager` with the type `Algorand Unreal Manager`.

    Let’s make *ConvertFUInt64ToString* and *ConvertFStringToUInt6* 4 like the above.

    <img src="Resources/16.png">

    <img src="Resources/18.png">

    Well, now we can call the GetBalance function and await the result. 

    Add `BindEventGetBalanceCallback` to the event graph (right after manager initialization or after manager initialization) and set its **Event** field to callback events via broadcast.

    <img src="Resources/17.png">
