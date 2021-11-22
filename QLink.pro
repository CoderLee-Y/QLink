QT += widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tutorials/widgets/childwidget
INSTALLS += target
TEMPLATE = app
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
#QT += testlib

HEADERS += \
    constant.h \
    controller.h \
    enum.h \
    linemask.h \
    menu.h \
    qblock.h \
    qlink.h \
    role.h \
    statusui.h \
    unittest.h

SOURCES = main.cpp \
    controller.cpp \
    linemask.cpp \
    menu.cpp \
    qblock.cpp \
    qlink.cpp \
    role.cpp \
    statusui.cpp \
    unittest.cpp
