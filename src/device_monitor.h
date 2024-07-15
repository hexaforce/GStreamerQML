#ifndef DEVICE_MONITOR_H
#define DEVICE_MONITOR_H

#include <string>
#include <json/json.h>
#include <gst/gst.h>

std::string listDevices(const std::string &deviceType);

#endif // DEVICE_MONITOR_H
