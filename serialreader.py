import os
import json
import serial
import random
from time import sleep


devices = ['/dev/ttyACM1', '/dev/ttyACM0']
baud = 57600


class NoSerialDeviceError(FileNotFoundError):
    def __init__(self, message, errors):
        super(NoSerialDeviceError, self).__init__(message)

class DummyPIDSerialReader:
    def __init__(self, devicelist=None, baud=57600):
        print('Using DUMMY PID Serial Reader')
        self.devicelist = devicelist
        self.baud = baud
        self.ser = None

    def read_json(self):
        sleep(0.5)
        return {'temp': random.randint(410,430), 'dev': 'dummyIP'}

    def close(self):
        pass

class PIDSerialReader:
    def __init__(self, devicelist, baud=57600):
        self.devicelist = [devicelist] if isinstance(devicelist, str) else devicelist
        self.baud = baud
        self.ser = None
        self.get_connection()



    def get_connection(self):
        for dev in self.devicelist:
            if os.path.exists(dev):
                self.ser = serial.Serial(dev, baud)
                self.ser.read_all()  # clear buffer
                return self.ser

        raise NoSerialDeviceError('No serial devices were found in list: ', self.devicelist)

    def readline(self):
        data = self.ser.readline()
        return data.decode('utf8').replace('\r\n', '\n')

    def read(self):
        return self.ser.read()

    def read_json(self):
        """
        Read data from the serial connection and try to parse it as json
        :return:
        """
        data = self.ser.readline()
        print('{}'.format(data))
        try:
          data = data.decode('utf8').strip()
        except UnicodeDecodeError:
          pass
        # print(data)
        jdata = {'status': 'failed to decode'}
        try:
            jdata = json.loads(data)
        except json.decoder.JSONDecodeError:
            return None
        except TypeError: # binary packet
            # print('AB[{}]'.format(data))
            return None
        return jdata

    def clear_buffer(self):
        self.ser.read_all()

    def close(self):
        self.ser.close()


class HastyMessageContainer:
    def __init__(self):
        # nasty hack in order to pass messages
        self.buffer = []

    def push(self, data):
        self.buffer.append(data)

    def recent(self):
        if self.buffer:
            return self.buffer[-1]
        else:
            return {'status': 'no data in buffer'}


if __name__ == '__main__':
    devicelist = ['/dev/ttyACM1', '/dev/ttyACM0', '/dev/ttyUSB0', '/dev/ttyUSB1']
    devicelist = [dev for dev in devicelist if os.path.exists(dev)]
    print("\nAvailable devices:")
    for i, dev in enumerate(devicelist):
        print('-[{}]: {}'.format(i, dev))

    print("Enter a number to select a device:")
    while True:
        res = input()
        try:
            i = int(res)
            dev = devicelist[i]
            break
        except (ValueError, IndexError):
            print("ERR: Invalid: ", res)

    print(dev)

    device = PIDSerialReader(dev)
    device.clear_buffer()

    while True:
        try:
            data = device.readline()
            print(data, end='')
        except KeyboardInterrupt:
            break
    device.close()
