#include "UiScaling.h"

#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <QDebug>
#include <QSettings>

#ifdef Q_OS_WIN
#include "windows.h"
const GUID GUID_CLASS_MONITOR = { 0x4d36e96e, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 };
#elif defined(Q_OS_LINUX)
#ifndef Q_OS_ANDROID
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#endif
#elif defined(Q_OS_MACOS)
#include <CoreGraphics/CoreGraphics.h>
#endif

float UiScaling::pointsToPixels(float a_pointsValue, float dpi /*default = 0*/)
{
#ifndef Q_OS_ANDROID
    if (!dpi)
        dpi = getNativDPI();
    float valueInPixels = dpi * pointsToInches(aptToPt(a_pointsValue));
#else
    Q_UNUSED(dpi)
    static qreal dpi_Android(QGuiApplication::primaryScreen()->geometry().width());

    float valueInPixels = dpi_Android * a_pointsValue / 360.f;

#endif
    return valueInPixels;
}

QSize UiScaling::pointsToPixels(const QSize &a_pointsSize)
{
    return QSize(UiScaling::pointsToPixels(a_pointsSize.width()),
                 UiScaling::pointsToPixels(a_pointsSize.height()));
}

inline double UiScaling::pointsToInches(float a_pointsValue)
{
    return a_pointsValue / 160.f;
}

float UiScaling::getNativDPI(){

    int hSize = 0, wSize = 0, hResolution = 0, wResolution = 0;

    if (QFile().exists(DapSystem::dirAppFiles(DAP_BRAND) + "/ScreenSettings.ini")){
        qInfo() << "UiScaling - Manual setting of screen characteristics selected";
        QSettings settings(DapSystem::dirAppFiles(DAP_BRAND) + "/ScreenSettings.ini", QSettings::IniFormat);
        hSize       = settings.value("hSize", 0).toInt();
        wSize       = settings.value("wSize", 0).toInt();
        hResolution = settings.value("hResolution", 0).toInt();
        wResolution = settings.value("wResolution", 0).toInt();
    } else {
#ifdef Q_OS_WIN
        HDC screen = GetDC(NULL);
        hSize       = GetDeviceCaps(screen, HORZSIZE);
        wSize       = GetDeviceCaps(screen, VERTSIZE);
        hResolution = GetDeviceCaps(screen, HORZRES);
        wResolution = GetDeviceCaps(screen, VERTRES);
#elif defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
        Display *dpy;
        XRRScreenResources *screen;
        XRROutputInfo * outInfo;
        XRRCrtcInfo *crtc_info;

        dpy = XOpenDisplay(":0");
        screen = XRRGetScreenResources(dpy, DefaultRootWindow(dpy));
        crtc_info = XRRGetCrtcInfo(dpy, screen, screen->crtcs[0]); //0 to get the first monitor
        outInfo = XRRGetOutputInfo(dpy, screen, *crtc_info->outputs);

        hSize       = crtc_info->height;
        wSize       = crtc_info->width;
        hResolution = outInfo->mm_height;
        wResolution = outInfo->mm_width;
#elif defined(Q_OS_MACOS)
        auto mainDisplayId = CGMainDisplayID();
        CGSize screenSize = CGDisplayScreenSize(mainDisplayId);

        hSize       = screenSize.height;
        wSize       = screenSize.width;
        hResolution = CGDisplayPixelsHigh(mainDisplayId);
        wResolution = CGDisplayPixelsWide(mainDisplayId);
#else
        static qreal dpi(QGuiApplication::primaryScreen()->physicalDotsPerInch());
#endif
    }
#ifndef Q_OS_ANDROID

    float pixelsPerMM = ((float)hResolution / hSize + (float)wResolution / wSize)/2;
    float dpi = pixelsPerMM * 25.4f;
    qInfo() << QString("UiScaling - Pixels pre mm: %1 Resolution: %2x%3 Screen size: %4x%5 dpi: %6 According to qt: %7")
               .arg(pixelsPerMM).arg(hResolution).arg(wResolution).arg(hSize).arg(wSize).arg(dpi)
               .arg(QGuiApplication::primaryScreen()->physicalDotsPerInch()); //for statistic, delete later
    return ((dpi < 50 || dpi > 350) ? QGuiApplication::primaryScreen()->physicalDotsPerInch() : dpi);
#else
    return 1;
#endif
}

float UiScaling::aptToPt(float apt){
    /*CSS has its own pt unit, which is 1.333 (96/72).
     times bigger than px however it is different from iOS p.*/
    //1.270f - relatively window Mira
    return apt * 1.270f;
}
