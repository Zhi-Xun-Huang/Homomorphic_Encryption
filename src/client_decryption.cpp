#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>
#include "seal/seal.h"

using namespace std;
using namespace seal;

int main(int argc, char *argv[]) {
    // Process exceptioin
    string help = "help";
    if (argc != 2 && argc != 3) {
        cerr << "Missing or too many arguments, type './decrypt help' to check info." << endl;
        return 1;
    }
    if (argv[1] == help) {
        cout << "./decrypt <ciphertext_filename> <secretkey_filename>" << endl;
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

    // Input ciphertext
    ifstream incipher(argv[1], ios::binary);
    if (!incipher.is_open()) {
        cerr << "Failed to open " << argv[1] << endl;
        return 1;
    }

    // Load ciphertext
    Ciphertext ciphertext;
    try {
        ciphertext.load(context, incipher);
    }   
    catch (const exception& excepted) {
        cerr << "Failed to load " << argv[1] << ": " << excepted.what() << endl;
        return 1;
    }
    incipher.close();

    // Input secretkey
    ifstream inskey(argv[2], ios::binary);
    if (!inskey.is_open()) {
        cerr << "Failed to open " << argv[2] << endl;
        return 1;
    }

    // Load secretkey
    SecretKey secret_key;
    try {
        secret_key.load(context, inskey);
    }   
    catch (const exception &excepted) {
        cerr << "Failed to load " << argv[2] << ": " << excepted.what() << endl;
        return 1;
    }
    inskey.close();

    // Decrypt
    Decryptor decryptor(context, secret_key);
    Plaintext plaintext;
    decryptor.decrypt(ciphertext, plaintext);
    vector<double> result;
    codec.decode(plaintext, result);

    // Generate result after decrypted
    ofstream outfile("result.txt");
    if (!outfile.is_open()) {
        cerr << "Generate result.txt failed." << endl;
        return 1;
    }

    char *res = (char *)malloc(128 * sizeof(char));
    sprintf(res, "%.0f", result[0]);
    outfile << res << endl;
    outfile.close();
    cout << "result: " << res << endl;
    cout << "result.txt generated." << endl;
    free(res);

    return 0;
}