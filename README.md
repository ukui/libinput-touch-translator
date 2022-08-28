# libinput-touch-translator
Map actions in touch screen and touchpad to keyboard shortcuts or other input devices.

# Introduction

This project is aim to providing a common resolusion for translate an "unclear" touch action on PC to exsited one our computer can recognize, often a global shortcut. 

## Background

There are some similar projects, such as [**libinput-gesetures**](https://github.com/bulletmark/libinput-gestures) and [**touchegg**](http://code.google.com/p/touchegg), having a good performance about improving user interaction of touching.

However, the existed projects are almost platform related, they might work well on X11, but not well on Wayland. So, I reference their idea and reimplement a new project, which we are talking about.

## How it works

The main idea of the project is very simple. First, get the touch event and recognize them. Next, get the crosponding action which exsited in our computer. Last, execute the action for responding the touch event.

## Advantage and disadvantage

As I said, this project is aim to providing a common resolution, that means no what X11 or Wayland will get the same interaction. However, due to the tech limited, I can't not do so much at the side of event response. For now it only support the basic keyborad(mostly) and mouse event's mapping.

## Usage

I provide a graphics interface for configure the shortcut of different touch gestures. However, it must have permission for modification, because the project must run system wide. Note that the shortcut must be supported in existed system, make sure you have learn about the shortcuts in your computer, or learn about how to config them to fit into the translator. For example, map four finger swipe gesture to 'Alt+Tab' or 'Shift+Alt+Tab' for switching window.

# Hacking

## build depends (on Debian or Ubuntu)
- libinput-dev
- libudev-dev
- qtbase5-dev
- libkf5xmlgui-dev

## build & test
- mkdir build && cd build
- qmake .. && sudo make install
- run project with sudo, or use
> systemctl start libinput-touch-translator.service
- sudo /usr/bin/ukui-touch-translator-config for find usage and config them
- have a try on touchscreen and touchpad

On Ubuntu is add PPA (focal only).

> sudo add-apt-repository ppa:larue/libinput-touch-translator

> sudo apt-get update

> apt source libinput-touch-translator && sudo apt build-dep libinput-touch-translator

or

> sudo apt install libinput-touch-translator
