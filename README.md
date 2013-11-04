 ---------------------------------------------------------------------------------------------------------- BT#1 ------------------------------------------------------------------------------------------------------------
                                BT#1 is the less user friendly, more effective and efficient BT program. It takes in a file called "newfile" and transmits the data via UDP to the requesting client. Who prints the file out as 
                                "newfile.txt". The IP addresses and port values are hard coded in and they don't accept broadcast IP ranges. BT#1 didn't adhere to the Assignment specs. but the decision to rework the entire thing
                                didn't happen until the last minute. BT#1 tracks and reliably transmits 100% of the packets. There are some bugs though. For example, if the first packet is dropped, the programs will hang. All of the
                                address memory is done manually which was another big error. Select makes like a LOT! easier. Like BT#2, BT#1 has reliable MD5 checking, but it ends the process after the last packet is delivered. I uploaded
                                both programs just in the hope that you would see that the progress as I improved between prototypes. Also, the irony is that the shortcomings of each programs are the strengths of the other. One transmits
                                but only to specific hosts while the other transmits less reliably to all hosts. If you take the best of both programs and multiply them by the effort I think you can see I put in, I'm sure you can 
                                overlook their downfalls. If not, well...at least they'll both compile! I'll describe how each program is used:
                                
                                
                                
                                BT#1: Serv.c runs as the sender, the file "newfile" must be in its directory. This information will be transmitted reliably to the "./Recv" who asks. It hasn't been tested throughly in the lab for 
                                concurrent connections but I had to choose with BT#1 or BT#2 and well...BT#2 broadcasts so he won. 
                                
                                
                                BT#2: BT#2 has a usage message specified by '-h' so it should be easy to run. The way to guarantee that the file is received is to let it run longer. The longer it runs the higher the probability all 
                                the packets were received. Also, BT#2 has been tested for concurrent connections and can broadcast files very quickly to multiple sources. 
                                
                                
                                