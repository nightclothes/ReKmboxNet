#include <iostream>
#include "../include/KmboxNet.hpp"
#include <vector>
#include <thread>
#include <chrono>

/**
 * @brief 测试鼠标移动功能
 * @param kmbox KmboxNet实例
 * @return 测试结果
 */
bool test_mouse_movement(KmboxNet& kmbox) {
    std::cout << "开始测试鼠标移动功能..." << std::endl;
    
    // 测试一系列移动
    std::vector<std::pair<int, int>> movements = {
        {100, 100},   // 右下
        {-100, -100}, // 左上
        {100, -100},  // 右上
        {-100, 100}   // 左下
    };
    
    for (const auto& move : movements) {
        std::cout << "移动鼠标到: (" << move.first << ", " << move.second << ")" << std::endl;
        ConResult result = kmbox.mouse_move(move.first, move.second);
        
        if (result != ConResult::SUCCESS) {
            std::cout << "鼠标移动失败，错误码：" << static_cast<int>(result) << std::endl;
            return false;
        }
        
        // 移动之间添加延迟
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout << "鼠标移动测试完成！" << std::endl;
    return true;
}

/**
 * @brief 测试键盘基本功能
 * @param kmbox KmboxNet实例
 * @return 测试结果
 */
bool test_keyboard_basic(KmboxNet& kmbox) {
    std::cout << "开始测试键盘基本功能..." << std::endl;
    
    // 测试基本按键
    std::vector<KeyboardTable> basic_keys = {
        KeyboardTable::A,
        KeyboardTable::B,
        KeyboardTable::C,
        KeyboardTable::SPACEBAR,
        KeyboardTable::ENTER
    };
    
    for (const auto& key : basic_keys) {
        std::cout << "测试按键: " << static_cast<int>(key) << std::endl;
        
        // 测试按下和释放
        ConResult down_result = kmbox.keyboard_down(key);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ConResult up_result = kmbox.keyboard_up(key);
        
        if (down_result != ConResult::SUCCESS || up_result != ConResult::SUCCESS) {
            std::cout << "键盘测试失败，错误码：" << static_cast<int>(down_result) << ", " 
                     << static_cast<int>(up_result) << std::endl;
            return false;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout << "键盘基本功能测试完成！" << std::endl;
    return true;
}

/**
 * @brief 测试组合键功能
 * @param kmbox KmboxNet实例
 * @return 测试结果
 */
bool test_keyboard_combination(KmboxNet& kmbox) {
    std::cout << "开始测试组合键功能..." << std::endl;
    
    // 测试Ctrl+A
    std::cout << "测试 Ctrl+A 组合键" << std::endl;
    
    ConResult result = kmbox.keyboard_down(KeyboardTable::LEFTCONTROL);
    if (result != ConResult::SUCCESS) return false;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    result = kmbox.keyboard_down(KeyboardTable::A);
    if (result != ConResult::SUCCESS) return false;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    result = kmbox.keyboard_up(KeyboardTable::A);
    if (result != ConResult::SUCCESS) return false;
    
    result = kmbox.keyboard_up(KeyboardTable::LEFTCONTROL);
    if (result != ConResult::SUCCESS) return false;
    
    std::cout << "组合键测试完成！" << std::endl;
    return true;
}

/**
 * @brief 测试键盘点击功能
 * @param kmbox KmboxNet实例
 * @return 测试结果
 */
bool test_keyboard_click(KmboxNet& kmbox) {
    std::cout << "开始测试键盘点击功能..." << std::endl;
    
    // 测试不同延迟的点击
    std::vector<std::pair<KeyboardTable, int>> click_tests = {
        {KeyboardTable::A, 100},
        {KeyboardTable::B, 200},
        {KeyboardTable::C, 300}
    };
    
    for (const auto& test : click_tests) {
        std::cout << "测试点击按键: " << static_cast<int>(test.first) 
                 << " 延迟: " << test.second << "ms" << std::endl;
        
        ConResult result = kmbox.Keyboard_click(test.first, test.second);
        
        if (result != ConResult::SUCCESS) {
            std::cout << "键盘点击测试失败，错误码：" << static_cast<int>(result) << std::endl;
            return false;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout << "键盘点击功能测试完成！" << std::endl;
    return true;
}

int main() {
    try {
        std::cout << "开始KmboxNet功能测试..." << std::endl;
        
        // 创建KmboxNet实例
        KmboxNet kmbox("192.168.2.188", "33792", "87585C53");
        
        // 连接设备
        std::cout << "正在连接设备..." << std::endl;
        ConResult connect_result = kmbox.connect();
        if (connect_result != ConResult::SUCCESS) {
            std::cout << "连接失败，错误码：" << static_cast<int>(connect_result) << std::endl;
            return -1;
        }
        std::cout << "设备连接成功！" << std::endl;
        
        // 执行测试ABC
        
        bool test_results[] = {
            test_mouse_movement(kmbox),
            test_keyboard_basic(kmbox),
            test_keyboard_combination(kmbox),
            test_keyboard_click(kmbox)
        };
        
        // 检查测试结果
        bool all_passed = true;
        for (int i = 0; i < 4; i++) {
            if (!test_results[i]) {
                std::cout << "测试 " << i + 1 << " 失败" << std::endl;
                all_passed = false;
            }
        }
        
        // 断开连接
        kmbox.disconnect();
        std::cout << "设备已断开连接" << std::endl;
        
        if (all_passed) {
            std::cout << "所有测试通过！" << std::endl;
            return 0;
        } else {
            std::cout << "部分测试失败" << std::endl;
            return -1;
        }
        
    } catch (const std::exception& e) {
        std::cout << "测试过程中发生异常: " << e.what() << std::endl;
        return -1;
    }
}



