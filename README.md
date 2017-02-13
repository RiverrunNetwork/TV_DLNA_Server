# TV_DLNA_Server
公司 DLNA Server 的 Platium 源码

# 自定义字段

showVR  : 是否支持 VR
version : DLNA Server 版本，用于区分支持的服务。最新版本为 3.0。 


``` c++
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
```

# 版本描述

| Version | 支持服务 | 
| --- | --- |
| 3.0 | 遥控器， 风迷频道， 普通视频投屏 |
