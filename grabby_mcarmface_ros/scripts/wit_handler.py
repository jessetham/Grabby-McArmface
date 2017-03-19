#!/usr/bin/env python

# created: Mar 18
# author: Noni
# nwHacks 2017 @UBC

import sys
from wit import Wit

import pyaudio
import time
from struct import pack
import wave
from array import array
from sys import byteorder

import rospy
from std_msgs.msg import String, UInt8
from grabby_mcarmface_ros.srv import TurnOnMic

class voiceHandler:
    def __init__(self, access_token):
        self.response = None
        self.actions = {
            'send': self.action_send,
        }
        self.client = Wit(access_token=access_token, actions=self.actions)
        self.session_id = 0
        self.mode_pub = rospy.Publisher('/grabby/mode', String, queue_size = 5)
        self.audio_parm = {"chunk" :1024, "FORMAT" :pyaudio.paInt16, "RATE" :44100, "CHANNELS":1}
        self.p = pyaudio.PyAudio()
        self.mic_on = 0

    def action_send(self, request, response):
        """
        Text communication.. well we don't need it for now
        """
        print response['text']

    def exr_words(self, sen):
        """
        Our robot onlu contains two mode: free mode and learning mode
        """
        if sen["entities"] == None:
            exr = "free"
        elif "intent" in sen["entities"].keys():
            exr = sen["entities"]["intent"][0]["value"]
        elif "on_off" in sen["entities"].keys():
            exr = sen["entities"]["on_off"][0]["value"]
        elif "movement" in sen["entities"].keys():
            exr = sen["entities"]["on_off"][0]["value"]
        print exr

        msg = String()
        msg.data = exr
        self.mode_pub.publish(msg)

    def get_mic(self, req):
        """
        button callback that trigers the voice command
        """
        if req == 1:
            self.mic_on = 1

        if self.mic_on == 1:
            stream = self.p.open(format=self.audio_parm["FORMAT"], channels=self.audio_parm["CHANNELS"], rate=self.audio_parm["RATE"], input=True, output=True,frames_per_buffer=self.audio_parm["chunk"])
            data = array('h')
            time_s = time.clock()
            while time.clock() < time_s + 0.02: #TODO: as above
                print time.clock()
                record = array('h', stream.read(self.audio_parm["chunk"]))
                if byteorder == 'big':
                    record.byteswap()
                data.extend(record)

            sample_width = self.p.get_sample_size(self.audio_parm["FORMAT"])
            stream.stop_stream()
            stream.close()
            self.p.terminate()

            data = pack('<' + ('h' * len(data)), *data)
            wave_file = wave.open('/tmp/tmp.wav', 'wb')
            wave_file.setnchannels(self.audio_parm["CHANNELS"])
            wave_file.setsampwidth(sample_width)
            wave_file.setframerate(self.audio_parm["RATE"])
            wave_file.writeframes(data)
            wave_file.close()

            resp = self.client.speech('/tmp/tmp.wav', None, {'content-type': 'audio/raw;encoding=unsigned-integer;bits=16;rate=44100;endian=little'})
            print 'resp = ', str(resp)

        else:
            print "waiting for client..."

    def audio_test(self):
        with open('../audio/sample6.wav', 'rb') as f:
            resp = self.client.speech(f, None, {'Content-Type': 'audio/wav'})
        print 'Yay, got Wit.ai response: ' , str(resp)
        self.exr_words(resp)

    def main(self, test_with_file):
        if test_with_file == "True":
            self.audio_test()
        else:
            #rospy.Subscriber('/button', Int8, self.button_cb)
            self.rospy.Service('turn_on_mic', TurnOnMic, self.get_mic)

if __name__ == "__main__":
    #reload(sys)
    #sys.setdefaultencoding('utf8')
    rospy.init_node("withandler")

    if len(sys.argv) != 3:
        print('usage: python ' + sys.argv[0] + ' <wit-token>' + '<bool:with_file>')
        exit(1)
    access_token = sys.argv[1]
    with_file = sys.argv[2]

    handler = voiceHandler(access_token)
    handler.main(test_with_file = with_file)
    rospy.spin()
