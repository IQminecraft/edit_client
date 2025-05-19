#include "itemget.h"
#include <string>

// コマンドハンドラー
bool HandleCommand(const std::string& command) {
    if (command == ".itemget") {
        if (!initialized) {
            InitializeItemGet();
        }
        CollectAllItems();
        ToggleItemList();
        return true;
    }
    return false;
}

// コマンドの登録
void RegisterCommands() {
    // ここでコマンドを登録
    // 実際の実装はゲームのコマンドシステムに依存
} 