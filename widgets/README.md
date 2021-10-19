@mainpage
@tableofcontents

# Description

**DapChainVpgGuiWidgets** - Module represents custom widgets for GUI.<br>
The purpose of this module is to make life easier with stylized gui.<br>
This module made to be used inside GUI and can be a plugin for QtDesigner.<br>

## Table of contents

- [GUI](#markdown-header-gui)
- [QtDesigner](#markdown-header-qt-designer)
- [Widgets](#markdown-header-widgets)

<a id="markdown-header-gui"></a>
# GUI

To use this module in gui just add this lines in *.pro file<br>
```
include (../dap-ui-sdk/widgets/libdap-qt-widgets.pri)
```

<a id="markdown-header-qt-designer"></a>
# QtDesigner

To use this module inside QtDesigner:<br>
- Compile module in Release
- Copy result library to ** /Tools/QtCreator/lib/Qt/plugins/designer **
- Start QtCreator
- Goto QtDesigner
- New widgets must appear at the end of all available widgets

<a id="markdown-header-widgets"></a>
# Widgets

All widgets can be found inside group @ref groupDapGuiWidgets
