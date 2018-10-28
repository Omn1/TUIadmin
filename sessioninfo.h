#ifndef SESSIONINFO_H
#define SESSIONINFO_H

#include <QString>

class SessionInfo
{
public:
    SessionInfo();

    static QString APIurl;
    static QString loginInfo;
    static int permissions;

    static bool checkPermissions(int permission_mask);

    static const int COOK_PERMISSION = 1 << 0;
    static const int DRIVER_PERMISSION = 1 << 1;
    static const int WAREHOUSE_MGR_PERMISSION = 1 << 2;
    static const int PERSONNEL_MGR_PERMISSION = 1 << 3;
    static const int MENU_MGR_PERMISSION = 1 << 4;
    static const int OPERATOR_PERMISSION = 1 << 5;
};

#endif // SESSIONINFO_H
