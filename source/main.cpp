#include <iostream>
#include "../include/KmboxNet.hpp"

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
    ConResult result = kmbox.move_mouse(100, 100);
    
    // 检查移动结果
    if (result == ConResult::SUCCESS) {
        std::cout << "鼠标移动成功！" << std::endl;
        return true;
    } else {
        std::cout << "鼠标移动失败，错误码：" << static_cast<int>(result) << std::endl;
        return false;
    }
}

int main() {
    // 测试连接
    if (!test_connect()) {
        return -1;
    }
    
    // 创建KmboxNet实例
    KmboxNet kmbox("192.168.1.100", "8080", "AABBCCDD");
    
    // 连接
    if (kmbox.connect() != ConResult::SUCCESS) {
        std::cout << "连接失败！" << std::endl;
        return -1;
    }
    
    // 测试鼠标移动
    if (!test_mouse_move(kmbox)) {
        return -1;
    }
    
    return 0;
}



