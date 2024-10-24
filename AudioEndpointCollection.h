#ifndef __AUDIO_ENDPOINT_COLLECTION_H__
#define __AUDIO_ENDPOINT_COLLECTION_H__

#include <vector>
#include "AudioEndpoint.h"

namespace std {


class AudioEndpointCollection {
    private:
        vector<AudioEndpoint*> endpoints;

    public:
        AudioEndpointCollection();
        AudioEndpointCollection(vector<AudioEndpoint*> endpoints);
        ~AudioEndpointCollection();

        void loadFromSystem(EDataFlow flowDirection);
        void push_back(AudioEndpoint* endpoint);
        vector<AudioEndpoint*>::iterator begin();
        vector<AudioEndpoint*>::iterator end();

        AudioEndpoint* findById(string id);
        AudioEndpoint* firstByPartialFriendlyName(string nameFragment);
        vector<AudioEndpoint*> allByPartialFriendlyName(string nameFragment);
};

}
#endif // __AUDIO_ENDPOINT_COLLECTION_H__
