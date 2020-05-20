#include <json.h>

void cardIdToJSON(String &tag_uuid, String &lab_uuid, String &msg) {

    const int capacity = JSON_OBJECT_SIZE(2);
    StaticJsonDocument<capacity> doc;

    doc[ "tag_uuid" ]    = tag_uuid.c_str();
    doc[ "device_uuid" ] = lab_uuid.c_str();

    serializeJson(doc, msg);
}
