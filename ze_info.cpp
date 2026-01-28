#include <level_zero/ze_api.h>
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    zeInit(0);

    // ---- Enumerate drivers ----
    uint32_t driverCount = 0;
    zeDriverGet(&driverCount, nullptr);
    assert(driverCount > 0);

    std::vector<ze_driver_handle_t> drivers(driverCount);
    zeDriverGet(&driverCount, drivers.data());

    uint32_t gpuCount = 0;

    // ---- Walk drivers and devices ----
    for (auto driver : drivers) {
        uint32_t deviceCount = 0;
        zeDeviceGet(driver, &deviceCount, nullptr);
        if (deviceCount == 0)
            continue;

        std::vector<ze_device_handle_t> devices(deviceCount);
        zeDeviceGet(driver, &deviceCount, devices.data());

        for (auto device : devices) {
            ze_device_properties_t props{};
            props.stype = ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES;
            zeDeviceGetProperties(device, &props);

            if (props.type != ZE_DEVICE_TYPE_GPU)
                continue;

            std::cout << "==============================\n";
            std::cout << "GPU " << gpuCount++ << "\n";
            std::cout << "Name: " << props.name << "\n";

            // ---- Topology ----
            uint32_t slices    = props.numSlices;
            uint32_t subslices = props.numSubslicesPerSlice;
            uint32_t eus       = props.numEUsPerSubslice;
            uint32_t threadsEU = props.numThreadsPerEU;

            uint32_t totalEUs      = slices * subslices * eus;
            uint32_t totalThreads = totalEUs * threadsEU;

            std::cout << "Slices: " << slices << "\n";
            std::cout << "Subslices per slice: " << subslices << "\n";
            std::cout << "EUs per subslice: " << eus << "\n";
            std::cout << "Threads per EU: " << threadsEU << "\n";
            std::cout << "Total EUs: " << totalEUs << "\n";
            std::cout << "Total HW threads: " << totalThreads << "\n";

            // ---- Memory heaps (size only, portable) ----
            uint32_t memCount = 0;
            zeDeviceGetMemoryProperties(device, &memCount, nullptr);

            std::vector<ze_device_memory_properties_t> memProps(memCount);
            for (auto &m : memProps) {
                m.stype = ZE_STRUCTURE_TYPE_DEVICE_MEMORY_PROPERTIES;
            }

            zeDeviceGetMemoryProperties(device, &memCount, memProps.data());

            std::cout << "Memory heaps:\n";
            for (uint32_t i = 0; i < memCount; ++i) {
                const auto &m = memProps[i];
                std::cout << "  Heap " << i << ":\n";
                std::cout << "    Name: " << m.name << "\n";
                std::cout << "    Size (GB): "
                          << (double)m.totalSize / (1024.0 * 1024.0 * 1024.0)
                          << "\n";
            }
        }
    }

    std::cout << "==============================\n";
    std::cout << "Total GPUs detected: " << gpuCount << "\n";

    return 0;
}
