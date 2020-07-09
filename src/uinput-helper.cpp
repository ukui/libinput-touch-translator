#include "uinput-helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/uinput.h>
#include <linux/input.h>

#include <QDebug>

static struct uinput_user_dev uinput_dev;
static int uinput_fd;

int creat_user_uinput(void);
int report_key(unsigned int type, unsigned int keycode, unsigned int value);

static UInputHelper *instance = nullptr;

UInputHelper *UInputHelper::getInstance()
{
    if (!instance)
        instance = new UInputHelper;
    return instance;
}

void UInputHelper::executeShortCut(const QKeySequence &shortCut)
{
    auto list = QKeySequence::listFromString(shortCut.toString());
    qDebug()<<list;
    auto keys = parseShortcut(shortCut);
    for (auto key: keys) {
        int ret = report_key(EV_KEY, key, 1);
        if (ret != 0) {
            qDebug()<<"failed, try recreate uinput";
            creat_user_uinput();
            report_key(EV_KEY, key, 1);
        }
    }
    for (auto key: keys) {
        report_key(EV_KEY, key, 0);
    }
}

QList<int> UInputHelper::parseShortcut(const QKeySequence &shortCut)
{
    QList<int> keys;

    auto shortcutString = shortCut.toString();
    QStringList stringList = shortcutString.split("+", QString::SkipEmptyParts);
    for (auto string : stringList) {
        int key = m_hash.value(string);
        if (key > 0) {
            keys<<key;
        }
    }

    return keys;
}

UInputHelper::UInputHelper(QObject *parent) : QObject(parent)
{
    int ret = creat_user_uinput();
    if (ret < 0) {
        qErrnoWarning(ret, "can't create uinput device, exit");
        exit(ret);
    }

    m_hash.insert("Alt", KEY_LEFTALT);
    m_hash.insert("Ctrl", KEY_LEFTCTRL);
    m_hash.insert("Shift", KEY_LEFTSHIFT);
    m_hash.insert("Tab", KEY_TAB);
    m_hash.insert("Super", KEY_LEFTMETA);
    m_hash.insert("Meta", KEY_LEFTMETA);
    m_hash.insert("Space", KEY_SPACE);
    m_hash.insert("Left", KEY_LEFT);
    m_hash.insert("Right", KEY_RIGHT);
    m_hash.insert("Up", KEY_UP);
    m_hash.insert("Down", KEY_DOWN);
    m_hash.insert("Delete", KEY_DELETE);
    m_hash.insert("Del", KEY_DOT);
    m_hash.insert("Plus", KEY_EQUAL);
    m_hash.insert("Minus", KEY_MINUS);
    m_hash.insert("Backspace", KEY_BACKSPACE);
    m_hash.insert("Esc", KEY_ESC);

    m_hash.insert("0", KEY_0);
    m_hash.insert("1", KEY_1);
    m_hash.insert("2", KEY_2);
    m_hash.insert("3", KEY_3);
    m_hash.insert("4", KEY_4);
    m_hash.insert("5", KEY_5);
    m_hash.insert("6", KEY_6);
    m_hash.insert("7", KEY_7);
    m_hash.insert("8", KEY_8);
    m_hash.insert("9", KEY_9);

    m_hash.insert("A", KEY_A);
    m_hash.insert("B", KEY_B);
    m_hash.insert("C", KEY_C);
    m_hash.insert("D", KEY_D);
    m_hash.insert("E", KEY_E);
    m_hash.insert("F", KEY_F);
    m_hash.insert("G", KEY_G);
    m_hash.insert("H", KEY_H);
    m_hash.insert("I", KEY_I);
    m_hash.insert("J", KEY_J);
    m_hash.insert("K", KEY_K);
    m_hash.insert("L", KEY_L);
    m_hash.insert("M", KEY_M);
    m_hash.insert("N", KEY_N);
    m_hash.insert("O", KEY_O);
    m_hash.insert("P", KEY_P);
    m_hash.insert("Q", KEY_Q);
    m_hash.insert("R", KEY_R);
    m_hash.insert("S", KEY_S);
    m_hash.insert("T", KEY_T);
    m_hash.insert("U", KEY_U);
    m_hash.insert("V", KEY_V);
    m_hash.insert("W", KEY_W);
    m_hash.insert("X", KEY_X);
    m_hash.insert("Y", KEY_Y);
    m_hash.insert("Z", KEY_Z);

    m_hash.insert("F1", KEY_F1);
    m_hash.insert("F2", KEY_F2);
    m_hash.insert("F3", KEY_F3);
    m_hash.insert("F4", KEY_F4);
    m_hash.insert("F5", KEY_F5);
    m_hash.insert("F6", KEY_F6);
    m_hash.insert("F7", KEY_F7);
    m_hash.insert("F8", KEY_F8);
    m_hash.insert("F9", KEY_F9);
    m_hash.insert("F10", KEY_F10);
    m_hash.insert("F11", KEY_F11);
    m_hash.insert("F12", KEY_F12);
}

int creat_user_uinput(void)
{
    int i;
    int ret = 0;

    uinput_fd = open("/dev/uinput", O_RDWR | O_NDELAY);
    if(uinput_fd < 0){
        printf("%s:%d\n", __func__, __LINE__);
        return -1;//error process.
    }

    //to set uinput dev
    memset(&uinput_dev, 0, sizeof(struct uinput_user_dev));
    snprintf(uinput_dev.name, UINPUT_MAX_NAME_SIZE, "uinput-custom-dev");
    uinput_dev.id.version = 1;
    uinput_dev.id.bustype = BUS_VIRTUAL;

    ioctl(uinput_fd, UI_SET_EVBIT, EV_SYN);
    ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinput_fd, UI_SET_EVBIT, EV_MSC);

    for(i = 0; i < 256; i++){
        ioctl(uinput_fd, UI_SET_KEYBIT, i);
    }

    ret = write(uinput_fd, &uinput_dev, sizeof(struct uinput_user_dev));
    if(ret < 0){
        printf("%s:%d\n", __func__, __LINE__);
        return ret;//error process.
    }

    ret = ioctl(uinput_fd, UI_DEV_CREATE);
    if(ret < 0){
        printf("%s:%d\n", __func__, __LINE__);
        //close(uinput_fd);
        return ret;//error process.
    }
}

int report_key(unsigned int type, unsigned int keycode, unsigned int value)
{
    struct input_event key_event;
    int ret;

    memset(&key_event, 0, sizeof(struct input_event));

    gettimeofday(&key_event.time, NULL);
    key_event.type = type;
    key_event.code = keycode;
    key_event.value = value;
    ret = write(uinput_fd, &key_event, sizeof(struct input_event));
    if(ret < 0){
        printf("%s failed:%d\n", __func__, __LINE__);
        return ret;//error process.
    }

    gettimeofday(&key_event.time, NULL);
    key_event.type = EV_SYN;
    key_event.code = SYN_REPORT;
    key_event.value = 0;//event status sync
    ret = write(uinput_fd, &key_event, sizeof(struct input_event));
    if(ret < 0){
        printf("%s:%d\n", __func__, __LINE__);
        return ret;//error process.
    }

    return 0;
}
