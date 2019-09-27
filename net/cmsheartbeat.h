#ifndef CMSHEARTBEAT_H
#define CMSHEARTBEAT_H

#include "icmsdata.h"

struct HeartBeatField
{
    SenderRunninigStatus        status = SENDER_Running;
    QList<CmsLabelID>           onlineLabels;
};

class CmsHeartBeat : public ICmsData
{
public:
    CmsHeartBeat();

    virtual ~CmsHeartBeat();

    virtual bool fromByteArray(const QByteArray &arr);

    virtual QByteArray toByteArray() const;

    void setData(HeartBeatField data);

    HeartBeatField data() const;

protected:
    HeartBeatField dataField;
};

#endif // CMSHEARTBEAT_H
