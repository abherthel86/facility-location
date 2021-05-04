#include <cmath>     // for hypot()
#include <fstream>   // file stream
#include <sstream>   // string stream used for tokenising

#include "FLPInstance.h"

FLPInstance::FLPInstance(string fname) {
    readInst(fname);
    cout << *this << endl;
}

void FLPInstance::readInst(string fname) {
    string line;
    ifstream in(fname.c_str(), ios::in);
    filename_ = fname;
    if (! in.is_open()){
        cerr << "can't open file: " << fname << endl;
        exit(2);
    }

    // first line = customers, facilities
    in >> n_ >> h_;

    // now let's allocate memory
    d_ = new int [n_ * h_];
    q_ = new int[n_];
    c_ = new int[h_];
    Q_ = new int[h_];
    // Customers
    for (int i = 0; i < n_; i++){
        in >> q_[i];
        // cout << "i: " << i << " demand: " << q(i) << endl;
        // getchar();
        for (int j = 0; j < h_; j++){
            in >> d_[i * h_ + j];
            // cout << "j: " << j << " dist: " << d(i,j) << endl;
            // getchar();
        }
    }
    // Facilities
    for (int j = 0; j < h_; j++){
        in >> c_[j] >> Q_[j];
    }
    in >> delta_;
}

FLPInstance::~FLPInstance() {
    free(d_);
    free(q_);
    free(c_);
    free(Q_);
}

ostream & operator<<(ostream &os, const FLPInstance &instance) {
    os << "Facility location instance loaded from " << instance.filename()
       << " with " << instance.n() << " customers and " << instance.h() << " facilities. ";
    return os;
}
