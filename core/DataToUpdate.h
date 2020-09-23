#pragma once

#include <QString>

enum class DownloadingUpdateState{
    Default, Download
};

struct DataToUpdate
{
    DownloadingUpdateState state;
    QString version;
    QString URLUpdate;

    DataToUpdate &operator = (const DataToUpdate &a_data)
    {
        this->state = a_data.state;
        this->version = a_data.version;
        this->URLUpdate = a_data.URLUpdate;
        return *this;
    }

    void setState(DownloadingUpdateState a_state)
    {
        this->state = a_state;
    }
};
