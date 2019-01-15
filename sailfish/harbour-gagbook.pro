TARGET = harbour-gagbook

DEFINES += APP_VERSION=\\\"$$VERSION\\\" HAS_LIBRESOURCEQT

QT += core qml quick network

CONFIG += sailfishapp c++11 #link_pkgconfig
PKGCONFIG += libresourceqt5

INCLUDEPATH += ..

HEADERS += \
    ../src/qmlutils.h \
    ../src/appsettings.h \
    ../src/gagbookmanager.h \
    ../src/gagmodel.h \
    ../src/gagobject.h \
    ../src/gagrequest.h \
    ../src/networkmanager.h \
    ../src/gagimagedownloader.h \
    ../src/gagcookiejar.h \
    ../src/volumekeylistener.h \
    ../src/votingmanager.h \
    ../src/ninegagapiclient.h \
    ../src/ninegagapirequest.h \
    ../src/sectionmodel.h

SOURCES += main.cpp \
    ../src/qmlutils.cpp \
    ../src/appsettings.cpp \
    ../src/gagbookmanager.cpp \
    ../src/gagmodel.cpp \
    ../src/gagobject.cpp \
    ../src/gagrequest.cpp \
    ../src/networkmanager.cpp \
    ../src/gagimagedownloader.cpp \
    ../src/gagcookiejar.cpp \
    ../src/volumekeylistener.cpp \
    ../src/votingmanager.cpp \
    ../src/ninegagapiclient.cpp \
    ../src/ninegagapirequest.cpp \
    ../src/sectionmodel.cpp

OTHER_FILES += \
    rpm/$${TARGET}.spec \
    rpm/$${TARGET}.yaml \
    rpm/$${TARGET}.changes \
    $${TARGET}.desktop \
    $${TARGET}.png \
    ../README.md \
    ../LICENSE \
    qml/AboutPage.qml \
    qml/AppSettingsPage.qml \
    qml/CommentsPage.qml \
    qml/Constant.qml \
    qml/CoverPage.qml \
    qml/GagDelegate.qml \
    qml/ImagePage.qml \
    qml/InfoBanner.qml \
    qml/main.qml \
    qml/MainPage.qml \
    qml/OpenLinkDialog.qml \
    qml/SectionPage.qml \
    qml/SimpleListItem.qml \
    qml/LoginDialog.qml \
    qml/Images/icon-gif-play.png \
    qml/VideoComponent.qml
