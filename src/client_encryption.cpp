#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include "seal/seal.h"

using namespace std;
using namespace seal;

template <typename T>

void outfile(string fileName, T& source) {
    ofstream outfile(fileName, ios::binary);
    source.save(outfile);
    outfile.close();
}

int main(int argc, char *argv[]) {
    // Process exceptioin
    string help = "help";
    if (argc != 2 && argc != 3) {
        cerr << "Missing or too many arguments, type './encrypt help' to check info." << endl;
        return 1;
    }
    if (argv[1] == help) {
        cout << "./encrypt <plaintext_filename> <additional: custom_publickey_filename>" << endl;
        return 0;
    }
    
    // Setup parameters
    EncryptionParameters parms(scheme_type::ckks);
    parms.set_poly_modulus_degree(8192);
    parms.set_coeff_modulus(CoeffModulus::Create(8192, {60, 40, 40, 60}));

    // Setup context
    SEALContext context(parms);
    // Setup codec
    CKKSEncoder codec(context);

    // Generate publickey
    KeyGenerator keygen(context);
    PublicKey public_key;
    keygen.create_public_key(public_key);

    // Generate secretkey
    SecretKey secret_key;
    secret_key = keygen.secret_key();

    // Open file
    string plain;
    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cerr << "Failed to open " << argv[1] << endl;
        return 1;
    }

    // Additional: Use custom publickey to encrypt plaintext
    if (argc == 3) {
        ifstream inpkey(argv[2], ios::binary);
        if (!inpkey.is_open()) {
            cerr << "Failed to open " << argv[2] << endl;
            return 1;
        }
        try {
            public_key.load(context, inpkey);
        }
        catch (const exception &excepted) {
            cerr << "Failed to load publickey: " << excepted.what() << endl;
            return 1;
        }
        inpkey.close();
    }
   
    int i = 0;
    while (getline(infile, plain)) {
        // Get lines from file
        cout << "getline: " << plain << endl;
        
        // Encode
        Plaintext encoded;
        codec.encode(stoi(plain), pow(2.0, 20), encoded);

        // Encrypt
        Encryptor encryptor(context, public_key);
        Ciphertext cipher;
        encryptor.encrypt(encoded, cipher);

        // Generate ciphertext.bin
        stringstream filename;
        while (1) {
            filename.str("");
            filename << "ciphertext_" << i << ".bin";
            ifstream file(filename.str());
            if (!file.good())
                break;
            i++;
        }
        
        outfile(filename.str(), cipher);
        cout << filename.str() << " generated." << endl;  
    }

    infile.close();

    if (argc == 2) {
        // Generate secretkey.bin
        outfile("secretkey.bin", secret_key);
        cout << "secretkey.bin generated." << endl;

        // Generate publickey.bin
        outfile("publickey.bin", public_key);
        cout << "publickey.bin generated." << endl;
    }
    else
        cout << "Using custom publickey, new publickey and secretkey didn't generate." << endl;
    
    return 0;
}

