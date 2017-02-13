#include "IMediaActionReceiver.h"

extern void ActionInflect(int cmd, const char* value, const char* data);

IMediaActionReceiver::IMediaActionReceiver() {
    LOGW("IMediaActionReceiver::IMediaActionReceiver!");
}

IMediaActionReceiver::~IMediaActionReceiver() {
    LOGW("IMediaActionReceiver::~IMediaActionReceiver!");
}

// ConnectionManager
NPT_Result IMediaActionReceiver::OnGetCurrentConnectionInfo(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnGetCurrentConnectionInfo!");

    if (NPT_FAILED(action->SetArgumentValue("RcsID", "0"))){
        return NPT_FAILURE;
    }
    
    if (NPT_FAILED(action->SetArgumentValue("AVTransportID", "0"))) {
        return NPT_FAILURE;
    }
    
    if (NPT_FAILED(action->SetArgumentOutFromStateVariable("ProtocolInfo"))) {
        return NPT_FAILURE;
    }
    
    if (NPT_FAILED(action->SetArgumentValue("PeerConnectionManager", "/"))) {
        return NPT_FAILURE;
    }
    
    if (NPT_FAILED(action->SetArgumentValue("PeerConnectionID", "-1"))) {
        return NPT_FAILURE;
    }
    
    if (NPT_FAILED(action->SetArgumentValue("Direction", "Input"))) {
        return NPT_FAILURE;
    }
    
    if (NPT_FAILED(action->SetArgumentValue("Status", "Unknown"))) {
        return NPT_FAILURE;
    }
    
    return NPT_SUCCESS;
}

// AVTransport
NPT_Result IMediaActionReceiver::OnNext(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnNext!");

    NPT_String NextURI;
    action->GetArgumentValue("NextURI", NextURI);
    
    NPT_String NextURIMetaData;
    action->GetArgumentValue("NextURIMetaData", NextURIMetaData);

    ActionInflect(MEDIA_RENDER_CTL_MSG_NEXT, NextURI.GetChars(), NextURIMetaData.GetChars());

    return NPT_SUCCESS;
}

NPT_Result IMediaActionReceiver::OnPause(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnPause!");

    ActionInflect(MEDIA_RENDER_CTL_MSG_PAUSE, "", "");

    return NPT_SUCCESS;
}
    
NPT_Result IMediaActionReceiver::OnPlay(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnPlay!");

    NPT_String Speed;
    action->GetArgumentValue("Speed", Speed);

    ActionInflect(MEDIA_RENDER_CTL_MSG_PLAY, Speed.GetChars(), "");

    return NPT_SUCCESS;
}
    
NPT_Result IMediaActionReceiver::OnPrevious(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnPrevious!");

    ActionInflect(MEDIA_RENDER_CTL_MSG_PRE, "", "");

    return NPT_SUCCESS;
}
    
NPT_Result IMediaActionReceiver::OnSeek(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnSeek!");

    NPT_String Unit;
    action->GetArgumentValue("Unit", Unit);

    NPT_String Target;
    action->GetArgumentValue("Target", Target);

    NPT_String Total = Unit + "=" + Target;
    ActionInflect(MEDIA_RENDER_CTL_MSG_SEEK, Total.GetChars(), "");

    return NPT_SUCCESS;
}
    
NPT_Result IMediaActionReceiver::OnStop(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnStop!");

    ActionInflect(MEDIA_RENDER_CTL_MSG_STOP, "", "");

    return NPT_SUCCESS;
}
    
NPT_Result IMediaActionReceiver::OnSetAVTransportURI(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnSetAVTransportURI!");

    NPT_String curURI;
    action->GetArgumentValue("CurrentURI", curURI);
    
    NPT_String metaData;
    action->GetArgumentValue("CurrentURIMetaData", metaData);

    // update service state variables
    PLT_Service* serviceAVT;
    mDevice->FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", serviceAVT);
    serviceAVT->SetStateVariable("AVTransportURI", curURI);
    serviceAVT->SetStateVariable("AVTransportURIMetaData", metaData);

    serviceAVT->SetStateVariable("CurrentTrackURI", curURI);
    serviceAVT->SetStateVariable("CurrentTrackMetadata", metaData);

    serviceAVT->SetStateVariable("TransportState", "TRANSITIONING");

    ActionInflect(MEDIA_RENDER_CTL_MSG_SET_AV_URL, curURI.GetChars(), metaData.GetChars());

    return NPT_SUCCESS;
}
    
NPT_Result IMediaActionReceiver::OnSetPlayMode(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnSetPlayMode!");

    NPT_String NewPlayMode;
    action->GetArgumentValue("NewPlayMode", NewPlayMode);

    ActionInflect(MEDIA_RENDER_CTL_MSG_SETPLAYMODE, NewPlayMode.GetChars(), "");

    return NPT_SUCCESS;
}

// RenderingControl
NPT_Result IMediaActionReceiver::OnSetVolume(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnSetVolume!");

    NPT_String Channel;
    action->GetArgumentValue("Channel", Channel);

    NPT_String DesiredVolume;
    action->GetArgumentValue("DesiredVolume", DesiredVolume);

    // update service state variables
    PLT_Service* serviceRC;
    mDevice->FindServiceByType("urn:schemas-upnp-org:service:RenderingControl:1", serviceRC);
    serviceRC->SetStateVariable("Volume", DesiredVolume);
    if (DesiredVolume == "0") {
        LOGW("MUTED!");
        serviceRC->SetStateVariable("Mute", "1");
    } else {
        LOGW("NOT MUTED!");
        serviceRC->SetStateVariable("Mute", "0");
    }

    //ActionInflect(MEDIA_RENDER_CTL_MSG_SETVOLUME, Channel.GetChars(), DesiredVolume.GetChars());
    ActionInflect(MEDIA_RENDER_CTL_MSG_SETVOLUME, DesiredVolume.GetChars(), "");

    return NPT_SUCCESS;
}
    
NPT_Result IMediaActionReceiver::OnSetVolumeDB(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnSetVolumeDB!");

    return NPT_SUCCESS;
}
    
NPT_Result IMediaActionReceiver::OnGetVolumeDBRange(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnGetVolumeDBRange!");

    return NPT_SUCCESS;
}
    
NPT_Result IMediaActionReceiver::OnSetMute(PLT_ActionReference& action) {
    LOGW("IMediaActionReceiver::OnSetMute!");

    NPT_String Channel;
    action->GetArgumentValue("Channel", Channel);

    NPT_String DesiredMute;
    action->GetArgumentValue("DesiredMute", DesiredMute);

    // update service state variables
    PLT_Service* serviceRC;
    mDevice->FindServiceByType("urn:schemas-upnp-org:service:RenderingControl:1", serviceRC);
    if (DesiredMute == "0") {
        LOGW("NOT MUTED!");
        serviceRC->SetStateVariable("Mute", "0");
    } else {
        LOGW("MUTED!");
        serviceRC->SetStateVariable("Mute", "1");
    }

    //ActionInflect(MEDIA_RENDER_CTL_MSG_SETMUTE, Channel.GetChars(), DesiredMute.GetChars());
    ActionInflect(MEDIA_RENDER_CTL_MSG_SETMUTE, DesiredMute.GetChars(), "");

    return NPT_SUCCESS;
}

void IMediaActionReceiver::setDevice(PLT_DeviceHostReference device) {
    mDevice = device;
}
