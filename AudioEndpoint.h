#ifndef __AUDIO_ENDPOINT_H__
#define __AUDIO_ENDPOINT_H__

#include <iostream>
#include <string>
#include <vector>

#include <Mmdeviceapi.h>
#include <endpointvolume.h>
#include <atlstr.h>

#define EXIT_ON_ERROR(hres) if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk) if ((punk) != NULL) { (punk)->Release(); (punk) = NULL; }


namespace std {

class AudioEndpoint {
    private:
        IMMDevice* handle;
    public:
        AudioEndpoint(IMMDevice* device) {
            this->handle = device;
        }

        ~AudioEndpoint() {
            if (handle != NULL) {
                handle->Release();
                handle = NULL;
            }
        }

        string getId() const;
        string getFriendlyName() const;
        float getVolume() const;
        void setVolume(float volume) const;

};

}



#endif // __AUDIO_ENDPOINT_H__
