# CIS241Final
Script to import files and get hash of each file using Sha1. The hash of each file will be complied into a hash tree 
and the final hash will represent all file hashes. 

To run:

        Make file: make
  
        Run script: ./finalHash
  
  Follow instructions on menu screen:
  
       1) Add file: Enter path to specific file with file name to hash file (example: "test.txt")
       2) Add directory: Add path to directory to hash entire directory and subdirectories. Assumes files end in
                         a valid extension. (ie .txt etc)
       3) View Hashes: View list of all file hashes currently implemented in program
       4) Get final hash: Compile all hashes togethere into a tree and view final hash of all files. 
       5) Exit 
       
  After final hash is created follow instructions on next menu screen:
  
       1) Print tree: View all hashes in tree. Tree is printed vertically with an extra space between each level
       2) Compare files: Compare final hash created to a hash previously created and stored in a text file. 
                         Assumes file for comparison has a line "Final Hash: " with the hash to compare to. 
       3) Exit
       
       
  Ran and tested on Windows OS
