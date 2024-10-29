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
    QString showMessage;
    bool forceDownload = false;
    bool forceInstall = false;

    DataToUpdate &operator = (const DataToUpdate &a_data)
    {
        this->state = a_data.state;
        this->version = a_data.version;
        this->URLUpdate = a_data.URLUpdate;
        this->forceDownload = a_data.forceDownload;
        this->forceInstall = a_data.forceInstall;
        this->showMessage = a_data.showMessage;
        return *this;
    }

    void setState(DownloadingUpdateState a_state)
    {
        this->state = a_state;
    }
};
