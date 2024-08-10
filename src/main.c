#include "dbus/dbus-protocol.h"
#include <stdio.h>
#include <dbus/dbus.h>
#include <stdlib.h>


int main() {
    DBusError error;

    dbus_error_init(&error);
    DBusConnection *connection = dbus_bus_get(DBUS_BUS_SESSION, &error);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "DBus Connection error: %s", error.message);
        dbus_error_free(&error);
    }

    if (connection == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("Connected to D-Bus system bus!\n");
    
    DBusMessage *message = dbus_message_new_method_call(
        "org.freedesktop.Notifications",
        "/org/freedesktop/Notifications",
        "org.freedesktop.Notifications",
        "GetCapabilities"
    );

    DBusMessage *reply = dbus_connection_send_with_reply_and_block(connection, message, -1, &error);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Failed to send message %s", error.message);
        exit(EXIT_FAILURE);
    }

    dbus_message_unref(message);

    if (reply) {
        int msg_type = dbus_message_get_type(reply);
        
        DBusMessageIter iter;
        dbus_message_iter_init(reply, &iter);

        // dbus_message_iter_recurse();
        // DBusMessageIter sub;

        while (dbus_message_iter_has_next(&iter)) {
            char *message;
            dbus_message_iter_get_basic(&iter, &message);
            dbus_message_iter_next(&iter);
            printf("%s\n", message);
        }
    }

 

    dbus_connection_unref(connection);

    return 0;
}
