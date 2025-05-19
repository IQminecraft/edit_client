#include "itemget.h"
#include <Psapi.h>
#include <iostream>

// グローバル変数の定義
std::vector<ItemInfo> allItems;
bool showItemList = false;
bool initialized = false;

// オリジナル関数のポインタ
typedef void (*RegisterItemFn)(void* registry, void* itemInfo);
RegisterItemFn originalRegisterItem = nullptr;

// アイテム登録関数のフック
void hookedRegisterItem(void* registry, void* itemInfo) {
    if (originalRegisterItem) {
        originalRegisterItem(registry, itemInfo);
    }
    
    // アイテム情報を保存
    ItemInfo* info = static_cast<ItemInfo*>(itemInfo);
    allItems.push_back(*info);
}

// メモリパターンスキャン関数
uintptr_t FindPattern(const char* module, const char* pattern, const char* mask) {
    MODULEINFO modInfo;
    GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module), &modInfo, sizeof(MODULEINFO));
    
    uintptr_t start = (uintptr_t)modInfo.lpBaseOfDll;
    uintptr_t end = start + modInfo.SizeOfImage;
    
    for (uintptr_t i = start; i < end; ++i) {
        bool found = true;
        for (size_t j = 0; j < strlen(mask); ++j) {
            if (mask[j] == 'x' && *(char*)(i + j) != pattern[j]) {
                found = false;
                break;
            }
        }
        if (found) return i;
    }
    return 0;
}

// アイテムレジストリ関数のアドレス特定
LPVOID FindRegisterItemFunction() {
    // 実際のパターンはバイナリ解析により特定する必要がある
    const char* pattern = "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54"; // 例
    const char* mask = "xxxx?xxxxx";
    return (LPVOID)FindPattern("Minecraft.Windows.exe", pattern, mask);
}

// 初期化関数
void InitializeItemGet() {
    if (initialized) return;

    // MinHookの初期化
    if (MH_Initialize() != MH_OK) {
        std::cout << "MinHookの初期化に失敗しました" << std::endl;
        return;
    }

    // アイテム登録関数のアドレスを特定
    LPVOID registerItemAddr = FindRegisterItemFunction();
    if (!registerItemAddr) {
        std::cout << "アイテム登録関数のアドレスが見つかりません" << std::endl;
        return;
    }

    // フックの作成
    if (MH_CreateHook(registerItemAddr, &hookedRegisterItem, 
        reinterpret_cast<LPVOID*>(&originalRegisterItem)) != MH_OK) {
        std::cout << "フックの作成に失敗しました" << std::endl;
        return;
    }

    // フックの有効化
    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
        std::cout << "フックの有効化に失敗しました" << std::endl;
        return;
    }

    initialized = true;
    std::cout << "ItemGet機能が初期化されました" << std::endl;
}

// 終了処理
void ShutdownItemGet() {
    if (!initialized) return;

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    initialized = false;
    allItems.clear();
}

// アイテムリストの表示
void RenderItemList() {
    if (!showItemList) return;

    ImGui::Begin("アイテムリスト", &showItemList);
    
    if (ImGui::BeginTabBar("ItemListTabs")) {
        if (ImGui::BeginTabItem("アイテム一覧")) {
            static char searchText[256] = "";
            ImGui::InputText("検索", searchText, IM_ARRAYSIZE(searchText));
            
            ImGui::BeginChild("ItemList", ImVec2(0, 0), true);
            for (const auto& item : allItems) {
                if (strlen(searchText) == 0 || 
                    item.name.find(searchText) != std::string::npos) {
                    ImGui::Text("名前: %s", item.name.c_str());
                    ImGui::Text("ID: %d", item.id);
                    ImGui::Text("説明: %s", item.description.c_str());
                    ImGui::Separator();
                }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

// アイテムリストの表示/非表示を切り替え
void ToggleItemList() {
    showItemList = !showItemList;
}

// 全アイテムの収集
void CollectAllItems() {
    allItems.clear();
    // ここでアイテムレジストリから全アイテムを収集
    // 実際の実装はゲームの構造に依存
} 