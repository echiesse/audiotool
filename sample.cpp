#include <iostream>
#include <string>
#include <vector>

//#include<Windows.h>
#include <Mmdeviceapi.h>
#include <combaseapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <atlstr.h>

using namespace std;

class AudioEndpoint {
    public:
        string id;
        string name;

    public:
        AudioEndpoint(LPWSTR id, LPWSTR name) {
            this->id = CW2A(id);
            this->name = CW2A(name);
        }
        static vector<AudioEndpoint> loadFromSystem();
};

//-----------------------------------------------------------
// This function enumerates all active (plugged in) audio
// rendering endpoint devices. It prints the friendly name
// and endpoint ID string of each endpoint device.
//-----------------------------------------------------------
#define EXIT_ON_ERROR(hres) if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk) if ((punk) != NULL) { (punk)->Release(); (punk) = NULL; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

void PrintEndpointNames(vector<AudioEndpoint>& endpoints)
{
    HRESULT hr = S_OK;
    IMMDeviceEnumerator *pEnumerator = NULL;
    IMMDeviceCollection *pCollection = NULL;

    hr = CoCreateInstance(
        CLSID_MMDeviceEnumerator,
        NULL,
        CLSCTX_ALL,
        IID_IMMDeviceEnumerator,
        (void**)&pEnumerator
    );
    EXIT_ON_ERROR(hr)

    hr = pEnumerator->EnumAudioEndpoints(
        eCapture, DEVICE_STATE_ACTIVE,
        &pCollection
    );
    EXIT_ON_ERROR(hr)

    UINT  count;
    hr = pCollection->GetCount(&count);
    printf("Count %i\n", count);
    EXIT_ON_ERROR(hr)

    if (count == 0)
    {
        printf("No endpoints found.\n");
    }

    // Each loop prints the name of an endpoint device.
    for (ULONG i = 0; i < count; i++)
    {
        // Get pointer to endpoint number i.
        IMMDevice *pEndpoint = NULL;
        hr = pCollection->Item(i, &pEndpoint);
        EXIT_ON_ERROR(hr)

        // Get the endpoint ID string.
        LPWSTR pwszID = NULL;
        hr = pEndpoint->GetId(&pwszID);
        EXIT_ON_ERROR(hr)

        IPropertyStore *pProps = NULL;
        hr = pEndpoint->OpenPropertyStore(STGM_READ, &pProps);
        EXIT_ON_ERROR(hr)

        PROPVARIANT varName;
        // Initialize container for property value.
        PropVariantInit(&varName);

        // Get the endpoint's friendly-name property.
        hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
        EXIT_ON_ERROR(hr)

        // GetValue succeeds and returns S_OK if PKEY_Device_FriendlyName is not found.
        // In this case vartName.vt is set to VT_EMPTY.
        if (varName.vt != VT_EMPTY)
        {
            // Print endpoint friendly name and endpoint ID.
            //printf("Endpoint %d: \"%S\" (%S)\n", i, varName.pwszVal, pwszID);
            endpoints.push_back(AudioEndpoint(pwszID, varName.pwszVal));
        }

        CoTaskMemFree(pwszID);
        pwszID = NULL;
        PropVariantClear(&varName);
        SAFE_RELEASE(pProps)
        SAFE_RELEASE(pEndpoint)
    }
    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pCollection)
    return;

Exit:
    printf("Error!\n");
    //CoTaskMemFree(pwszID);
    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pCollection)
    //SAFE_RELEASE(pEndpoint)
    //SAFE_RELEASE(pProps)
}


vector<AudioEndpoint> AudioEndpoint::loadFromSystem() {
    auto devices = vector<AudioEndpoint>();
    PrintEndpointNames(devices);
    return devices;
}

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

int main() {
    initializeCom();
    auto devices = vector<AudioEndpoint>();
    PrintEndpointNames(devices);
    for (auto device: devices) {
        cout << device.name << endl;
        cout << device.id << endl;
        cout << endl;
    }
}
