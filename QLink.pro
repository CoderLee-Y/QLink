QT += widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tutorials/widgets/childwidget
INSTALLS += target

HEADERS += \
    QLink.h \
    constant.h \
    controller.h \
    enum.h \
    linemask.h \
    menu.h \
    qblock.h \
    role.h \
    statusui.h

SOURCES = main.cpp \
    QLink.cpp \
    controller.cpp \
    linemask.cpp \
    menu.cpp \
    qblock.cpp \
    role.cpp \
    statusui.cpp
