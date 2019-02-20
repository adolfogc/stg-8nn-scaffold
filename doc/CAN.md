## Using Libcanard_dsdlc

Run from the root directory the following command:

```bash
python3 dependencies/libcanard/dsdl_compiler/libcanard_dsdlc --outdir dsdl-gen dependencies/dsdl/uavcan
```

**Note:** this is handle automatically by CMake, refer to `CMakeLists_uavcan_dsdl.txt`.

## Tooling (using Ubuntu Linux)

### Setting up a virtual CAN interface

```bash
sudo modprobe can
sudo modprobe can_raw
sudo modprobe vcan

sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0

```

### Setting up a KVaser Leaf interface
```bash
sudo modprobe can
sudo modprobe can_raw
sudo modprobe can_dev
sudo modprobe kvaser_usb

sudo ip link set can0 type can bitrate 500000 sample-point 0.875 restart-ms 100
sudo ip link set up can0
```

### Installing and using CAN Utils

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

### Installing the UAVCAN GUI Tool
```bash
sudo apt-get install -y python3-pip python3-setuptools python3-wheel
sudo apt-get install -y python3-numpy python3-pyqt5 python3-pyqt5.qtsvg git-core
sudo -H pip3 install git+https://github.com/UAVCAN/gui_tool@master
```
