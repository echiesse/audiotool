#include <iostream>
#include <string>
#include <vector>

//#include<Windows.h>
#include <Mmdeviceapi.h>
#include <combaseapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <atlstr.h>

#include "AudioEndpoint.h"
#include "AudioEndpointCollection.h"

using namespace std;


int initializeCom() {
    HRESULT res = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED); // Importante e não está no exemplo original !!!
    switch (res)
    {
    case S_OK:
    case S_FALSE:
        return 0;

    default:
        return res;
    }
}

void showDeviceVolumes() {
    initializeCom();
    AudioEndpointCollection devices;
    devices.loadFromSystem(eAll);

    cout << "Device Volumes: " << endl;
    for (auto device: devices) {
        cout << device->getFriendlyName() << ": " << device->getVolume() << endl;
    }

    CoUninitialize();
}


void setDeviceVolume(string endpointPartialFriendlyName, float level) {
    initializeCom();
    AudioEndpointCollection devices;
    devices.loadFromSystem(eAll);

    AudioEndpoint* device = devices.firstByPartialFriendlyName(endpointPartialFriendlyName);
    if (device == NULL) {
        cout << "Device with friendly name containing \""<< endpointPartialFriendlyName << "\" not found" << endl;
        return;
    }

    device->setVolume(level);

    cout << "Volume set to "<< level << " for device \"" << device->getFriendlyName() << "\"" <<endl;

    CoUninitialize();
}

int main(int argc, char* argv[]) {

    if (argc > 1) {
        auto command = string(argv[1]);
        if (command == "show-volumes") {
            showDeviceVolumes();
        }
        else if (command == "set-volume") {
            if (argc < 3 ) {
                cerr << "Usage: audiotool set-volume <device-name-fragment> <level>" << endl;
            }
            auto nameFragment = string(argv[2]);
            float level = strtof(argv[3], NULL);
            setDeviceVolume(nameFragment, level);
        }
        else {
            cout << "Unknown command: " << command << endl;
        }
    }
}
