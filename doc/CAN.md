## Tooling

### Virtual CAN on Ubuntu Linux

```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

### KVaser Leaf on Ubuntu Linux
```bash
sudo modprobe kvaser_usb
sudo ip link set can0 type can bitrate 500000
sudo ip link set up can0
```

### CAN Utils on Ubuntu Linux

#### Installation
```bash
sudo apt-get install can-utils
```

#### Listening for messages
```bash
candump vcan0
```

#### Sending a message
The following command sends a message with identifier 0x1 and eight bytes of data `11223344556677AA`.
```bash
cansend vcan0 001#11223344556677AA
```

### UAVCAN GUI Tool on Ubuntu Linux
```bash
sudo apt-get install -y python3-pip python3-setuptools python3-wheel
sudo apt-get install -y python3-numpy python3-pyqt5 python3-pyqt5.qtsvg git-core
sudo -H pip3 install git+https://github.com/UAVCAN/gui_tool@master
```