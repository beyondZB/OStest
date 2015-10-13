#ifndef VIRTUAL_KEY_H
#define VIRTUAL_KEY_H

#include "type.h"
#include "const.h"

bool IsKeyF(VirtualKey key);
bool IsKeyArrow(VirtualKey key);
bool IsKeyDelete(VirtualKey key);
bool IsKeyControl(VirtualKey key);
bool IsKeyControllers(VirtualKey key);
bool IsKeyBackspace(VirtualKey key);
bool IsKeyReturn(VirtualKey key);
bool IsKeyControlModifiers(VirtualKey key);
bool IsKeyPageUp(VirtualKey key);
bool IsKeyPageDown(VirtualKey key);
bool IsKeyHome(VirtualKey key);
bool IsKeyEnd(VirtualKey key);
bool IsKeySwitchTty(VirtualKey k, VirtualKey modify);

#endif // VIRTUAL_KEY_H

