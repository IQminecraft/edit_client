#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <MinHook.h>
#include <imgui.h>
#include <d3d11.h>

// アイテム情報の構造体
struct ItemInfo {
    std::string name;
    int id;
    std::string description;
    // 必要に応じて他の情報を追加
};

// グローバル変数
extern std::vector<ItemInfo> allItems;
extern bool showItemList;
extern bool initialized;

// 関数プロトタイプ
void InitializeItemGet();
void ShutdownItemGet();
void RenderItemList();
void ToggleItemList();
void CollectAllItems(); 