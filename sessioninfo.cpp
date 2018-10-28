#include "sessioninfo.h"

QString SessionInfo::APIurl = "";
QString SessionInfo::loginInfo = "";
int SessionInfo::permissions = 0;

SessionInfo::SessionInfo()
{

}

bool SessionInfo::checkPermissions(int permission_mask)
{
    return (permissions & permission_mask) == permission_mask;
}
