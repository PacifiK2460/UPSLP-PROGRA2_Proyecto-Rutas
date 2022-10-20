#include "logic.h"

Result AdminPanel(User *admin)
{
    listWidget widgets = {0};
    Widget ManageUser;
    Widget ManageRoutes;
    Widget ConsultLogs;

    Widget Logs;

    llist_append(&widgets, &ManageUser);
    llist_append(&widgets, &ManageRoutes);
    llist_append(&widgets, &ConsultLogs);
    llist_append(&widgets, &Logs);

    focus(widgets, NULL, NULL);
}