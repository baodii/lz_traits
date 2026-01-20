#include <level_zero/ze_api.h>
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    zeInit(0);

    uint32_t driverCount = 0;
    zeDriverGet(&driverCount, nullptr);
    assert(driverCount > 0);

    std::vector<ze_driver_handle_t> drivers(driverCount);
    zeDriverGet(&driverCount, drivers.data());

    ze_driver_handle_t driver = drivers[0];

    uint32_t deviceCount = 0;
    zeDeviceGet(driver, &deviceCount, nullptr);
    assert(deviceCount > 0);

    std::vector<ze_device_handle_t> devices(deviceCount);
    zeDeviceGet(driver, &deviceCount, devices.data());

    ze_device_handle_t device = devices[0];

    ze_device_properties_t props{};
    props.stype = ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES;
    zeDeviceGetProperties(device, &props);

    uint32_t slices    = props.numSlices;
    uint32_t subslices = props.numSubslicesPerSlice;
    uint32_t eus       = props.numEUsPerSubslice;
    uint32_t threadsEU = props.numThreadsPerEU;

    uint32_t totalEUs      = slices * subslices * eus;
    uint32_t totalThreads = totalEUs * threadsEU;

    std::cout << "Device: " << props.name << "\n";
    std::cout << "Slices: " << slices << "\n";
    std::cout << "Subslices per slice: " << subslices << "\n";
    std::cout << "EUs per subslice: " << eus << "\n";
    std::cout << "Threads per EU: " << threadsEU << "\n";
    std::cout << "Total EUs: " << totalEUs << "\n";
    std::cout << "Total HW threads: " << totalThreads << "\n";

    return 0;
}

