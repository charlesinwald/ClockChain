# ClockChain
- Proof of concept blockchain that ensures the integrity of chronological events/data.
For example, one could prove that they submitted a given document at given time before someone else.
You can submit just a hash of the document if desired, keeping the original document secret, while still
securing the timestamp on the blockchain.  

# Potential Uses:
 - Original Content Creation
 - School Assignment Submissions
 - Record Keeping
 
# Features
 - Network I/O
 - File Persistence
 - Multithreaded/Atomicity with 

# Server usage
 - Compile with `Makefile` (tested with g++ version 7) 
 - (Optional) Modify `Blockchain.dat` with desired data.  Currently includes Genesis block and sample data, with correct format.
 - Run `Blockchain.exe`
 - Arguments:
    -p <int> : Port on which to listen (default 41100)
    -h help
# Client Usage:
 - Compile with `Makefile` (tested with g++ version 7)
 - Run `client.exe`
 - Arguments
    -    -p <int> : Port on which to send to (default 41100)
    -    -u <int> : User ID
    -    -m <String> : Document body
    -    -h help

Uses SHA-256 encryption, see SHA256_LICENSE for additonal licensing information.

