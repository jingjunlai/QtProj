#include "cmsheartbeat.h"

CmsHeartBeat::CmsHeartBeat()
    : ICmsData()
{
}

CmsHeartBeat::~CmsHeartBeat()
{
}

bool CmsHeartBeat::fromByteArray(const QByteArray &arr)
{
    if (arr.isEmpty())
        return false;

    dataField.status = (SenderRunninigStatus)(arr.at(0) & 0x03);

    dataField.onlineLabels.clear();
    for (int i = 1; i < arr.size(); i++)
    {
        auto label = (CmsLabelID)arr.at(i);
        dataField.onlineLabels.append(label);
    }

    return true;
}

QByteArray CmsHeartBeat::toByteArray() const
{
    char buf[dataField.onlineLabels.size() + 1];
    buf[0] = dataField.status;
    for (int i = 0; i < dataField.onlineLabels.size(); i++)
    {
        buf[i + 1] = dataField.onlineLabels.at(i);
    }

    QByteArray content;
    content = QByteArray(buf, sizeof(buf));
    return content;
}

void CmsHeartBeat::setData(HeartBeatField data)
{
    this->dataField = data;
}

HeartBeatField CmsHeartBeat::data() const
{
    return dataField;
}

