#include <vector>
#include "AudioEndpoint.h"
#include "AudioEndpointCollection.h"

namespace std {

static vector<AudioEndpoint*> retrieveEndpoints(EDataFlow dataFlow)
{
    HRESULT hr = S_OK;
    IMMDeviceEnumerator *pEnumerator = NULL;
    IMMDeviceCollection *pCollection = NULL;
    vector<AudioEndpoint*> endpoints;

    hr = CoCreateInstance(
        CLSID_MMDeviceEnumerator,
        NULL,
        CLSCTX_ALL,
        IID_IMMDeviceEnumerator,
        (void**)&pEnumerator
    );
    EXIT_ON_ERROR(hr)

    hr = pEnumerator->EnumAudioEndpoints(
        dataFlow,
        DEVICE_STATE_ACTIVE,
        &pCollection
    );
    EXIT_ON_ERROR(hr)

    UINT count;
    hr = pCollection->GetCount(&count);
    EXIT_ON_ERROR(hr)

    for (ULONG i = 0; i < count; i++)
    {
        IMMDevice *pEndpoint = NULL;
        hr = pCollection->Item(i, &pEndpoint);
        EXIT_ON_ERROR(hr)

        endpoints.push_back(new AudioEndpoint(pEndpoint));
    }
    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pCollection)
    return endpoints;

Exit:
    printf("Error!\n");
    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pCollection)
}


AudioEndpointCollection::AudioEndpointCollection(){}

AudioEndpointCollection::AudioEndpointCollection(vector<AudioEndpoint*> endpoints){
    this->endpoints = endpoints;
}

AudioEndpointCollection::~AudioEndpointCollection(){
    for (auto endpoint: endpoints) {
        delete endpoint;
    }
}

void AudioEndpointCollection::push_back(AudioEndpoint* endpoint) {
    endpoints.push_back(endpoint);
}

vector<AudioEndpoint*>::iterator AudioEndpointCollection::begin() {
    return endpoints.begin();
}

vector<AudioEndpoint*>::iterator AudioEndpointCollection::end() {
    return endpoints.end();
}

void AudioEndpointCollection::loadFromSystem(EDataFlow flowDirection) {
    this->endpoints = retrieveEndpoints(flowDirection);
}

AudioEndpoint* AudioEndpointCollection::findById(string id) {
    for (auto device: endpoints) {
        if (device->getId() == id){
            return device;
        }
    }
    return NULL;
}

AudioEndpoint* AudioEndpointCollection::firstByPartialFriendlyName(string nameFragment) {
    for (auto device: endpoints) {
        if (device->getFriendlyName().find(nameFragment) != string::npos){
            return device;
        }
    }
    return NULL;
}

vector<AudioEndpoint*> AudioEndpointCollection::allByPartialFriendlyName(string nameFragment) {
    auto devices = vector<AudioEndpoint*>();
    for (auto device: endpoints) {
        if (device->getFriendlyName().find(nameFragment) != string::npos){
            devices.push_back(device);
        }
    }
    return devices;
}


}
