#ifndef _FLPINSTANCE_H_
#define _FLPINSTANCE_H_

#include <string>    // string operations
#include <iostream>  // printing stuff

using namespace std;

#define rounding_factor 100

class FLPInstance {
protected:
    // a name for this instance
    string filename_;
    // number of customers
    int n_;
    // number of facilities
    int h_;
    // customer demand
    int *q_;
    // facility cost
    int *c_;
    // facility capacity
    int *Q_;
    // delta
    int delta_;
    // distance matrix using a flat representation, i.e. d(i,j) is d[i*n+j]
    // d[i*n+j] already contains the service time at j!
    int *d_;
    // read the provided instance
    void readInst(string filename);

public:
    // a constructor that takes the name of the instance file as input
    FLPInstance(string filename);
    // copy constructor
    FLPInstance(const FLPInstance& original);
    ~FLPInstance();
    
    // getters
    int d(int i, int j) const { return d_[i * h_ + j]; };
    string filename() const { return filename_; };
    int n() const { return n_; };
    int h() const { return h_; };
    int c(int i) const { return c_[i]; };
    int Q(int i) const { return Q_[i]; };
    int q(int i) const { return q_[i]; };
    int delta() const { return delta_; };
};

ostream & operator<<(ostream &os, const FLPInstance &instance);

#endif
