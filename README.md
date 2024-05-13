# Easy to use Homomorphic Encryption
This project is based on Microsoft SEAL CKKS.

## Before Start the Deployment:

### Follow the steps below:
Step 1: Clone Microsoft SEAL from https://github.com/microsoft/SEAL.git<br>
```
git clone https://github.com/microsoft/SEAL.git
```
Step 2: Make sure that cmake, make, gcc-12 and g++12 had been installed on your platform.<br>
```
gcc --version
g++ --version
cmake --version
make --version
```
If you had installed gcc-12 and g++-12 but default version did not match. Run these commands.
```
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100
```
Step 3: Type these commands below:
```
cd SEAL-mian
cmake -S . -B build
cmake --build build
sudo cmake --install build
```
Step 4: Leave seal's folder and clone this project on your platform.<br>
```
cd ..
git clone https://github.com/Zhi-Xun-Huang/Homomorphic_Encryption.git
```
Step 5: type these commands below:
```
cd Homomorphic_Encryption/build
cmake ..
make -j
```
Step 6: Now the project has been built accomplish.

## Before Start Encryption
Create a plaintext file in 'Homomorphic_Encryption/build'.<br>
Put some integers you wanna encrypted. One integer per line.
```
vim plaintext.txt
```
Execute encrypt
```
./encrypt plaintext.txt
```
After execution, the publickey.bin, secretkey.bin and ciphertext_n.bin would been generated.

### More details of encryption
If you wanna use publickey which been generated before, type command like this below:
```
./encrypt plaintext.txt publickey.bin
```
After execution, only the ciphertext_n.bin would been generated.

## Before Start Evaluation
There are the operations could been executed in evaluate
1. Addition
```
./evaluate add ciphertext_0.bin ciphertext_1.bin
```
2. Subtraction
```
./evaluate sub ciphertext_0.bin ciphertext_1.bin
```
3. Multiplication
```
./evaluate mul ciphertext_0.bin ciphertext_1.bin
```
4. Square
```
/evaluate pow ciphertext_0.bin
```
After execution, the res_cipher.bin would been generated.

## Before Decryption

Assure the secretkey and ciphertext matched or result of decryption would been incorrect.<br>
To decrypt the ciphertext:
```
./decrypt ciphertext.bin secretkey.bin
```
After execution, the result.txt would been generated.

## That's All. Have Fun.