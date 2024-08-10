#include <stdio.h>
#include <dbus/dbus.h>
#include <stdlib.h>


int main() {
    DBusError error;

    dbus_error_init(&error);
    DBusConnection *connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "DBus Connection error: %s", error.message);
        dbus_error_free(&error);
    }

    if (connection == NULL) {
        exit(EXIT_FAILURE);
    }
    

    printf("Connected to D-Bus system bus!\n");
    

    dbus_connection_unref(connection);

    return 0;
}
