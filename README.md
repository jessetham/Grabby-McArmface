# Grabby-McArmface  


**Dependencies**  
1. ros audio-common. Install by `sudo apt-get install ros-distro-audio-common`  
2. WIT speech API. Install by `pip install wit`  
3. pyaudio  

**Usage**  
1. `roslaunch audio_capture capture.launch`
2. `python wit_handler.py <token> <bool:with_file>`  
Ask Noni for a token    
3. rosrun grabby_mcarmface_ros wit_handler.py RPVCFXKC5MTTYQYEHZVZUAJSL7AGWD False  for quick test
