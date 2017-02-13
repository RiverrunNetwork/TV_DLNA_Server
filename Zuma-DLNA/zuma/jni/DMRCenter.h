#ifndef DMRCENTER_H_
#define DMRCENTER_H_

#include "PltUPnP.h"
#include "PltMediaRenderer.h"

#include "Logs.h"
#include "IMediaActionReceiver.h"

class DMRCenter{
public:
    DMRCenter();
    virtual ~DMRCenter();

    int Start(const char *name,const char *showVR, const char  *version, const char* uuid);
    int Stop();

    bool responseGenaEvent(int cmd, const char *value, const char *data);
private:
    PLT_UPnP mUPnP;
    PLT_DeviceHostReference mDevice;

    IMediaActionReceiver *mIMediaActionReceiver;

    bool mAdded;
};

#endif // DMRCENTER_H_
