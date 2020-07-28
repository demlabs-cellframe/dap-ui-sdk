#include "UiScaling.h"

#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <qDebug>

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
#ifdef Q_OS_WIN
    HDC screen = GetDC(NULL);
    int hSize = GetDeviceCaps(screen, HORZSIZE);
    int hRes = GetDeviceCaps(screen, HORZRES);

    float PixelsPerMM = (float)(hRes / hSize); // pixels per millimeter
    float dpi = PixelsPerMM * 25.4f;
#elif defined(Q_OS_LINUX)
#ifndef Q_OS_ANDROID
    Display *dpy;
    XRRScreenResources *screen;
    XRROutputInfo * outInfo;
    XRRCrtcInfo *crtc_info;

    dpy = XOpenDisplay(":0");
    screen = XRRGetScreenResources(dpy, DefaultRootWindow(dpy));
    crtc_info = XRRGetCrtcInfo(dpy, screen, screen->crtcs[0]); //0 to get the first monitor
    outInfo = XRRGetOutputInfo(dpy, screen, *crtc_info->outputs);

    float PixelsPerMM = (float)crtc_info->height / outInfo->mm_height;
    float dpi = PixelsPerMM * 25.4f;
#endif
#elif defined(Q_OS_MACOS)
    auto mainDisplayId = CGMainDisplayID();
    CGSize screenSize = CGDisplayScreenSize(mainDisplayId);
    float dpi = 25.4f * (CGDisplayPixelsWide(mainDisplayId) / screenSize.width);
#else
    static qreal dpi(QGuiApplication::primaryScreen()->physicalDotsPerInch());
#endif
#ifndef Q_OS_ANDROID
    //if dpi is less than 50, it may have been calculated incorrectly
    qInfo() << "Pixels per mm: " << PixelsPerMM << " Resolution: " << hRes << " Screen height: " << hSize << " UiScaling - dpi: " << dpi;
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
