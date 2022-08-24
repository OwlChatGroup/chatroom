#-------------------------------------------------
#
# Project created by QtCreator 2022-08-19T16:33:39
#
#-------------------------------------------------

QT      += core gui network sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatRoom1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        login.cpp \
    register.cpp \
    ApiUtils/packet.cpp \
    ApiUtils/socket_utils.cpp \
    ApiUtils/api_utils.cpp \
    ApiUtils/tools/byte_convert.cpp \
    ApiUtils/tools/logger.cpp \
    imageviewlabel.cpp\
        owlchatmain.cpp \
        ItemInfo.cpp \
        chatbubble.cpp \
        dialog.cpp \
    useritemdelegate.cpp \
    expressionpanel.cpp \
    addnewfriends.cpp \
    personalmain.cpp \
    friendshow.cpp \
    ifaddnewfriendbutton.cpp \
    groupshow.cpp \
    headshots.cpp\
    ApiUtils/data/data_storage.cpp \

HEADERS  += login.h \
    register.h \
    ApiUtils/packet.h \
    ApiUtils/socket_utils.h \
    ApiUtils/api_utils.h \
    ApiUtils/tools/byte_convert.h \
    ApiUtils/tools/logger.h \
    imageviewlabel.h\
owlchatmain.h \
        ItemInfo.h \
        chatbubble.h \
        dialog.h \
        global.h \
        unit.h \
    useritemdelegate.h \
    UserListData.h \
    expressionpanel.h \
    addnewfriends.h \
    personalmain.h \
    friendshow.h \
    ifaddnewfriendbutton.h \
    groupshow.h \
    ApiUtils/data/data_storage.h \
    headshots.h

FORMS    += login.ui \
    register.ui \
owlchatmain.ui \
         dialog.ui \
    expressionpanel.ui \
    addnewfriends.ui \
    personalmain.ui \
    friendshow.ui \
    ifaddnewfriendbutton.ui \
    groupshow.ui \
    headshots.ui

CONFIG +=C++11

RESOURCES += \
    images.qrc \
    img.qrc


OBJECTS_DIR  = tmp
MOC_DIR      = tmp


DISTFILES += \
    images/appIcon.png \
    images/emoji/pic_60/image_emoticon.png \
    images/emoji/pic_60/image_emoticon2.png \
    images/emoji/pic_60/image_emoticon3.png \
    images/emoji/pic_60/image_emoticon4.png \
    images/emoji/pic_60/image_emoticon5.png \
    images/emoji/pic_60/image_emoticon6.png \
    images/emoji/pic_60/image_emoticon7.png \
    images/emoji/pic_60/image_emoticon8.png \
    images/emoji/pic_60/image_emoticon9.png \
    images/emoji/pic_60/image_emoticon10.png \
    images/emoji/pic_60/image_emoticon11.png \
    images/emoji/pic_60/image_emoticon12.png \
    images/emoji/pic_60/image_emoticon13.png \
    images/emoji/pic_60/image_emoticon14.png \
    images/emoji/pic_60/image_emoticon15.png \
    images/emoji/pic_60/image_emoticon16.png \
    images/emoji/pic_60/image_emoticon17.png \
    images/emoji/pic_60/image_emoticon18.png \
    images/emoji/pic_60/image_emoticon19.png \
    images/emoji/pic_60/image_emoticon20.png \
    images/emoji/pic_60/image_emoticon21.png \
    images/emoji/pic_60/image_emoticon22.png \
    images/emoji/pic_60/image_emoticon23.png \
    images/emoji/pic_60/image_emoticon24.png \
    images/emoji/pic_60/image_emoticon25.png \
    images/emoji/pic_60/image_emoticon26.png \
    images/emoji/pic_60/image_emoticon27.png \
    images/emoji/pic_60/image_emoticon28.png \
    images/emoji/pic_60/image_emoticon29.png \
    images/emoji/pic_60/image_emoticon30.png \
    images/emoji/pic_60/image_emoticon31.png \
    images/emoji/pic_60/image_emoticon32.png \
    images/emoji/pic_60/image_emoticon33.png \
    images/emoji/pic_60/image_emoticon61.png \
    images/emoji/pic_60/image_emoticon66.png \
    images/emoji/pic_60/image_emoticon67.png \
    images/emoji/pic_60/image_emoticon68.png \
    images/emoji/pic_60/image_emoticon69.png \
    images/emoji/pic_60/image_emoticon70.png \
    images/emoji/pic_60/image_emoticon71.png \
    images/emoji/pic_60/image_emoticon72.png \
    images/emoji/pic_60/image_emoticon73.png \
    images/emoji/pic_60/image_emoticon74.png \
    images/emoji/pic_60/image_emoticon75.png \
    images/emoji/pic_60/image_emoticon76.png \
    images/emoji/pic_60/image_emoticon85.png \
    images/emoji/pic_60/image_emoticon86.png \
    images/emoji/pic_60/image_emoticon87.png \
    images/emoji/pic_60/image_emoticon88.png \
    images/emoji/pic_60/image_emoticon89.png \
    images/emoji/pic_60/image_emoticon90.png \
    images/emoji/pic_60/image_emoticon91.png \
    images/emoji/pic_60/image_emoticon92.png \
    images/emoji/pic_60/image_emoticon93.png \
    images/emoji/pic_60/image_emoticon94.png \
    images/emoji/pic_60/image_emoticon95.png \
    images/emoji/pic_60/image_emoticon96.png \
    images/emoji/pic_60/image_emoticon97.png \
    images/emoji/pic_60/image_emoticon98.png \
    images/emoji/pic_60/image_emoticon99.png \
    images/emoji/pic_60/image_emoticon100.png

#RC_FILE = logo.rc

