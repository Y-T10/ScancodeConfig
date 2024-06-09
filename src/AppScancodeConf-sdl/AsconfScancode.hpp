#pragma once

// 列挙体->どこでも使う、汎用性あり
// 値と名前の変換->カスタマイズできるとよい->後々で外部から入れる
// 値と画像の対->後々で外部から入れる
// これらのコードは`Scancode Set 1`と対応する．

#include "SDL_keycode.h"
#ifndef ENABLE_F24
    #define ENABLE_F24 0
#endif

#ifndef ENABLE_HankakuZenkaku
    #define ENABLE_HankakuZenkaku 1
#endif

enum WinScancode {
    // Do not use   = 0x0000,
    Escape          = 0x0001,
    KB1             = 0x0002,
    KB2             = 0x0003,
    KB3             = 0x0004,
    KB4             = 0x0005,
    KB5             = 0x0006,
    KB6             = 0x0007,
    KB7             = 0x0008,
    KB8             = 0x0009,
    KB9             = 0x000A,
    KB0             = 0x000B,
    Minus           = 0x000C,
    Equal           = 0x000D,
    BackSpace       = 0x000E, 
    Tab             = 0x000F,
    Q               = 0x0010,
    W               = 0x0011,
    E               = 0x0012,
    R               = 0x0013,
    T               = 0x0014,
    Y               = 0x0015,
    U               = 0x0016,
    I               = 0x0017,
    O               = 0x0018,
    P               = 0x0019,
    LSquareBracket  = 0x001A,
    RSquareBracket  = 0x001B,
    Enter           = 0x001C, 
    LCtrl           = 0x001D, 
    A               = 0x001E,
    S               = 0x001F,
    D               = 0x0020,
    F               = 0x0021,
    G               = 0x0022,
    H               = 0x0023,
    J               = 0x0024,
    K               = 0x0025,
    L               = 0x0026,
    Semicolon       = 0x0027,
    Apostrophe      = 0x0028,
    Grave           = 0x0029,
    LShift          = 0x002A,
    Slash           = 0x002B,
    Z               = 0x002C,
    X               = 0x002D,
    C               = 0x002E,
    V               = 0x002F,
    B               = 0x0030,
    N               = 0x0031,
    M               = 0x0032,
    Comma           = 0x0033,
    Period          = 0x0034,
    BackSlash       = 0x0035,
    RShift          = 0x0036,
    PadAsterisk     = 0x0037,
    LAlt            = 0x0038,
    Space           = 0x0039,
    CapsLock        = 0x003A,
    F1              = 0x003B,
    F2              = 0x003C,
    F3              = 0x003D,
    F4              = 0x003E,
    F5              = 0x003F,
    F6              = 0x0040,
    F7              = 0x0041,
    F8              = 0x0042,
    F9              = 0x0043,
    F10             = 0x0044,
    NumLock         = 0x0045,
    ScrLk           = 0x0046,
    Pad7            = 0x0047,
    Pad8            = 0x0048,
    Pad9            = 0x0049,
    PadMinus        = 0x004A,
    Pad4            = 0x004B,
    Pad5            = 0x004C,
    Pad6            = 0x004D,
    PadPlus         = 0x004E,
    Pad1            = 0x004F,
    Pad2            = 0x0050,
    Pad3            = 0x0051,
    Pad0            = 0x0052,
    PadPeriod       = 0x0053,
    SysRq           = 0x0054,
    #ifdef ENABLE_UNASSIGNED_CODE
    Unassigned      = 0x0055,
    #endif
    BackSlashNonUS  = 0x0056,
    F11             = 0x0057,
    F12             = 0x0058,
    PadEqual        = 0x0059,
    #ifdef ENABLE_UNASSIGNED_CODE
    Unassigned5A    = 0x005A,
    Unassigned5B    = 0x005B,
    #endif
    PadComma        = 0x005C,
    // not assigned = 0x005D,
    // not assigned = 0x005E,
    // not assigned = 0x005F,
    // Do not use   = 0x0060,
    // Do not use   = 0x0061,
    //
    #ifdef ENABLE_UNASSIGNED_CODE
    Unassigned62    = 0x0062,
    Unassigned63    = 0x0063,
    #endif
    F13             = 0x0064,
    F14             = 0x0065,
    F15             = 0x0066,
    F16             = 0x0067,
    F17             = 0x0068,
    F18             = 0x0069,
    F19             = 0x006A,
    F20             = 0x006B,
    F21             = 0x006C,
    F22             = 0x006D,
    F23             = 0x006E,
    #ifdef ENABLE_UNASSIGNED_CODE
    Unassigned6F    = 0x006F,
    #endif
    Kana            = 0x0070,
    #ifdef ENABLE_UNASSIGNED_CODE
    Unassigned71    = 0x0071,
    Unassigned72    = 0x0072,
    #endif
    BackSlashBarBRJP= 0x0073,
    #ifdef ENABLE_UNASSIGNED_CODE
    Unassigned74    = 0x0074,
    Unassigned75    = 0x0075,
    #endif
    #if (ENABLE_F24) || (ENABLE_HankakuZenkaku)
        #if ENABLE_F24
        F24         = 0x0076,
        #endif
        #if ENABLE_HankakuZenkaku
        HankakuZenkaku  = 0x0076,
        #endif
    #else
        #error "Define ENABLE_F24 or ENABLE_HankakuZenkaku with 1!"
    #endif
    Hiragana        = 0x0077,
    Katakana        = 0x0078,
    ConvertJP       = 0x0079,
    // Do not use   = 0x007A,
    NonConvertJP    = 0x007B,
    // Do not use   = 0x007C,
    YenJP           = 0x007D,
    PadCommaBR      = 0x007E,
    // Do not use   = 0x007F,
    /* single byte scan code はここまで */
    HangleEng       = 0x00F2,
    Hanja           = 0x00F1,
    /* ここから前置修飾付コード */

    ScanPrevTrack   = 0xE010,
    ScanNextTrack   = 0xE019,
    PadEnter        = 0xE01C, 
    RCtrl           = 0xE01D,
    Mute            = 0xE020,
    Calc            = 0xE021,
    PlayPause       = 0xE022,
    Stop            = 0xE024,
    VolumeDowm      = 0xE02E,
    VolumeUp        = 0xE030,
    WWWHome         = 0xE032,
    PadSlash        = 0xE035, 
    PrintScreen     = 0xE037,
    RAlt            = 0xE038,
    // キー変換の都合でこの値を割り当てている
    Pause           = 0xE045,
    Break           = 0xE046,
    Home            = 0xE047, 
    UpArrow         = 0xE048, 
    PageUp          = 0xE049, 
    LeftArrow       = 0xE04B, 
    RightArrow      = 0xE04D, 
    End             = 0xE04F, 
    DownArrow       = 0xE050, 
    PageDown        = 0xE051, 
    Insert          = 0xE052, 
    Delete          = 0xE053, 
    LGUI            = 0xE05B, 
    RGUI            = 0xE05C,
    App             = 0xE05D,
    Power           = 0xE05E,
};