#include "arithmetic.h"
#include <iostream>
#include "string.h"
#include "cmath"
#include "fstream"
int main(int argc, char* argv[]) {
  // read parameters here ...
  string b;
  int a;
  int depth;
  int x,y;
  ofstream myfile;
  vector<unsigned char> msg;
  //to input msg as pgm
  if ((argc !=3 && argc !=6) || strcmp(argv[1],"-encode")==0) {
    cin>>b;
    //handling comments in pgm files 
    while (1) {
    cin>>b;
      if (b[0]!='#')
        break;
      else 
        cin.ignore(5000,'\n');
    }
    x=stoi(b);
    cin>>y;
    b=argv[1];
    cin>>depth;
    //get the msg
    vector<int> file;
    while (cin>>a)
      file.push_back(a);
    //converting msg to unsigned char
    for (int i=0;i<file.size();i++) {
      msg.push_back((unsigned char)file[i]);
  }
  }
  else 
    b=argv[1];
  Arithmetic A;
  vector<bitset<32>> encoded;
  vector<unsigned char> decoded;
  // perform the required operation ...
  if (b.compare("-bin_to_dec")==0) 
      a=0;
  else if (b.compare("-dec_to_bin")==0) 
      a=1;
  else if (b.compare("-encode")==0)
      a=2;
  else if (b.compare("-decode")==0)
      a=3;
  // write output ...
  int bits=A.encode(msg,stoi(argv[2]),&encoded);
  int tt;
  bitset<32>w;
  switch (a)
  {
  case 0:
    cout<<A.binary_to_decimal((bitset<32>)stoul(argv[2], nullptr, 16))<<endl;
    break;
  case 1:
    cout<<hex<<"0x"<<A.decimal_to_binary(stod(argv[2])).to_ulong()<<endl;
    break;
  case 2:
    cout<<encoded.size()<<endl;
    for(int i=0;i<encoded.size();i++) {
      cout<<encoded[i]<<endl;
    }
    for (int i=0;i<msg.size();i++) {
      cout<<(int)msg[i]<<" ";
    }
    cout<<endl;
    cout<<"# of bits = "<<bits<<endl;
    cout<<"# of bytes = "<<(float)bits/8<<endl;
    cout<<"Compression ratio = "<<ceil(log2(depth))*x*y/(float)bits<<endl;
    break;
  case 3:
    cin>>tt;
    for  (int i=0;i<tt;i++) {
      cin>>w;
      encoded.push_back(w);
    }
    A.decode(encoded,stoi(argv[2]),&decoded);
    cout<<"image decoded successfully you can find it as decoded.pgm\n";
    myfile.open("decoded.pgm");
    myfile.clear();
    myfile<<"P2"<<endl<<stoi(argv[3])<<" "<<stoi(argv[4])<<endl<<stoi(argv[5])<<endl;
    for (int i=0;i<decoded.size();i++) {
      if(i%24==0&&i!=0)
        myfile<<endl;
      myfile<<(int)decoded[i]<<" ";
    }
    myfile.close();
    break;
  
  default:
    break;
  }
  return 0;
}
