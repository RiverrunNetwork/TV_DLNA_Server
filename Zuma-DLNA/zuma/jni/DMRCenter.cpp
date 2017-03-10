#include "DMRCenter.h"

DMRCenter::DMRCenter(): mAdded(false) {
    LOGW("DMRCenter::DMRCenter!");
    
    mIMediaActionReceiver = new IMediaActionReceiver();
}

DMRCenter::~DMRCenter() {
    LOGW("DMRCenter::~DMRCenter!");
    
    delete mIMediaActionReceiver;
}

int DMRCenter::Start(const char *name, const char *showVR, const char *version, const char *uuid) {
    LOGW("DMRCenter::Start[%s][%s]", name, uuid);
    
    PLT_MediaRenderer *device = new PLT_MediaRenderer(name != NULL ? (const char *)name : "MatchStick Media Renderer",
                                                      showVR,
                                                      version,
                                                      false,
                                                      uuid != NULL ? (const char *)uuid :"e6572b54-f3c7-2d91-2fb5-b757f2537e22");
    device->SetDelegate(mIMediaActionReceiver);
    
    mDevice = device;
    
    mIMediaActionReceiver->setDevice(mDevice);
    
    mUPnP.AddDevice(mDevice);
    
    mAdded = true;
    
    mUPnP.Start();
}

int DMRCenter::Stop() {
    LOGW("DMRCenter::Stop!");
    
    if (mAdded) {
        mUPnP.RemoveDevice(mDevice);
    }
    
    mUPnP.Stop();
}

bool DMRCenter::responseGenaEvent(int cmd, const char *value, const char *data) {
    LOGW("DMRCenter::responseGenaEvent[%d][%s][%s]!", cmd, value, data);
    
    PLT_Service* serviceAVT = NULL;
    PLT_Service* serviceCMR;
    PLT_Service* serviceRC;
    NPT_String   type;
    
    if (!mDevice->GetType().StartsWith("urn:schemas-upnp-org:device:MediaRenderer")) {
        return false;
    }
    
    type = "urn:schemas-upnp-org:service:AVTransport:*";
    if (NPT_FAILED(mDevice->FindServiceByType(type, serviceAVT))) {
        LOGW("Service %s not found", (const char*)type);
        
        return false;
    }
    
    type = "urn:schemas-upnp-org:service:ConnectionManager:*";
    if (NPT_FAILED(mDevice->FindServiceByType(type, serviceCMR))) {
        LOGW("Service %s not found", (const char*)type);
        
        return false;
    }
    
    type = "urn:schemas-upnp-org:service:RenderingControl:*";
    if (NPT_FAILED(mDevice->FindServiceByType(type, serviceRC))) {
        LOGW("Service %s not found", (const char*)type);
        
        return false;
    }
    
    switch(cmd) {
        case MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_DURATION:
            serviceAVT->SetStateVariable("CurrentMediaDuration", value);
            serviceAVT->SetStateVariable("CurrentTrackDuration", value);
            break;
        case MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_POSITION:
            serviceAVT->SetStateVariable("RelativeTimePosition", value);
            break;
        case MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_PLAYINGSTATE:
            serviceAVT->SetStateVariable("TransportState", value);
            break;
    }
    
    return true;
}