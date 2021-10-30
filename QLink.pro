QT += widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tutorials/widgets/childwidget
INSTALLS += target

HEADERS += \
    QLink.h \
    constant.h \
    enum.h \
    linemask.h \
    playinterface.h \
    qblock.h \
    role.h \
    statusui.h

SOURCES = main.cpp \
    QLink.cpp \
    linemask.cpp \
    playinterface.cpp \
    qblock.cpp \
    role.cpp \
    statusui.cpp
