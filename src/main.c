#include "dbus/dbus-protocol.h"
#include <stdio.h>
#include <dbus/dbus.h>
#include <stdlib.h>
#include <stdbool.h>


char *get_display_device_path(DBusConnection *connection) {
    static char *object_path;

    if (object_path != NULL) {
        return object_path;
    }

    DBusError error;
    dbus_error_init(&error);

    DBusMessage *message = dbus_message_new_method_call(
        "org.freedesktop.UPower",
        "/org/freedesktop/UPower",
        "org.freedesktop.UPower",
        "GetDisplayDevice"
    );

    DBusMessage *reply = dbus_connection_send_with_reply_and_block(connection, message, -1, &error);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Failed to send message %s", error.message);
        exit(EXIT_FAILURE);
    }

    dbus_message_unref(message);

    
    DBusMessageIter iter;
    dbus_message_iter_init(reply, &iter);

    if (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_OBJECT_PATH) {
        fprintf(stderr, "Invalid reply arg type %c", dbus_message_iter_get_arg_type(&iter));
        exit(EXIT_FAILURE);
    }

    dbus_message_iter_get_basic(&iter, &object_path);

    return object_path;
}

enum PowerSourceType {
    POWER_UNKNOWN,
    POWER_LINE_POWER,
    POWER_BATTERY,
    POWER_UPS,
    POWER_MONITOR,
    POWER_MOUSE,
    POWER_KEYBOARD,
    POWER_PDA,
    POWER_PHONE,
    POWER_MEDIA_PLAYER,
    POWER_TABLET,
    POWER_COMPUTER,
    POWER_GAMING_INPUT,
    POWER_PEN,
    POWER_TOUCHPAD,
    POWER_MODEM,
    POWER_NETWORK,
    POWER_HEADSET,
    POWER_SPEAKERS,
    POWER_HEADPHONES,
    POWER_VIDEO,
    POWER_OTHER_AUDIO,
    POWER_REMOTE_CONTROL,
    POWER_PRINTER,
    POWER_SCANNER,
    POWER_CAMERA,
    POWER_WEARABLE,
    POWER_TOY,
    POWER_BLUETOOTH_GENREIC,
};

enum PowerSourceState {
    POWER_STATE_UNKNOWN,
    POWER_STATE_CHARGING,
    POWER_STATE_DISCHARGING,
    POWER_STATE_EMPTY,
    POWER_STATE_FULLY_CHARGED,
    POWER_STATE_PENDING_CHARGE,
    POWER_STATE_PENDING_DISCHARGE,
};

enum PowerSourceTechnology {
    POWER_TECH_UNKNOWN,
    POWER_TECH_LITHIUM_ION,
    POWER_TECH_LITHIUM_POLYMER,
    POWER_TECH_LITHIUM_IRON_PHOSPHATE,
    POWER_TECH_LEAD_ACID,
    POWER_TECH_NICKEL_CADMIUM,
    POWER_TECH_NICKEL_METAL_HYDRIDE,
};

typedef struct PowerSourceProperties {
    char *path;
    char *vendor;

    enum PowerSourceType type;
    enum PowerSourceState state;
    enum PowerSourceTechnology technology;

    float percentage;
    float capacity;
} PowerSourceProperties;

PowerSourceProperties get_power_source_properties(DBusConnection* connection, char *objpath) {
    DBusError error;
    dbus_error_init(&error);

    DBusMessage *message = dbus_message_new_method_call(
        "org.freedesktop.UPower",
        objpath,
        "org.freedesktop.DBus.Properties",
        "GetAll"
    );

    const char *ifc = "org.freedesktop.UPower.Device";
    dbus_message_append_args(message, DBUS_TYPE_STRING, &ifc, DBUS_TYPE_INVALID);

    DBusMessage *reply = dbus_connection_send_with_reply_and_block(connection, message, -1, &error);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Failed to send message %s", error.message);
        exit(EXIT_FAILURE);
    }

    dbus_message_unref(message);

    printf("REPLY TYPE: %s", dbus_message_type_to_string(dbus_message_get_type(reply)));

    PowerSourceProperties properties = {
        0
    };

    return properties;
}

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



    // dbus_message_iter_recurse();
    // DBusMessageIter sub;

    /*
    while (dbus_message_iter_has_next(&iter)) {
        char *message;
        ;
        dbus_message_iter_next(&iter);
        printf("%s\n", message);
    }
    */

    char *path = get_display_device_path(connection);
    get_power_source_properties(connection, path);
 

    dbus_connection_unref(connection);

    return 0;
}
