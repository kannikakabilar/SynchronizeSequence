# SynchronizeSequence
The Synchronize Sequence program utilizes the multiple cores of a device's processor effectively to calculate high orders of a sequence synchronously. The program makes use of fork system call to run processes concurrently and share results through a pipe file descriptor. This program can be used to execute memory occupying and cpu bound processes simultaneously allowing it to reduce its completion time. The program can also be easily modified by adding execl() system call to execute another program synchronously while calculating the order of the sequence. 

# How to Run
Navigate to the repository where the program is located
```md
> make
> ./syncseq
```
Execute the above commands in terminal and enter user inputs as prompted. The below image illustrates a demonstration. <br />
![Quote](https://github.com/kannikakabilar/SynchronizeSequence/blob/main/Screen%20Shot%202022-04-15%20at%207.46.52%20PM.png)
