#include <iostream>
#include "../include/KmboxNet.hpp"
#include <vector>
#include <thread>
#include <chrono>

/**
 * @brief 测试KmboxNet连接功能
 * @return 测试结果
 */
bool test_connect() {
    // 创建KmboxNet实例
    KmboxNet kmbox("192.168.2.188", "33792", "87585C53");
    
    // 尝试连接
    ConResult result = kmbox.connect();
    
    // 检查连接结果
    if (result == ConResult::SUCCESS) {
        std::cout << "连接成功！" << std::endl;
        return true;
    } else {
        std::cout << "连接失败，错误码：" << static_cast<int>(result) << std::endl;
        return false;
    }
}

/**
 * @brief 测试鼠标移动功能
 * @param kmbox KmboxNet实例
 * @return 测试结果
 */
bool test_mouse_move(KmboxNet& kmbox) {
    // 测试鼠标移动到(100, 100)
    ConResult result = kmbox.mouse_move(100, 100);
    
    // 检查移动结果
    if (result == ConResult::SUCCESS) {
        std::cout << "鼠标移动成功！" << std::endl;
        return true;
    } else {
        std::cout << "鼠标移动失败，错误码：" << static_cast<int>(result) << std::endl;
        return false;
    }
}

/**
 * @brief 测试键盘click功能
 * @param kmbox KmboxNet实例
 * @return 测试结果
 */
bool test_keyboard_click(KmboxNet& kmbox) {
    // 定义要测试的按键列表
    std::vector<KeyboardTable> test_keys = {
        // 普通按键测试
        KeyboardTable::A,
        KeyboardTable::A,
        KeyboardTable::A,
    };
    
    // 测试每个按键
    for (const auto& key : test_keys) {
        std::cout << "测试按键: " << static_cast<int>(key) << std::endl;
        
        // 点击按键，延迟100ms
        ConResult result = kmbox.Keyboard_click(key, 100);
        
        // 检查结果
        if (result != ConResult::SUCCESS) {
            std::cout << "按键点击失败，错误码：" << static_cast<int>(result) << std::endl;
            return false;
        }
        
        // 按键之间添加延迟
        std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    }
    
    std::cout << "所有按键测试完成！" << std::endl;
    return true;
}

int main() {
    // 测试连接
    if (!test_connect()) {
        return -1;
    }
    
    // 创建KmboxNet实例
    KmboxNet kmbox("192.168.2.188", "33792", "87585C53");
    
    // 连接
    if (kmbox.connect() != ConResult::SUCCESS) {
        std::cout << "连接失败！" << std::endl;
        return -1;
    }
    
    // 测试鼠标移动
    if (!test_mouse_move(kmbox)) {
        return -1;
    }
    
    // 测试键盘click
    if (!test_keyboard_click(kmbox)) {
        return -1;
    }
    
    return 0;

}



