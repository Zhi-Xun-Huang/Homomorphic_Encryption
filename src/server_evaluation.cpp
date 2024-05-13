#include <iostream>
#include <fstream>
#include <sstream>
#include "seal/seal.h"

using namespace std;
using namespace seal;

// 
int main(int argc, char *argv[]) { 
    // Process exceptioin
    string help = "help";
    if (argc != 2 && argc != 3 && argc != 4) {
        cerr << "Missing or too many arguments, type './evaluate help' to check info." << endl;
        return 1;
    }
    if (argv[1] == help) {
        cout << "./evaluate <operation> <argument_1_filename> <additional: argument_2_filename>" << endl;
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
    // Get require operation
    string operation[] = {"add", "sub", "mul", "pow"};
    // Setup evaluator
    Evaluator evaluator(context);

    Ciphertext arg1, arg2;
    // Open file for arg1
    ifstream in_arg1(argv[2], ios::binary);
    arg1.load(context, in_arg1);
    in_arg1.close();

    // Evaluate
    int i;
    for (i = 0; i <= 4; i++) {
        if (i == 4) {
            cerr << "Invalid operation: " << argv[1] << endl;
            return 1;
        }
        if (argv[1] == operation[i])
            break;
    }
    if (argv[1] == operation[3]) {
        evaluator.square_inplace(arg1);
        cout << "Operation square completed." << endl;
    }
    else {
        ifstream in_arg2(argv[3], ios::binary);
        arg2.load(context, in_arg2);
        in_arg2.close();
        if (argv[1] == operation[0])
            evaluator.add_inplace(arg1, arg2);
        else if (argv[1] == operation[1])
            evaluator.sub_inplace(arg1, arg2);
        else if (argv[1] == operation[2])
            evaluator.multiply_inplace(arg1, arg2);
    }
    
    // Output file
    ofstream outfile("res_cipher.bin", ios::binary);
    arg1.save(outfile);
    outfile.close();
    cout << "Evaluation accompilshed. res_cipher.bin generated." << endl;

    return 0;
}