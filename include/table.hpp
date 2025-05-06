/*
    键盘信号表 
    TODO 需要检查
*/
enum class KeyboardTable {
    /**
     * 无按键
     */
    key_none = 0x00,
    
    /**
     * 按键错误
     */
    key_error_rollover = 0x01,
    key_post_fail = 0x02,
    key_error_undefined = 0x03,
    
    /**
     * 字母按键
     */
    key_a = 0x04,
    key_b = 0x05,
    key_c = 0x06,
    key_d = 0x07,
    key_e = 0x08,
    key_f = 0x09,
    key_g = 0x0A,
    key_h = 0x0B,
    key_i = 0x0C,
    key_j = 0x0D,
    key_k = 0x0E,
    key_l = 0x0F,
    key_m = 0x10,
    key_n = 0x11,
    key_o = 0x12,
    key_p = 0x13,
    key_q = 0x14,
    key_r = 0x15,
    key_s = 0x16,
    key_t = 0x17,
    key_u = 0x18,
    key_v = 0x19,
    key_w = 0x1A,
    key_x = 0x1B,
    key_y = 0x1C,
    key_z = 0x1D,
    
    /**
     * 数字和符号按键
     */
    key_1_exclamation = 0x1E,
    key_2_at = 0x1F,
    key_3_hash = 0x20,
    key_4_dollar = 0x21,
    key_5_percent = 0x22,
    key_6_caret = 0x23,
    key_7_ampersand = 0x24,
    key_8_asterisk = 0x25,
    key_9_left_paren = 0x26,
    key_0_right_paren = 0x27,
    
    /**
     * 控制按键
     */
    key_enter = 0x28,
    key_escape = 0x29,
    key_backspace = 0x2A,
    key_tab = 0x2B,
    key_space = 0x2C,
    key_minus_underscore = 0x2D,
    key_equal_plus = 0x2E,
    key_left_bracket = 0x2F,
    key_right_bracket = 0x30,
    key_backslash = 0x31,
    key_hash_tilde = 0x32,
    key_semicolon_colon = 0x33,
    key_apostrophe_quote = 0x34,
    key_grave_tilde = 0x35,
    key_comma_less = 0x36,
    key_period_greater = 0x37,
    key_slash_question = 0x38,
    key_caps_lock = 0x39,
    
    /**
     * 功能按键
     */
    key_f1 = 0x3A,
    key_f2 = 0x3B,
    key_f3 = 0x3C,
    key_f4 = 0x3D,
    key_f5 = 0x3E,
    key_f6 = 0x3F,
    key_f7 = 0x40,
    key_f8 = 0x41,
    key_f9 = 0x42,
    key_f10 = 0x43,
    key_f11 = 0x44,
    key_f12 = 0x45,
    
    /**
     * 控制和导航按键
     */
    key_print_screen = 0x46,
    key_scroll_lock = 0x47,
    key_pause = 0x48,
    key_insert = 0x49,
    key_home = 0x4A,
    key_page_up = 0x4B,
    key_delete = 0x4C,
    key_end = 0x4D,
    key_page_down = 0x4E,
    key_right_arrow = 0x4F,
    key_left_arrow = 0x50,
    key_down_arrow = 0x51,
    key_up_arrow = 0x52,
    
    /**
     * 数字键盘
     */
    key_num_lock = 0x53,
    key_keypad_slash = 0x54,
    key_keypad_asterisk = 0x55,
    key_keypad_minus = 0x56,
    key_keypad_plus = 0x57,
    key_keypad_enter = 0x58,
    key_keypad_1_end = 0x59,
    key_keypad_2_down = 0x5A,
    key_keypad_3_page_down = 0x5B,
    key_keypad_4_left = 0x5C,
    key_keypad_5 = 0x5D,
    key_keypad_6_right = 0x5E,
    key_keypad_7_home = 0x5F,
    key_keypad_8_up = 0x60,
    key_keypad_9_page_up = 0x61,
    key_keypad_0_insert = 0x62,
    key_keypad_decimal = 0x63,
    
    /**
     * 修饰键
     */
    key_left_control = 0xE0,
    key_left_shift = 0xE1,
    key_left_alt = 0xE2,
    key_left_gui = 0xE3,
    key_right_control = 0xE4,
    key_right_shift = 0xE5,
    key_right_alt = 0xE6,
    key_right_gui = 0xE7
};

/*
    鼠标信号表
    TODO 检查
*/
enum class MouseTable {
    /**
     * 按键位定义
     */
    button_none = 0x00,       // 无按键
    button_left = 0x01,       // 左键
    button_right = 0x02,      // 右键
    button_middle = 0x04,     // 中键
    button_side1 = 0x08,      // 侧键1
    button_side2 = 0x10,      // 侧键2
    
    /**
     * 鼠标轴定义
     */
    axis_x = 0x01,            // X轴
    axis_y = 0x02,            // Y轴
    axis_wheel = 0x04,        // 滚轮
    
    /**
     * 屏蔽位定义
     */
    mask_left = 0x01,         // 屏蔽左键
    mask_right = 0x02,        // 屏蔽右键
    mask_middle = 0x04,       // 屏蔽中键
    mask_side1 = 0x08,        // 屏蔽侧键1
    mask_side2 = 0x10,        // 屏蔽侧键2
    mask_x = 0x20,            // 屏蔽X轴移动
    mask_y = 0x40,            // 屏蔽Y轴移动
    mask_wheel = 0x80         // 屏蔽滚轮
};






