#include "vkey.h"
#include "const.h"
#include "keyboard.h"

bool IsKeyArrow(VirtualKey key){
    if(key==UP || key==DOWN
            || key==LEFT || key==RIGHT){
        return true;
    }
    return false;
}

bool IsKeyF(VirtualKey key){
    if(key==F1 || key==F2
            || key==F3 || key==F4
            || key==F5 || key==F6
            || key==F7 || key==F8
            || key==F9 || key==F10
            || key==F11 || key==F12){
        return true;
    }
    return false;
}

bool IsKeyControllers(VirtualKey key){
    if(key==ALT_L || key==ALT_R || key==SHIFT_L || key==SHIFT_R || key==CAPS_LOCK){
        return true;
    }
    return false;
}

bool IsKeyControl(VirtualKey key){
    if(key==CTRL_L || key==CTRL_R){
        return true;
    }

    return false;
}

bool IsKeyDelete(VirtualKey key){
    if(key==DELETE){
        return true;
    }

    return false;
}

bool IsKeyReturn(VirtualKey key){
    if(key==ENTER){
        return true;
    }

    return false;
}

bool IsKeyBackspace(VirtualKey key){
    if(key==BACKSPACE){
        return true;
    }

    return false;
}

bool IsKeyControlModifiers(VirtualKey key){
    if(key==CTRL_L || key==CTRL_R){
        return true;
    }

    return false;
}

bool IsKeyPageUp(VirtualKey key){
    if(key==PAGEUP){
        return true;
    }

    return false;
}

bool IsKeyPageDown(VirtualKey key){
    if(key==PAGEDOWN){
        return true;
    }

    return false;
}

bool IsKeyHome(VirtualKey key){
    if(key==HOME){
        return true;
    }

    return false;
}

bool IsKeyEnd(VirtualKey key){
    if(key==END){
        return true;
    }

    return false;
}

bool IsKeySwitchTty(VirtualKey key, VirtualKey modify){
    if(modify!=CTRL_L && modify!=CTRL_R){
        return false;
    }
    if(key==F1 || key==F2 ||
            key==F3 || key==F4 ||
            key==F5 || key==F6 ||
            key==F7 || key==F8){
        return true;
    }

    return false;
}

