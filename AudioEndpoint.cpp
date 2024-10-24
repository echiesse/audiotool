#include "AudioEndpoint.h"

#include <combaseapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <atlstr.h>

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);


namespace std {

string AudioEndpoint::getId() const {
    LPWSTR pwszID = NULL;
    HRESULT hr = handle->GetId(&pwszID);
    if (!FAILED(hr)) {
        return (string)CW2A(pwszID);
    }
    CoTaskMemFree(pwszID);
    pwszID = NULL;
    return "";
}

string AudioEndpoint::getFriendlyName() const {
    IPropertyStore *pProps = NULL;
    HRESULT hr = handle->OpenPropertyStore(STGM_READ, &pProps);
    if (FAILED(hr)) {
        // TODO: Throw exception
        return "";
    }

    PROPVARIANT varName;
    // Initialize container for property value.
    PropVariantInit(&varName);

    // Get the endpoint's friendly-name property.
    hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
    if (FAILED(hr)) {
        // TODO: Throw exception
        return "";
    }

    bool isEmpty = varName.vt != VT_EMPTY;
    string res = CW2A(varName.pwszVal);

    PropVariantClear(&varName);
    SAFE_RELEASE(pProps)

    return isEmpty ? res : "";
}

float AudioEndpoint::getVolume() const {
    IAudioEndpointVolume *g_pEndptVol = NULL;
    HRESULT hr = handle->Activate(
        __uuidof(IAudioEndpointVolume),
        CLSCTX_ALL,
        NULL,
        (void**)&g_pEndptVol
    );

    float level = 0.0;
    g_pEndptVol->GetMasterVolumeLevelScalar(&level);
    g_pEndptVol->Release();
    return level;
}

void AudioEndpoint::setVolume(float volume) const {
    IAudioEndpointVolume *g_pEndptVol = NULL;
    HRESULT hr = handle->Activate(
        __uuidof(IAudioEndpointVolume),
        CLSCTX_ALL,
        NULL,
        (void**)&g_pEndptVol
    );

    g_pEndptVol->SetMasterVolumeLevelScalar(volume, NULL);
    g_pEndptVol->Release();
}


}
