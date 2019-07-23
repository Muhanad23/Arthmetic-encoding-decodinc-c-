// Your includes here
#include "arithmetic.h"
#include "cmath"
#include <iostream>
#include <iomanip>
// Your implementations here

double Arithmetic::binary_to_decimal(const bitset<32>& fraction) {
    double frac=0;
    int m=fraction.size();
    for (int i=m-1;i>=0;i--) {
        frac+=pow(2,i-m)*fraction[i];
        }
    return frac;
}

bitset<32> Arithmetic::decimal_to_binary(double fraction) {
    bitset<32>b;
    b.reset();
    for (int i=0;i<32;i++) {
            fraction*=2;
        if (fraction>=1) {
            b.set(31-i);
            fraction-=1;
        }
    }
    return b;
}

int Arithmetic::encode(const vector<unsigned char>& msg,int num_symbols_per_block,vector<bitset<32> >* encoded_msg) {
    //to detect if the symbol is found in the vector
    bool isFound;
    int m=msg.size();
    int totalBits=0;
    //Where we are from the msg
    int n=0;
    double L,U,L0,U0,Fx0,Fx;
    //calculate probability for each symbol
    for (int i=0;i<m;i++) {
        isFound=false;
        for (int j=0;j<CDF.size();j++) {
            if (msg[i]==CDF[j].first) {
                isFound=true;
                CDF[j].second+=1/(float)m;
                break;
            }
        }
        if (!isFound)
            CDF.push_back(make_pair(msg[i],1/(float)m));
    }
    //Store CDF for every item in CDF vector
    for (int i=1;i<CDF.size();i++) 
        CDF[i].second+=CDF[i-1].second;
    while (n<m) {
        //initialize L0,U0
        for (int j=0;j<CDF.size();j++) {
                if (CDF[j].first==msg[n]) {
                    U0=CDF[j].second;
                    if (j!=0)
                        L0=CDF[j-1].second;
                    else 
                        L0=0;
                    break;
                }
            }
            n++;
        for (int i=1;i<num_symbols_per_block;i++) {
            if (n>=m) {
                L=L0;
                U=U0;
                break;
            }          
            for (int j=0;j<CDF.size();j++) {
                if (CDF[j].first==msg[n]) {
                    Fx=CDF[j].second;
                    if (j!=0)
                        Fx0=CDF[j-1].second;
                    else 
                        Fx0=0;
                    break;
                }
            }
            L=L0+(U0-L0)*Fx0;
            U=L0+(U0-L0)*Fx;
            n++;
            L0=L;
            U0=U;
        }
        if (num_symbols_per_block==1) {
            L=L0;
            U=U0;
        }
        totalBits+=ceil(log2(1/(U-L)))+1;
        encoded_msg->push_back(decimal_to_binary((L+U)/2));
    }
    return totalBits;
}

void Arithmetic::decode(const vector<bitset<32> >& encoded_msg,int num_symbols_per_block,
vector<unsigned char>* decoded_msg) {
    double L0,U0,tag,t,Fx,Fx0,L,U;
    int s;
    vector <unsigned char> r;
    //Getting msg to calculate CDF
    while (cin>>s) {
        r.push_back(s);
    }
    int m=r.size();
    bool isFound;
    for (int i=0;i<m;i++) {
        isFound=false;
        for (int j=0;j<CDF.size();j++) {
            if (r[i]==CDF[j].first) {
                isFound=true;
                CDF[j].second+=1/(float)m;
                break;
            }
        }
        if (!isFound)
            CDF.push_back(make_pair(r[i],1/(float)m));
    }
    //Store CDF for every item in CDF vector
    for (int i=1;i<CDF.size();i++) 
        CDF[i].second+=CDF[i-1].second;

    int n=0;
    for (int i=0;i<encoded_msg.size();i++) {
        //initialize U0,L0
        U0=1;
        L0=0;
        tag=binary_to_decimal(encoded_msg[i]);
        //Loop on each block
        for (int j=0;j<num_symbols_per_block;j++) {
            if (n>=m)
                break;
            t=(tag-L0)/(U0-L0);
            for (int k=1;k<CDF.size();k++) {
                if (CDF[0].second>=t) {
                    decoded_msg->push_back(CDF[0].first);
                    Fx=CDF[0].second;
                    Fx0=0;
                    n++;
                    break;
                }
                else if (t>=CDF[k-1].second && t<=CDF[k].second) {
                    decoded_msg->push_back(CDF[k].first);
                    Fx=CDF[k].second;
                    Fx0=CDF[k-1].second;
                    n++;
                    break;
                }
            }
            L=L0+(U0-L0)*Fx0;
            U=L0+(U0-L0)*Fx;
            L0=L;
            U0=U;
        }
    }
    
}
  