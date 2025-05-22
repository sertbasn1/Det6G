//optimization
#include <Python.h>
#include <time.h>
#include <stdexcept>
#include <iostream>
#include <stdio.h>

//STD
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <iterator>     // std::advance
#include <sstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include <list>

using namespace std;
// g++ omnettest.cc -I/usr/include/python3.11 -lpython3.11 -lstdc++

void get_optimal_assignment() {
    
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
    
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pList, *pList_assignments;
    
    pName = PyUnicode_DecodeFSDefault("omnettest");
    pModule = PyImport_Import(pName);       // loaded script
    
   // PyErr_PrintEx(1);  // debug
    
    if(pModule == NULL)
        throw std::invalid_argument("Script cannot be loaded.\n");
    
    pDict = PyModule_GetDict(pModule);
    pFunc = PyDict_GetItemString(pDict, "main"); // load function
    
        
    PyObject* pResult = PyObject_CallObject(pFunc, nullptr); // call function
    
    //PyErr_PrintEx(1);  // debug

}


int main() {
    
    Py_Initialize();
    
    get_optimal_assignment();
        
    Py_Finalize();
    return 0;
}
