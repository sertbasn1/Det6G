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

// g++ test.cc -I/usr/include/python2.7 -lpython2.7 -lstdc++

struct Demand {
    int index;
    
    int talker;
    int listener;
    
    int service_type; // 0-7 service priority class
    
    float max_data; // maximum data rate to reserve links
    float max_latency; // maximum tolerable latency
    
    Demand() {index = -1; talker = 0; listener = 0; service_type = 0; max_data = 0.0; max_latency = 0.0;}
    
    Demand(int i, int t, int l, int s, float d, float lt) { index = i; talker = t; listener = l; service_type = s; max_data = d; max_latency = lt;}
    
    Demand& operator=(const Demand& d) { index = d.index; talker = d.talker; listener = d.listener; service_type = d.service_type; max_data = d.max_data; max_latency = d.max_latency; return *this;}
    
    Demand(const Demand &d) { index = d.index; talker = d.talker; listener = d.listener; service_type = d.service_type; max_data = d.max_data; max_latency = d.max_latency; }
};

struct Assignment {
    int demand_index;
    float data;
    vector<int> path;
    int path_index;
    
    Assignment() { demand_index = -1; data = 0.0; path_index = -1; }
    
    Assignment(int i, float d, vector<int> p, int j) { demand_index = i; data = d; path = p;  path_index = j;}
    
    Assignment& operator=(const Assignment& a) { demand_index = a.demand_index; data = a.data; path = a.path; path_index = a.path_index; return *this;}
    
    Assignment(const Assignment &a) { demand_index = a.demand_index; data = a.data; path = a.path; path_index = a.path_index; }
};

vector<Demand> demand_generator(int network_size, int demand_size)  {
    
    vector<Demand> ds(demand_size);
    
    srand (time(NULL));
    
    for (unsigned int i = 0; i < demand_size; i++) {
        
        Demand d = Demand(
                          i,                      // index
                          rand() % network_size,  // talker
                          rand() % network_size,  // listener
                          rand() % 7,             // service_type
                          float(rand() / (RAND_MAX + 1.)),
                          100.0);                   // no latency bound
        
        ds[i] = d;
    }
    
    return ds;
}

vector<Assignment> get_optimal_assignment(vector<Demand> ds, vector<Assignment> as, int demand_size, int assignment_size) {
    
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
    
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pList, *pList_assignments;
    
    pName = PyString_FromString("main");  // script name
    pModule = PyImport_Import(pName);       // loaded script
    
   // PyErr_PrintEx(1);  // debug
    
    if(pModule == NULL)
        throw std::invalid_argument("Script cannot be loaded.\n");
    
    pDict = PyModule_GetDict(pModule);
    pFunc = PyDict_GetItemString(pDict, "optimize"); // load function
    
    pList = PyList_New(demand_size);
    
    pList_assignments = PyList_New(assignment_size);
    
    for(unsigned int i = 0; i < demand_size; i++) {
        Demand d = ds[i];
        PyObject *pd = Py_BuildValue("(iiiiff)", d.index, d.talker, d.listener, d.service_type, d.max_data, d.max_latency);
        PyList_SetItem(pList, i, pd);
    }
    
    for(unsigned int i = 0; i < assignment_size; i++) {
        Assignment a = as[i];
        PyObject *pd = Py_BuildValue("(ii)", a.demand_index, a.path_index);
        PyList_SetItem(pList_assignments, i, pd);
    }
    
    pArgs = Py_BuildValue("(SS)", pList, pList_assignments);    // load arguments
    
    PyObject* pResult = PyObject_CallObject(pFunc, pArgs); // call function
    
    //PyErr_PrintEx(1);  // debug
    
    if(pResult == NULL)
        return {};
        //throw std::invalid_argument("Method call failed.\n");
    
    // parse output coming from optimization problem
    
    vector<Assignment> path_assignments;
    
    if(PyList_Check(pResult)) {
        int size_assignment = int(PyList_Size(pResult));
        
        for(unsigned int i = 0; i < size_assignment; i++) {
            PyObject *item = PyList_GetItem(pResult, i);
            
            int index = PyInt_AsLong(PyTuple_GetItem(item, 0));
            float data = PyFloat_AsDouble(PyTuple_GetItem(item, 1));
            
            PyObject *path = PyTuple_GetItem(item, 2);
            
            int path_index = PyInt_AsLong(PyTuple_GetItem(item, 3));
            
            int path_length = int(PyList_Size(path));
            
            vector<int> cpath;
            
            for(unsigned int j = 0; j < path_length; j++) {
                cpath.push_back(PyInt_AsLong(PyList_GetItem(path, j)));
            }
            
            Assignment as = Assignment();
            as.demand_index=index;
            as.data=data;
            as.path=cpath;
            as.path_index =path_index;

            path_assignments.push_back(as);
        }
    }
    
    return path_assignments;
}


int main() {
    
    Py_Initialize();
    
    int network_size=27;
    int counter=5;
    int handled =0;
    vector<Demand> ds ;
    vector<Assignment> prevAssignments;
    srand (time(NULL));
    
    int i =0;
    int index=0;
    while(i<counter){
    	cout<<"######### Demand "<<index<<" #########"<<endl;
        Demand d;
    	while(1){
    		d = Demand( index,   rand() % network_size,  rand() % network_size,  rand() % 7,  float(rand() / (RAND_MAX + 1.)),  100.0);
    		if(d.talker!=d.listener)
    		break;
        }

        ds.push_back(d);
        index++;
        
        vector<Assignment> bs = get_optimal_assignment(ds, prevAssignments, ds.size(), prevAssignments.size());
        
        if(bs.empty()){
            ds.pop_back();
	        index--;
            cout<<"Could not embedded ! "<<endl;
        }
        else{
	 
            for (std::vector<Assignment>::iterator it = bs.begin() ; it != bs.end(); ++it){
                int demandId=it->demand_index;
                bool prev=false;
                
                for (std::vector<Assignment>::iterator it1 = prevAssignments.begin(); it1 != prevAssignments.end(); ++it1 ){
                    if( (it1->demand_index)==demandId){
                        prev=true;
                        
                        if(it1->path != it->path){
                            cout<<">New path for previous demand with the id of "<<demandId<<endl;
                            it1->path=it->path; //update path in the prevAssignments list
                            
                            cout<<"PREV PATH ";
                            for(auto x:(it1->path))
                                cout<<x<<" ";
                            cout<<endl;
                            
                            cout<<"NEW PATH ";
                            for(auto x:(it->path))
                                cout<<x<<" ";
                            cout<<endl;
                            
                            
                            cout<<"[Reconfigured Stream]"<<endl;
                            
                        }
                        //else cout<<"\t >>same path for previous demand with the id of "<<demandId<<endl;
                        //do nothing
                        
                    }
                }
                if(prev==false){ //new demand
                    vector<int> path = it->path;
                    
                    cout<<"Assigned path for the demand "<<demandId<<" is ";
                    for(auto x:path)
                        cout<<x<<" ";
                    cout<<endl;
                    prevAssignments.push_back((*it));
                }
                
            }
            handled++;
        }
        i++;
    }
    

    cout<<"SUMMARY"<<endl;
    for (std::vector<Assignment>::iterator it = prevAssignments.begin() ; it != prevAssignments.end(); ++it){
        cout<<"-->assigned path for demand "<<(it->demand_index)<<":";
        vector<int> path=(it->path);
        for(int i=0; i<path.size(); ++i)
        cout << path[i] << ' ';
        cout << " with path index " << (it->path_index)<< endl;
    }
    
    
    cout<<counter<<" demands are sent, "<<handled<<" are handled"<<endl;
    
    Py_Finalize();
    return 0;
}
