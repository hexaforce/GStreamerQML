import gi
import json

gi.require_version("Gst", "1.0")
from gi.repository import Gst


def caps_to_string(caps):
    caps_list = []
    for i in range(caps.get_size()):
        structure = caps.get_structure(i)
        caps_list.append(structure.to_string())
    return caps_list


def device_to_dict(device):
    device_info = {
        "Device": device.get_display_name(),
    }
    # Get properties
    properties = device.get_properties()
    if properties is not None:
        n_fields = properties.n_fields()
        for i in range(n_fields):
            field_name = properties.nth_field_name(i)
            value = properties.get_value(field_name)
            if value is not None:
                if isinstance(value, str):
                    device_info[field_name] = value
                elif isinstance(value, int):
                    device_info[field_name] = value
                elif isinstance(value, float):
                    device_info[field_name] = value
                elif isinstance(value, bool):
                    device_info[field_name] = value
                else:
                    device_info[field_name] = str(value)

    # Get caps information
    caps = device.get_caps()
    if caps is not None:
        device_info["caps"] = caps_to_string(caps)

    return device_info


def main():
    Gst.init(None)

    common_device_monitor = Gst.CommonDeviceMonitor.new()
    common_device_monitor.add_filter("Video/Source", None)
    # common_device_monitor.add_filter("Audio/Source", None)

    device_list = common_device_monitor.get_devices()

    devices = []
    for device in device_list:
        devices.append(device_to_dict(device))

    print(json.dumps(devices, indent=2, ensure_ascii=False))


if __name__ == "__main__":
    main()
