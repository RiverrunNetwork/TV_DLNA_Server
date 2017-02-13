#ifndef IMEDIAACTIONRECEIVER_H_
#define IMEDIAACTIONRECEIVER_H_

#include "PltService.h"
#include "PltMediaRenderer.h"

#include "Logs.h"

#define MEDIA_RENDER_CTL_MSG_BASE 			            0x100

#define MEDIA_RENDER_CTL_MSG_SET_AV_URL 		        (MEDIA_RENDER_CTL_MSG_BASE+0)
#define MEDIA_RENDER_CTL_MSG_STOP 			            (MEDIA_RENDER_CTL_MSG_BASE+1)
#define MEDIA_RENDER_CTL_MSG_PLAY 			            (MEDIA_RENDER_CTL_MSG_BASE+2)
#define MEDIA_RENDER_CTL_MSG_PAUSE 			            (MEDIA_RENDER_CTL_MSG_BASE+3)
#define MEDIA_RENDER_CTL_MSG_SEEK 			            (MEDIA_RENDER_CTL_MSG_BASE+4)
#define MEDIA_RENDER_CTL_MSG_SETVOLUME 			        (MEDIA_RENDER_CTL_MSG_BASE+5)
#define MEDIA_RENDER_CTL_MSG_SETMUTE 			        (MEDIA_RENDER_CTL_MSG_BASE+6)
#define MEDIA_RENDER_CTL_MSG_SETPLAYMODE 		        (MEDIA_RENDER_CTL_MSG_BASE+7)
#define MEDIA_RENDER_CTL_MSG_PRE 			            (MEDIA_RENDER_CTL_MSG_BASE+8)
#define MEDIA_RENDER_CTL_MSG_NEXT 			            (MEDIA_RENDER_CTL_MSG_BASE+9)

#define MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_DURATION 	(MEDIA_RENDER_CTL_MSG_BASE+0)
#define MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_POSITION 	(MEDIA_RENDER_CTL_MSG_BASE+1)
#define MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_PLAYINGSTATE (MEDIA_RENDER_CTL_MSG_BASE+2)

#define RENDERER_TOCONTRPOINT_CMD_INTENT_NAME		    "com.geniusgithub.platinum.tocontrolpointer.cmd.intent"
#define GET_RENDERER_TOCONTRPOINT_CMD			        "get_dlna_renderer_tocontrolpointer.cmd"
#define GET_PARAM_MEDIA_DURATION			            "get_param_media_duration"
#define GET_PARAM_MEDIA_POSITION			            "get_param_media_position"
#define GET_PARAM_MEDIA_PLAYINGSTATE			        "get_param_media_playingstate"

#define MEDIA_PLAYINGSTATE_STOP				            "STOPPED"
#define MEDIA_PLAYINGSTATE_PAUSE			            "PAUSED_PLAYBACK"
#define MEDIA_PLAYINGSTATE_PLAYING			            "PLAYING"
#define MEDIA_PLAYINGSTATE_TRANSTION			        "TRANSITIONING"
#define MEDIA_PLAYINGSTATE_NOMEDIA			            "NO_MEDIA_PRESENT"
    
#define MEDIA_SEEK_TIME_TYPE_REL_TIME			        "REL_TIME"
#define MEDIA_SEEK_TIME_TYPE_TRACK_NR			        "TRACK_NR"

class IMediaActionReceiver : public PLT_MediaRendererDelegate {
public:
    IMediaActionReceiver();
    virtual ~IMediaActionReceiver();

    // ConnectionManager
    NPT_Result OnGetCurrentConnectionInfo(PLT_ActionReference& action);

    // AVTransport
    NPT_Result OnNext(PLT_ActionReference& action);
    NPT_Result OnPause(PLT_ActionReference& action);
    NPT_Result OnPlay(PLT_ActionReference& action);
    NPT_Result OnPrevious(PLT_ActionReference& action);
    NPT_Result OnSeek(PLT_ActionReference& action);
    NPT_Result OnStop(PLT_ActionReference& action);
    NPT_Result OnSetAVTransportURI(PLT_ActionReference& action);
    NPT_Result OnSetPlayMode(PLT_ActionReference& action);

    // RenderingControl
    NPT_Result OnSetVolume(PLT_ActionReference& action);
    NPT_Result OnSetVolumeDB(PLT_ActionReference& action);
    NPT_Result OnGetVolumeDBRange(PLT_ActionReference& action);
    NPT_Result OnSetMute(PLT_ActionReference& action);

    void setDevice(PLT_DeviceHostReference device);
private:
    PLT_DeviceHostReference mDevice;
};

#endif // IMEDIAACTIONRECEIVER_H_
