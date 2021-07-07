QT += widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tutorials/widgets/childwidget
INSTALLS += target

HEADERS += \
    QLink.h

SOURCES = main.cpp \
    QLink.cpp
