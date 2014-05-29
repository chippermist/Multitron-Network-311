MULTITRON NETWORK DRIVER FOR UNIX
---------------------------------
---------------------------------

This is a simple device driver for multitron which is implementing a sequential server and client system on a Unix machine. Basically, it passes the signals sent by the client running a selected/all workloads and then sends it over to the server which processes it and recieves the packet data.

PACKET COMPOSITION
------------------
------------------


Offset      Size      Description
-----------------------------------
0             4       Multitron instruction

4             2       No of bytes n in buffer

6             2       Return Value  

8             n       Data buffer


-----------------------------------

A LITTLE EXPLAINATION
---------------------

Programmatically, all calls to tronctl have been replaced with calls to tronctl net. This function should send the appropriate message to the MultiTron server, then receive and handle the response. The tronctl net function will be written by you. This function is called as follows:

int tronctl_net(int *sock, uint32_t instr, void *data, int datalen);

• sock (passed by reference): File descriptor of a socket which is connected to the server.
• instr: MultiTron instruction (the parameter formerly known as op). The format of this param-
eter is exactly the same as in previous assignments.
• data: Data buffer for the operation, if needed. This parameter should function exactly as in previous assignments.
• datalen: Size, in bytes, of the data buffer.

COMPILING AND INSTALLING
------------------------

To compile the kernel module, run `make` inside the `assign5` directory. A executable verify will be created will be created and can be executed by `./verify` to run all the workloads. And if running seperately you can execute `server` in a seperate terminal window and `client` in another with usage such as `./verify -v < workload/[workload name]`.


QUESTIONS?
----------

If you have any questions or need more material or explaination contact me. 
