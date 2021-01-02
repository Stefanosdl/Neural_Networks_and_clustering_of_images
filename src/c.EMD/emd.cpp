#include <iostream>
#include <Python/Python.h>
#include "headers/common.hpp"
#include "headers/search.hpp"
#include "headers/handle-input.hpp"
using namespace std;

int main(int argc, char **argv) {
    uint32_t number_of_images = 0;
    uint32_t number_of_query_images = 0;
    uint64_t d_original = 0;
    string output_file, query_file_original_space, labels_query;
    ofstream o_file;

    // Begin Python interface
    PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *presult;
    // Initialize the Python Interpreter
    Py_Initialize();
    // Build the name object
    pName = PyString_FromString((char*)"emd");
    // Load the module object
    pModule = PyImport_Import(pName);
    // pDict is a borrowed reference 
    pDict = PyModule_GetDict(pModule);
    // pFunc is also a borrowed reference 
    pFunc = PyDict_GetItemString(pDict, (char*)"someFunction");

    if (PyCallable_Check(pFunc)) {
        pValue=Py_BuildValue("(z)",(char*)"something");
        PyErr_Print();
        printf("Let's give this a shot!\n");
        presult=PyObject_CallObject(pFunc,pValue);
        PyErr_Print();
    } else {
        PyErr_Print();
    }
    printf("Result is %d\n",PyInt_AsLong(presult));
    Py_DECREF(pValue);

    // Clean up
    Py_DECREF(pModule);
    Py_DECREF(pName);

    // Finish the Python Interpreter
    Py_Finalize();

    cout << "Finished with python" << endl;

    handleInput(argc, argv, &number_of_images, &d_original, &output_file, &query_file_original_space, &labels_query);
    // open output file
    o_file.open(output_file);
    if (!o_file.is_open()) {
        cerr << "Output file can't be opened" << endl;
        exit(ERROR);
    }
    do {
        readFileOriginalSpace(query_file_original_space, QUERY_FILE, &number_of_query_images, &d_original);
        readFileOriginalSpace(labels_query, QUERY_LABELS, &number_of_query_images, &d_original);
        
        // printFiles(number_of_images, number_of_query_images, d_original);
        for (uint32_t q_num = 0; q_num < number_of_query_images; q_num++) {
            // Brute
            
            // EMD

        }
        // writeLastMeta(&o_file, number_of_query_images, d_original);
        o_file.close();
        
        handleReExecution(&number_of_images, &d_original, &output_file, &query_file_original_space, &labels_query);
        // open output file
        o_file.open(output_file);
        if (!o_file.is_open()){
            cerr << "Output file can't be opened" << endl;
            exit(ERROR);
        }
    } while (true);
    o_file.close();
    
    // Free spaces
    for (uint32_t i = 0; i < number_of_images;i++) {
        delete[] all_images_original_space[i];
    }
    for (uint32_t i = 0; i < number_of_query_images;i++) {
        delete[] query_images_original_space[i];
    }
    delete[] all_images_original_space;
    delete[] query_images_original_space;
    delete[] all_images_labels;
    delete[] query_images_labels;
    return SUCCESS;
    
    // int query_image[] = {0,1,3};
    // int test_image[] = {5,6,8};
    // // ............... Call EMD
    // size_t size_q = sizeof query_image;
    // size_t size_t = sizeof test_image;
    // cout << EMDdistance(query_image, size_q, test_image, size_t) << endl;
}
