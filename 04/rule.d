# /etc/udev/rules.d/99-usb-keyboard.rules
ACTION=="add", SUBSYSTEM=="usb", ATTR{bInterfaceClass}=="03", ATTR{bInterfaceProtocol}=="01", RUN+="/sbin/insmod /root/little-penguin/04/main.ko"
