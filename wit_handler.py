#!/usr/bin/env python

# created: Mar 18
# author: Noni
# nwHacks 2017 @UBC

import sys
from wit import Wit

import rospy
from audio_common_msgs.msg import AudioData

class voiceHandler:
    def __init__(self, access_token):
        self.response = None
        self.actions = {
            'send': self.action_send,
        }
        self.client = Wit(access_token=access_token, actions=self.actions)
        self.session_id = 0

    def action_send(self, request, response):
        print(response['text'])


    def add_actions(self):
        #TODO
        # add functionality for learning
        # add specific taught task as new entities
        pass

    def exr_words(self, sen):
        """
        Our robot onlu contains two mode: free mode and learning mode
        """
        if "intent" in sen["entities"].keys():
            exr = sen["entities"]["intent"][0]["value"]
        else:
            exr = sen["entities"]["on_off"][0]["value"]
        print exr

    def converse(self):
        pass

    def audio_cb(self, msg):
        #TODO
        # add some global variables to monitor the buttom status from serials
        #print(msg.data.strip("\\").split())
        #resp = self.client.speech(int(msg.data, 16), None, {'Content-Type': 'audio/raw;encoding=unsigned-integer;bits=8;rate=8000;endian=little'})
        #print('resp = '+ str(resp))
        pass

    def audio_test(self):
        with open('../audio/sample1.wav', 'rb') as f:
            resp = self.client.speech(f, None, {'Content-Type': 'audio/wav'})
        #print('Yay, got Wit.ai response: ' + str(resp))
        self.exr_words(resp)

    def main(self, test_with_file):
        if test_with_file == "True":
            self.audio_test()
        else:
            rospy.Subscriber('/audio', AudioData, self.audio_cb)
            rospy.spin()
            #self.client.interactive()

if __name__ == "__main__":
    rospy.init_node("withandler")

    if len(sys.argv) != 3:
        print('usage: python ' + sys.argv[0] + ' <wit-token>' + '<bool:with_file>')
        exit(1)
    access_token = sys.argv[1]
    with_file = sys.argv[2]

    handler = voiceHandler(access_token)
    handler.main(test_with_file = with_file)
